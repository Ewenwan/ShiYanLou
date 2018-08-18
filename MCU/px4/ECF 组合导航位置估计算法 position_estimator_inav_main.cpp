/****************************************************************************
 *
 组合导航 integrated navigation 位置估计器 
 
 互补滤波 
 *
 ****************************************************************************/

/**
 * @file position_estimator_inav_main.c
 * Model-identification based position estimator for multirotors
 *
 * @author Anton Babushkin <anton.babushkin@me.com>
 * @author Nuno Marques <n.marques21@hotmail.com>
 * @author Christoph Tobler <toblech@student.ethz.ch>
 */
#include <px4_posix.h>
#include <px4_tasks.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <px4_config.h>
#include <math.h>
#include <float.h>

/*话题发布订阅*/ 
#include <uORB/uORB.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/actuator_controls.h>
#include <uORB/topics/actuator_armed.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/vehicle_global_position.h>
#include <uORB/topics/vehicle_gps_position.h>
#include <uORB/topics/vision_position_estimate.h>
#include <uORB/topics/att_pos_mocap.h>
#include <uORB/topics/optical_flow.h>
#include <uORB/topics/distance_sensor.h>
#include <poll.h>


#include <systemlib/err.h>
#include <systemlib/mavlink_log.h>
#include <geo/geo.h>
#include <drivers/drv_hrt.h>
#include <platforms/px4_defines.h>

#include <terrain_estimation/terrain_estimator.h>
#include "position_estimator_inav_params.h"
#include "inertial_filter.h"

#define MIN_VALID_W 0.00001f  // 用于检测是否为零 
#define PUB_INTERVAL 10000	  // 发布速率 限制 limit publish rate to 100 Hz
#define EST_BUF_SIZE 250000 / PUB_INTERVAL	 // 缓存 0.5s   buffer size is 0.5s
#define MAX_WAIT_FOR_BARO_SAMPLE 3000000     // 等待气压计响应时间限制  wait 3 secs for the baro to respond

static bool thread_should_exit = false; /**< Deamon exit flag */
static bool thread_running = false; /**< Deamon status flag */
static int position_estimator_inav_task; /**< Handle of deamon task / thread */

static bool inav_verbose_mode = false;//在linux中：--verbose 显示指令执行过程
// 守护进程状态，守护进程：运行于后台，并周期性的执行某种任务；

static const hrt_abstime vision_topic_timeout = 500000;	// 视觉 超时     Vision topic timeout = 0.5s
static const hrt_abstime mocap_topic_timeout = 500000;	// 动作捕捉超时  Mocap topic timeout = 0.5s
static const hrt_abstime gps_topic_timeout = 500000;	// GPS话题超时   GPS topic timeout = 0.5s
static const hrt_abstime flow_topic_timeout = 1000000;	// 光流 超时     optical flow topic timeout = 1s
static const hrt_abstime lidar_timeout = 150000;	    // 激光雷达超时  lidar timeout = 150ms
static const hrt_abstime lidar_valid_timeout = 1000000;	// 估计雷达 位置 estimate lidar distance during this time after lidar loss
static const unsigned updates_counter_len = 1000000;    // 更新计数器 步长
static const float max_flow = 1.0f;	                    // 光流输出上限  max flow value that can be used, rad/s

/*//以 c 方式编译，并输出函数接口； 重定向*/
//这是因为c++有重载，但是c没有，所以c代码需要作出声明，好进行链接
extern "C" __EXPORT int position_estimator_inav_main(int argc, char *argv[]);

int position_estimator_inav_thread_main(int argc, char *argv[]);

static void usage(const char *reason);//打印命令调用格式信息；

static inline int min(int val1, int val2)//最小值
{
	return (val1 < val2) ? val1 : val2;
}

static inline int max(int val1, int val2)//最大值
{
	return (val1 > val2) ? val1 : val2;
}

/**
 * Print the correct usage.
 */
static void usage(const char *reason)//打印命令调用格式信息；
{
	if (reason && *reason) {
		PX4_INFO("%s", reason);
	}

	PX4_INFO("usage: position_estimator_inav {start|stop|status} [-v]\n");
}

/**
 * The position_estimator_inav_thread only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 *
 * The actual stack size should be set in the call
 * to task_create().
 
 *  position_estimator_inav_thread 进程只是短暂存在，用于启动后台进程。在makefile
 * 中指定的堆栈大小仅用于该管理进程。
 * 实际的堆栈大小需要在调用 task_create() 时设置。
 */
int position_estimator_inav_main(int argc, char *argv[])
{
	if (argc < 2) {
		usage("missing command");//指令错误，并打印正确调用格式；
	}
/*启动进程*/ 
	if (!strcmp(argv[1], "start")) {//启动进程
		if (thread_running) {       //判断是否已经启动;
			warnx("already running");//显示启动信息 
			/* this is not an error */
			return 0;
		}

		inav_verbose_mode = false;//初始化参数；v   进程详细信息

		if ((argc > 2) && (!strcmp(argv[2], "-v"))) {
			inav_verbose_mode = true;//若有参数 -v，则打印进程详细信息；
		}

		thread_should_exit = false;// 退出进程 参数初始化为 false  
		//px4_task_spawn_cmd()，此处为 POSIX 接口的 任务进程创建函数
		position_estimator_inav_task = px4_task_spawn_cmd("position_estimator_inav",//进程名称；
					       SCHED_DEFAULT, SCHED_PRIORITY_MAX - 5, 4600,//任务调度模式，优先级，堆栈大小(非确定)；
					       position_estimator_inav_thread_main,        //进程入口函数；
					       (argv && argc > 2) ? (char *const *) &argv[2] : (char *const *) nullptr);// 第三个 参数   指针 
		return 0;
	}

/*停止进程*/ 
	if (!strcmp(argv[1], "stop")) {//进程停止
		if (thread_running) {
			warnx("stop");////显示停止信息
			thread_should_exit = true;//设置标识位，在线程中自动跳出 while 循环并结束；

		} else {
			warnx("not started");//本身就没有开始 
		}

		return 0;
	}
	
/* 查看进程状态 是否启动 */
	if (!strcmp(argv[1], "status")) {//进程状态，打印当前进程状态及用法；
		if (thread_running) {
			warnx("is running");

		} else {
			warnx("not started");
		}

		return 0;
	}

	usage("unrecognized command");
	return 1;
}

/*调试 组合导航算法 */
#ifdef INAV_DEBUG//调试打印函数
static void write_debug_log(const char *msg, float dt, float x_est[2], float y_est[2], float z_est[2],
			    float x_est_prev[2], float y_est_prev[2], float z_est_prev[2],
			    float acc[3], float corr_gps[3][2], float w_xy_gps_p, float w_xy_gps_v, float corr_mocap[3][1], float w_mocap_p,
			    float corr_vision[3][2], float w_xy_vision_p, float w_z_vision_p, float w_xy_vision_v)
{
	FILE *f = fopen(PX4_ROOTFSDIR"/fs/microsd/inav.log", "a");

	if (f) {
		char *s = malloc(256);
		unsigned n = snprintf(s, 256,
				      "%llu %s\n\tdt=%.5f x_est=[%.5f %.5f] y_est=[%.5f %.5f] z_est=[%.5f %.5f] x_est_prev=[%.5f %.5f] y_est_prev=[%.5f %.5f] z_est_prev=[%.5f %.5f]\n",
				      (unsigned long long)hrt_absolute_time(), msg, (double)dt,
				      (double)x_est[0], (double)x_est[1], (double)y_est[0], (double)y_est[1], (double)z_est[0], (double)z_est[1],
				      (double)x_est_prev[0], (double)x_est_prev[1], (double)y_est_prev[0], (double)y_est_prev[1], (double)z_est_prev[0],
				      (double)z_est_prev[1]);
		fwrite(s, 1, n, f);
		n = snprintf(s, 256,
			     "\tacc=[%.5f %.5f %.5f] gps_pos_corr=[%.5f %.5f %.5f] gps_vel_corr=[%.5f %.5f %.5f] w_xy_gps_p=%.5f w_xy_gps_v=%.5f mocap_pos_corr=[%.5f %.5f %.5f] w_mocap_p=%.5f\n",
			     (double)acc[0], (double)acc[1], (double)acc[2],
			     (double)corr_gps[0][0], (double)corr_gps[1][0], (double)corr_gps[2][0], (double)corr_gps[0][1], (double)corr_gps[1][1],
			     (double)corr_gps[2][1],
			     (double)w_xy_gps_p, (double)w_xy_gps_v, (double)corr_mocap[0][0], (double)corr_mocap[1][0], (double)corr_mocap[2][0],
			     (double)w_mocap_p);
		fwrite(s, 1, n, f);
		n = snprintf(s, 256,
			     "\tvision_pos_corr=[%.5f %.5f %.5f] vision_vel_corr=[%.5f %.5f %.5f] w_xy_vision_p=%.5f w_z_vision_p=%.5f w_xy_vision_v=%.5f\n",
			     (double)corr_vision[0][0], (double)corr_vision[1][0], (double)corr_vision[2][0], (double)corr_vision[0][1],
			     (double)corr_vision[1][1], (double)corr_vision[2][1],
			     (double)w_xy_vision_p, (double)w_z_vision_p, (double)w_xy_vision_v);
		fwrite(s, 1, n, f);
		free(s);
	}

	fsync(fileno(f));
	fclose(f);
}
#else
#define write_debug_log(...)  //此处 ... 为占位符，用于在 c 语言中实现函数重载。
#endif

/****************************************************************************
 * main //进程入口主函数；
 ****************************************************************************/
int position_estimator_inav_thread_main(int argc, char *argv[])
{
	orb_advert_t mavlink_log_pub = nullptr;

	float x_est[2] = { 0.0f, 0.0f };	// pos, vel  [位置，速度] x轴 初始化 
	float y_est[2] = { 0.0f, 0.0f };	// pos, vel
	float z_est[2] = { 0.0f, 0.0f };	// pos, vel
	
 //缓存数据：
	float est_buf[EST_BUF_SIZE][3][2];	// estimated position buffer  （位置估计）
	float R_buf[EST_BUF_SIZE][3][3];	// rotation matrix buffer   （旋转矩阵）
	float R_gps[3][3];					// rotation matrix for GPS correction moment（当前时刻 用于 GPS 校正的 旋转矩阵）
	memset(est_buf, 0, sizeof(est_buf));// 相应内存单元初始化为 0 
	memset(R_buf, 0, sizeof(R_buf));
	memset(R_gps, 0, sizeof(R_gps));
	int buf_ptr = 0;//内存指针 
	
// GPS 水平定位精度参数EPH、垂直定位精度参数EPV
	static const float min_eph_epv = 2.0f;	// min EPH/EPV, used for weight calculation  权重计算（最小误差）
	static const float max_eph_epv = 20.0f;	// max EPH/EPV acceptable for estimation     估计    （最大误差）
// 各设备信息 精度 
	float eph = max_eph_epv;
	float epv = 1.0f;

	float eph_flow = 1.0f;

	float eph_vision = 0.2f;
	float epv_vision = 0.2f;

	float eph_mocap = 0.05f; // 光学运动捕捉系统 vicon   Motion Capture
	float epv_mocap = 0.05f;
//从上面的参数设置来看，果然精度还是 vicon > 视觉 > 光流，花钱就是D；
	
 // 上一时刻的估计值，初始化为0；  previous  
	float x_est_prev[2], y_est_prev[2], z_est_prev[2];
	memset(x_est_prev, 0, sizeof(x_est_prev));// 相应内存单元初始化为 0
	memset(y_est_prev, 0, sizeof(y_est_prev));
	memset(z_est_prev, 0, sizeof(z_est_prev));
	
 // 气压计相关参数，200个值求平均，得到气压计读数；
	int baro_init_cnt = 0;
	int baro_init_num = 200;
	float baro_offset = 0.0f; // 偏置   baro offset for reference altitude, initialized on start, then adjusted

	hrt_abstime accel_timestamp = 0; //加速度计的时间
	hrt_abstime baro_timestamp = 0;  //气压计时间

	bool ref_inited = false;         //参考位置初始化  reference location  
	hrt_abstime ref_init_start = 0;  //  
	const hrt_abstime ref_init_delay = 1000000;	// wait for 1s after 3D fix
	struct map_projection_reference_s ref;      //参考位置，启动 1s 后初始化该值；
	memset(&ref, 0, sizeof(ref));

//用于计算刷新率；
	uint16_t accel_updates = 0;  
	uint16_t baro_updates = 0;
	uint16_t gps_updates = 0;
	uint16_t attitude_updates = 0;
	uint16_t flow_updates = 0;
	uint16_t vision_updates = 0;
	uint16_t mocap_updates = 0;

	hrt_abstime updates_counter_start = hrt_absolute_time();//时间，用于计算刷新率
	hrt_abstime pub_last = hrt_absolute_time();             //时间，用于将当前估计值存入缓存；

	hrt_abstime t_prev = 0;

// 存储误差 
	/* store error when sensor updates, but correct on each time step to avoid jumps in estimated value */
	float acc[] = { 0.0f, 0.0f, 0.0f };	        // N E D 导航坐标系  加速度   N、E和D分别表示北向、东向和地向.
	float acc_bias[] = { 0.0f, 0.0f, 0.0f };	// body frame   机体坐标系
	float corr_baro = 0.0f;		// D   气压计 导航坐标系 Z轴
	float corr_gps[3][2] = {    // 导航坐标系
		{ 0.0f, 0.0f },		// N (pos, vel)
		{ 0.0f, 0.0f },		// E (pos, vel)
		{ 0.0f, 0.0f },		// D (pos, vel)
	};
	float w_gps_xy = 1.0f; //GPS权重
	float w_gps_z = 1.0f; 

// 视觉 导航坐标系  位置  速度   修正 
	float corr_vision[3][2] = {
		{ 0.0f, 0.0f },		// N (pos, vel)
		{ 0.0f, 0.0f },		// E (pos, vel)
		{ 0.0f, 0.0f },		// D (pos, vel)
	};
// 视觉 导航坐标系  位置  速度   修正 
	float corr_mocap[3][1] = {
		{ 0.0f },		// N (pos)
		{ 0.0f },		// E (pos)
		{ 0.0f },		// D (pos)
	};
	const int mocap_heading = 2; //外部偏航角模式 ，2 表示 动作捕捉系统；
	
 //激光 雷达 lidar 高度估计相关的变量
	float dist_ground = 0.0f; //variables for lidar altitude estimation
	float corr_lidar = 0.0f;
	float lidar_offset = 0.0f;
	int lidar_offset_count = 0;
	bool lidar_first = true;
	bool use_lidar = false;
	bool use_lidar_prev = false;
	
// N E 光流
	float corr_flow[] = { 0.0f, 0.0f };	// N E
	float w_flow = 0.0f;

	hrt_abstime lidar_time = 0;			// time of last lidar measurement (not filtered)
	hrt_abstime lidar_valid_time = 0;	// time of last lidar measurement used for correction (filtered)

	int n_flow = 0;
	float gyro_offset_filtered[] = { 0.0f, 0.0f, 0.0f };
	float flow_gyrospeed[] = { 0.0f, 0.0f, 0.0f };
	float flow_gyrospeed_filtered[] = { 0.0f, 0.0f, 0.0f };
	float att_gyrospeed_filtered[] = { 0.0f, 0.0f, 0.0f };
	float yaw_comp[] = { 0.0f, 0.0f };//偏航补偿，光流安装位置不在中心时引起的偏差；
	hrt_abstime flow_time = 0; //光流
	float flow_min_dist = 0.2f;//光流最小距离
	
  //各个传感器是否正常工作；
	bool gps_valid = false;			// GPS is valid
	bool lidar_valid = false;		// lidar is valid
	bool flow_valid = false;		// flow is valid
	bool flow_accurate = false;		// flow should be accurate (this flag not updated if flow_valid == false)
	bool vision_valid = false;		// vision is valid
	bool mocap_valid = false;		// mocap is valid

	/* declare and safely initialize all structs */
	// 个话题消息数据类型 变量结构体 初始化 
	struct actuator_controls_s actuator;
	memset(&actuator, 0, sizeof(actuator)); //电机控制参数
	struct actuator_armed_s armed;
	memset(&armed, 0, sizeof(armed));       //电机解锁状态参数
	struct sensor_combined_s sensor;
	memset(&sensor, 0, sizeof(sensor));     //传感器读数（陀螺仪、加速度计、磁力计、气压计） 
	struct vehicle_gps_position_s gps;
	memset(&gps, 0, sizeof(gps));           // GPS 信息
	struct vehicle_attitude_s att;
	memset(&att, 0, sizeof(att));           // 姿态信息 
	struct vehicle_local_position_s local_pos;
	memset(&local_pos, 0, sizeof(local_pos));  //位置信息（NED）
	struct optical_flow_s flow;
	memset(&flow, 0, sizeof(flow));            //光流读数 
	struct vision_position_estimate_s vision;
	memset(&vision, 0, sizeof(vision));        //视觉位置估计
	struct att_pos_mocap_s mocap;
	memset(&mocap, 0, sizeof(mocap));           //动作捕捉系统
	struct vehicle_global_position_s global_pos;//以 GPS 为主的位置估计
	memset(&global_pos, 0, sizeof(global_pos));
	struct distance_sensor_s lidar;
	memset(&lidar, 0, sizeof(lidar));           //激光 雷达 lidar
	struct vehicle_rates_setpoint_s rates_setpoint; //频率 
	memset(&rates_setpoint, 0, sizeof(rates_setpoint));

	/* subscribe 订阅话题，返回值为 句柄*/
	int parameter_update_sub = orb_subscribe(ORB_ID(parameter_update)); //参数更新 
	int actuator_sub = orb_subscribe(ORB_ID_VEHICLE_ATTITUDE_CONTROLS); //执行器电机话题 
	int armed_sub = orb_subscribe(ORB_ID(actuator_armed));              //执行器电机解锁 
	int sensor_combined_sub = orb_subscribe(ORB_ID(sensor_combined));   //传感器读数（陀螺仪、加速度计、磁力计、气压计）
	int vehicle_attitude_sub = orb_subscribe(ORB_ID(vehicle_attitude)); //机身姿态信息  roll  pich  yow  
	int optical_flow_sub = orb_subscribe(ORB_ID(optical_flow));         //光流信息 
	int vehicle_gps_position_sub = orb_subscribe(ORB_ID(vehicle_gps_position));//机体 GPS 位置  
	int vision_position_estimate_sub = orb_subscribe(ORB_ID(vision_position_estimate));//视觉位置估计 
	int att_pos_mocap_sub = orb_subscribe(ORB_ID(att_pos_mocap));                      //姿态位置 动作捕捉 
	int distance_sensor_sub = orb_subscribe(ORB_ID(distance_sensor));                  //位置距离传感器 (激光雷达 超声 红外  无线电雷达) 
	
	int vehicle_rate_sp_sub = orb_subscribe(ORB_ID(vehicle_rates_setpoint));           //角速度设定值  话题 

	/* advertise 广播 话题 publish ，返回值为 句柄  */
	orb_advert_t vehicle_local_position_pub = orb_advertise(ORB_ID(vehicle_local_position), &local_pos);// 机身 组合导航位置估计 本地位置 
	orb_advert_t vehicle_global_position_pub = nullptr;

   // position_estimator_inav_params.h中定义的参数：
	struct position_estimator_inav_params params;//参数 
	memset(&params, 0, sizeof(params));          //初始化参数 
	struct position_estimator_inav_param_handles pos_inav_param_handles;//参数句柄 
	/*   //初始化参数句柄； initialize parameter handles */
	inav_parameters_init(&pos_inav_param_handles);

	/* 参数 默认初始化  */
	struct parameter_update_s param_update;
	orb_copy(ORB_ID(parameter_update), parameter_update_sub, //初始化更新状态参数，没有实际作用；
		 &param_update); /* read from param topic to clear updated flag */
	/* first parameters update */
	inav_parameters_update(&pos_inav_param_handles, &params);//执行更新 

/*// 阻塞等待   sensor_combined 话题消息  初始化 */
	px4_pollfd_struct_t fds_init[1] = {};
	fds_init[0].fd = sensor_combined_sub;// 传感器订阅句柄->文件设备描述符
	fds_init[0].events = POLLIN;         // 有数据可读 

	/* wait for initial baro value */
	bool wait_baro = true;
	TerrainEstimator terrain_estimator; //地形估计，用于估计对地距离；

	thread_running = true;
	hrt_abstime baro_wait_for_sample_time = hrt_absolute_time();

/*气压计高度初始化 得到起飞点高度*/ 
	while (wait_baro && !thread_should_exit) { //while 循环用于 气压计数据初始化 且 thread_should_exit 线程未结束 
	    
		/*poll函数调用阻塞的时间为1s*/
		int ret = px4_poll(&fds_init[0], 1, 1000);//读取传感器，数组维度 1，超时最大 1000ms， 最小1HZ；
	// 返回值：
    //>0：数组fds中准备好读、写或出错状态的那些socket描述符的总数量；
    //==0:poll()函数会阻塞timeout所指定的毫秒时间长度之后返回;
    //-1:poll函数调用失败；同时会自动设置全局变量errno；
    
		if (ret < 0) {
			/* poll error */ // 读取失败；
			mavlink_log_info(&mavlink_log_pub, "[inav] poll error on init");

		} else if (hrt_absolute_time() - baro_wait_for_sample_time > MAX_WAIT_FOR_BARO_SAMPLE) { //气压计超时，无数据???
			wait_baro = false;//气压计数据读取失败，结束while循环；
			mavlink_log_info(&mavlink_log_pub, "[inav] timed out waiting for a baro sample");

		} else if (ret > 0) {//有数据 
			if (fds_init[0].revents & POLLIN) {
				orb_copy(ORB_ID(sensor_combined), sensor_combined_sub, &sensor);//复制传感器话题上的 气压计消息到本地内存 

				if (wait_baro && sensor.timestamp + sensor.baro_timestamp_relative != baro_timestamp) {//气压计消息 得到更新 
					baro_timestamp = sensor.timestamp + sensor.baro_timestamp_relative;//更新 气压计本次更新消息 
					baro_wait_for_sample_time = hrt_absolute_time();//当前时间 

					/* mean calculation over several measurements */
					if (baro_init_cnt < baro_init_num) {//累加200个气压计读数求均值；
						if (PX4_ISFINITE(sensor.baro_alt_meter)) {
							baro_offset += sensor.baro_alt_meter;// 求和 
							baro_init_cnt++;
						}

					} else {
						wait_baro = false;//结束while循环，完成气压计高度初始化；
						baro_offset /= (float) baro_init_cnt;//均值   气压计高度； 起飞点高度(一开始气压计定高高度)
						local_pos.z_valid = true;     //气压计数据有效；
						local_pos.v_z_valid = true;
					}
				}
			}

		} else {
			PX4_WARN("INAV poll timeout");// 读取数据超时 
		}
	}

/* 进程主循环  */ 
	/* main loop */
	px4_pollfd_struct_t fds[1];
	fds[0].fd = vehicle_attitude_sub;//订阅 机身姿态信息 话题 
	fds[0].events = POLLIN;

	while (!thread_should_exit) {
		int ret = px4_poll(fds, 1, 20); // wait maximal 20 ms = 50 Hz minimum rate
		hrt_abstime t = hrt_absolute_time();//当前时间 

		if (ret < 0) {
			/* poll error */// 读取失败；
			mavlink_log_info(&mavlink_log_pub, "[inav] poll error on init");
			continue;

		} else if (ret > 0) {//有姿态数据 
			/* act on attitude updates */

			/* vehicle attitude */
			orb_copy(ORB_ID(vehicle_attitude), vehicle_attitude_sub, &att);//复制姿态消息到本地内存 
			attitude_updates++;//姿态消息计数 

			bool updated;

			/* parameter update */
			orb_check(parameter_update_sub, &updated);// 参数数据话题 有更新吗 

			if (updated) {//参数 有更新的话 更新参数 
				struct parameter_update_s update;
				orb_copy(ORB_ID(parameter_update), parameter_update_sub, &update);//复制参数话题数据到本地内存 
				inav_parameters_update(&pos_inav_param_handles, &params);//更新参数 
			}

			/* actuator 执行器话题是否更新*/
			orb_check(actuator_sub, &updated); 

			if (updated) { // actuator_controls_0 执行器控制信息，通过混控器  mixer 之前输出 PWM 控制电机 
				orb_copy(ORB_ID_VEHICLE_ATTITUDE_CONTROLS, actuator_sub, &actuator);//复制执行器控制信息到本地 
			}

			/* armed */
			orb_check(armed_sub, &updated);//执行器是否解锁 话题是否更新 

			if (updated) {//如果更新了 
				orb_copy(ORB_ID(actuator_armed), armed_sub, &armed);//复制 执行器解锁标志 到本地 
			}
/***************************************************************************************/ 
// 获得机体 相对于 地理坐标系  的 加速度向量
 
			/* sensor combined  传感器数据话题是否更新 */
			orb_check(sensor_combined_sub, &updated);

			matrix::Dcmf R = matrix::Quatf(att.q);//用于提取旋转矩阵对应元素；

			if (updated) {
				orb_copy(ORB_ID(sensor_combined), sensor_combined_sub, &sensor);//复制 传感器消息到本地内存变量 

				if (sensor.timestamp + sensor.accelerometer_timestamp_relative != accel_timestamp) {//消息有更新 
					/* correct accel bias 校准加速度偏差 */
					sensor.accelerometer_m_s2[0] -= acc_bias[0];// //加速度计原始数据（m/s^2），减去偏移量；
					sensor.accelerometer_m_s2[1] -= acc_bias[1];
					sensor.accelerometer_m_s2[2] -= acc_bias[2];

					/* transform acceleration vector from body frame to NED frame */
					// 将加速度 向量 从  机身坐标系下 转换到  世界坐标系NED坐标系 
					for (int i = 0; i < 3; i++) {
						acc[i] = 0.0f;//初始化为0 

						for (int j = 0; j < 3; j++) {
							acc[i] += R(i, j) * sensor.accelerometer_m_s2[j];
						}
					}

					acc[2] += CONSTANTS_ONE_G;//Z轴 ， 重力加速度补偿；

					accel_timestamp = sensor.timestamp + sensor.accelerometer_timestamp_relative;//加速度消息时间戳更新 
					accel_updates++;//加速度消息计数 
				}

				if (sensor.timestamp + sensor.baro_timestamp_relative != baro_timestamp) {//气压计消息 已经 更新 
					corr_baro = baro_offset - sensor.baro_alt_meter - z_est[0];
					//高度差 = 起飞点高度(一开始气压计定高高度) - 气压计当前高度 - z轴高度（负）
					baro_timestamp = sensor.timestamp + sensor.baro_timestamp_relative;
					baro_updates++;//气压计数据 计数 
				}
			}

/***************************************************************************************/ 
//激光雷达测高度差 
			/* lidar alt estimation 距离传感器数据  检查激光雷达数据是否更新*/
			orb_check(distance_sensor_sub, &updated);

			/* update lidar separately, needed by terrain estimator */
			if (updated) {
				orb_copy(ORB_ID(distance_sensor), distance_sensor_sub, &lidar);//获取 lidar 读数， 
				lidar.current_distance += params.lidar_calibration_offset;     //获取 lidar 读数，并补偿偏移量。
			}

			if (updated) { //check if altitude estimation for lidar is enabled and new sensor data

				if (params.enable_lidar_alt_est && lidar.current_distance > lidar.min_distance
				    && lidar.current_distance < lidar.max_distance
				    && (R(2, 2) > 0.7f)) {//R[3,3]=cos(横滚角)*cos(俯仰)>0.7；表示横滚与俯仰 小于45°；

					if (!use_lidar_prev && use_lidar) {//等待 use_lidar=true;prev=false;
						lidar_first = true;
					}

					use_lidar_prev = use_lidar;//雷达数据使用 

					lidar_time = t;
					dist_ground = lidar.current_distance * R(2, 2); //vertical distance

					if (lidar_first) {
						lidar_first = false;
						lidar_offset = dist_ground + z_est[0];//获取 lidar 偏差；z轴高度
						mavlink_log_info(&mavlink_log_pub, "[inav] LIDAR: new ground offset");
						warnx("[inav] LIDAR: new ground offset");
					}

					corr_lidar = lidar_offset - dist_ground - z_est[0];//由 lidar 得到的高度差；

					if (fabsf(corr_lidar) > params.lidar_err) { //check for spike  //检查野值；
						corr_lidar = 0;
						lidar_valid = false;//激光雷达数据  不可靠 
						lidar_offset_count++;

						if (lidar_offset_count > 3) { //if consecutive bigger/smaller measurements -> new ground offset -> reinit
							lidar_first = true; //稳定的偏差变化->到达新地形->重新初始化;
							lidar_offset_count = 0;
						}

					} else {//lidar 数据有效；
						corr_lidar = lidar_offset - dist_ground - z_est[0];
						lidar_valid = true;
						lidar_offset_count = 0;
						lidar_valid_time = t;
					}

				} else {
					lidar_valid = false;
				}
			}
/***************************************************************************************/ 
//  光流数据 
			/* optical flow 光流数据话题 */
			orb_check(optical_flow_sub, &updated);//话题是否更新 

			if (updated && lidar_valid) {//更新了 且 雷达数据有效 
				orb_copy(ORB_ID(optical_flow), optical_flow_sub, &flow);//得到光流数据 

				flow_time = t;
				float flow_q = flow.quality / 255.0f;//归一化；
				float dist_bottom = lidar.current_distance;//当前距离 

				if (dist_bottom > flow_min_dist && flow_q > params.flow_q_min && R(2, 2) > 0.7f) {
					//R[3,3]=cos(横滚角)*cos(俯仰)>0.7；表示横滚与俯仰 小于45°；
					/* distance to surface */
					//float flow_dist = dist_bottom / PX4_R(att.R, 2, 2); //use this if using sonar
					float flow_dist = dist_bottom; //use this if using lidar

					/* check if flow if too large for accurate measurements */
					/* calculate estimated velocity in body frame */
					float body_v_est[2] = { 0.0f, 0.0f };

					for (int i = 0; i < 2; i++) {
						body_v_est[i] = R(0, i) * x_est[1] + R(1, i) * y_est[1] + R(2, i) * z_est[1];//x、y轴的速度，坐标转换
					}

					/* set this flag if flow should be accurate according to current velocity and attitude rate estimate */
					flow_accurate = fabsf(body_v_est[1] / flow_dist - att.rollspeed) < max_flow &&
							fabsf(body_v_est[0] / flow_dist + att.pitchspeed) < max_flow;
                    // V_xy / height，相差较大时，可近似为角速度w，与 roll/pitch 相减后，检查是否超出光流图像刷新率. [sin(0)~tan(0)~0]


					/*calculate offset of flow-gyro using already calibrated gyro from autopilot*/
					 //积分值 / 积分时间 = 角速度；（时间单位us？？）
					flow_gyrospeed[0] = flow.gyro_x_rate_integral / (float)flow.integration_timespan * 1000000.0f;
					flow_gyrospeed[1] = flow.gyro_y_rate_integral / (float)flow.integration_timespan * 1000000.0f;
					flow_gyrospeed[2] = flow.gyro_z_rate_integral / (float)flow.integration_timespan * 1000000.0f;

					//moving average
					if (n_flow >= 100) {
						//根据校准后的姿态att，获取光流陀螺仪的偏差；
                      //滤波后数据：经过 100 点数据低通滤波获得；
						gyro_offset_filtered[0] = flow_gyrospeed_filtered[0] - att_gyrospeed_filtered[0];
						gyro_offset_filtered[1] = flow_gyrospeed_filtered[1] - att_gyrospeed_filtered[1];
						gyro_offset_filtered[2] = flow_gyrospeed_filtered[2] - att_gyrospeed_filtered[2];
						 //数据恢复，准备下一次校准；
						n_flow = 0;
						flow_gyrospeed_filtered[0] = 0.0f;
						flow_gyrospeed_filtered[1] = 0.0f;
						flow_gyrospeed_filtered[2] = 0.0f;
						att_gyrospeed_filtered[0] = 0.0f;
						att_gyrospeed_filtered[1] = 0.0f;
						att_gyrospeed_filtered[2] = 0.0f;

					} else {
						 //对 flow_gyrospeed光流陀螺仪角速度 低通滤波；
						flow_gyrospeed_filtered[0] = (flow_gyrospeed[0] + n_flow * flow_gyrospeed_filtered[0]) / (n_flow + 1);
						flow_gyrospeed_filtered[1] = (flow_gyrospeed[1] + n_flow * flow_gyrospeed_filtered[1]) / (n_flow + 1);
						flow_gyrospeed_filtered[2] = (flow_gyrospeed[2] + n_flow * flow_gyrospeed_filtered[2]) / (n_flow + 1);
						//对 姿态角速度 低通滤波同上；
						att_gyrospeed_filtered[0] = (att.pitchspeed + n_flow * att_gyrospeed_filtered[0]) / (n_flow + 1);
						att_gyrospeed_filtered[1] = (att.rollspeed + n_flow * att_gyrospeed_filtered[1]) / (n_flow + 1);
						att_gyrospeed_filtered[2] = (att.yawspeed + n_flow * att_gyrospeed_filtered[2]) / (n_flow + 1);
						n_flow++;
					}


					/*yaw compensation (flow sensor is not in center of rotation) -> params in QGC*/
				     //光流安装位置造成的偏差补偿；
					yaw_comp[0] = - params.flow_module_offset_y * (flow_gyrospeed[2] - gyro_offset_filtered[2]);
					yaw_comp[1] = params.flow_module_offset_x * (flow_gyrospeed[2] - gyro_offset_filtered[2]);

					/*
					 * Convert raw flow from the optical_flow uORB topic (rad) to angular flow (rad/s)
					 * Note that the optical_flow uORB topic defines positive delta angles as produced by RH rotations
					 * around the correspdonding body axes.
					 */

					float flow_ang[2];

					/* check for vehicle rates setpoint - it below threshold -> dont subtract -> better hover */
/****************************************************************************************************/
// 得到角速度设定值 	 
					//角速度设定值
					orb_check(vehicle_rate_sp_sub, &updated);

					if (updated) {
						orb_copy(ORB_ID(vehicle_rates_setpoint), vehicle_rate_sp_sub, &rates_setpoint);
					}

					float rate_threshold = 0.15f;//约8.6°

					/* calculate the angular flow rate produced by a negative velocity along the X body axis */
					  //机身转动较小时，不做补偿；
					  //俯仰角 设定值  
					if (fabsf(rates_setpoint.pitch) < rate_threshold) {
						//warnx("[inav] test ohne comp");
						//flow_k 光流缩放因子；flow_ang[0]光流像素 x轴角速度
						flow_ang[0] = (-flow.pixel_flow_y_integral / (float)flow.integration_timespan * 1000000.0f) *
							      params.flow_k;//for now the flow has to be scaled (to small)
							      
					  //机身俯仰超过阈值，使用陀螺仪补偿其转动；
					} else {
						//warnx("[inav] test mit comp");
						//calculate flow [rad/s] and compensate for rotations (and offset of flow-gyro)
						flow_ang[0] = (-(flow.pixel_flow_y_integral - flow.gyro_y_rate_integral) / (float)flow.integration_timespan * 1000000.0f
							       + gyro_offset_filtered[0]) * params.flow_k;//for now the flow has to be scaled (to small)
					}

					/* calculate the angular flow rate produced by a negative velocity along the Y body axis */
					//机身转动较小时，不做补偿；
					//滚转角 设定值  
					if (fabsf(rates_setpoint.roll) < rate_threshold) {
						//flow_k 光流缩放因子；flow_ang[0]光流像素 x轴角速度
						flow_ang[1] = (flow.pixel_flow_x_integral / (float)flow.integration_timespan * 1000000.0f) *
							      params.flow_k;//for now the flow has to be scaled (to small)

					} else {
						//calculate flow [rad/s] and compensate for rotations (and offset of flow-gyro)
						flow_ang[1] = ((flow.pixel_flow_x_integral - flow.gyro_x_rate_integral) / (float)flow.integration_timespan * 1000000.0f
							       + gyro_offset_filtered[1]) * params.flow_k;//for now the flow has to be scaled (to small)
					}

					/* flow measurements vector */
				//
					float flow_m[3];

					if (fabsf(rates_setpoint.yaw) < rate_threshold) {
						flow_m[0] = -flow_ang[0] * flow_dist;  //由角速度换算水平速度；
						flow_m[1] = -flow_ang[1] * flow_dist;

					} else {//偏航较大时，补偿其安装位置引起的偏差；
						flow_m[0] = -flow_ang[0] * flow_dist - yaw_comp[0] * params.flow_k;
						flow_m[1] = -flow_ang[1] * flow_dist - yaw_comp[1] * params.flow_k;
					}

					flow_m[2] = z_est[1];

					/* velocity in NED */
					float flow_v[2] = { 0.0f, 0.0f };

					/* project measurements vector to NED basis, skip Z component */
					//将光流在水平面的测量值 映射到导航坐标系（NED）；
					for (int i = 0; i < 2; i++) {
						for (int j = 0; j < 3; j++) {
							flow_v[i] += R(i, j) * flow_m[j];
						}
					}

					/* velocity correction */
					//光流的偏差校正量；
					corr_flow[0] = flow_v[0] - x_est[1];
					corr_flow[1] = flow_v[1] - y_est[1];
					/* adjust correction weight */
					float flow_q_weight = (flow_q - params.flow_q_min) / (1.0f - params.flow_q_min);
					w_flow = R(2, 2) * flow_q_weight / fmaxf(1.0f, flow_dist);
					 //w_flow = cos(俯仰)* cos(横滚)* flow_q_weight 权重 / 高度；

					/* if flow is not accurate, reduce weight for it */
					// TODO make this more fuzzy
					if (!flow_accurate) {//飞太快，相机跟不上；
						w_flow *= 0.05f;
					}

					/* under ideal conditions, on 1m distance assume EPH = 10cm */
					eph_flow = 0.1f / w_flow;//根据 w_flow 确定水平精度；

					flow_valid = true;

				} else {//光流条件恶劣，数据无效；
					w_flow = 0.0f;
					flow_valid = false;
				}

				flow_updates++;
			}

			/* check no vision circuit breaker is set */
			if (params.no_vision != CBRK_NO_VISION_KEY) {//启用视觉输入；
				/* vehicle vision position */
				orb_check(vision_position_estimate_sub, &updated);

				if (updated) {
					orb_copy(ORB_ID(vision_position_estimate), vision_position_estimate_sub, &vision);

					static float last_vision_x = 0.0f;
					static float last_vision_y = 0.0f;
					static float last_vision_z = 0.0f;

					/* reset position estimate on first vision update */
					if (!vision_valid) {//只执行一次；
						x_est[0] = vision.x;
						x_est[1] = vision.vx;
						y_est[0] = vision.y;
						y_est[1] = vision.vy;

						/* only reset the z estimate if the z weight parameter is not zero */
						if (params.w_z_vision_p > MIN_VALID_W) {
							z_est[0] = vision.z;
							z_est[1] = vision.vz;
						}

						vision_valid = true;

						last_vision_x = vision.x;
						last_vision_y = vision.y;
						last_vision_z = vision.z;

						warnx("VISION estimate valid");
						mavlink_log_info(&mavlink_log_pub, "[inav] VISION estimate valid");
					}

					/* calculate correction for position */
					//只执行一次；
					corr_vision[0][0] = vision.x - x_est[0];
					corr_vision[1][0] = vision.y - y_est[0];
					corr_vision[2][0] = vision.z - z_est[0];

					static hrt_abstime last_vision_time = 0;

					float vision_dt = (vision.timestamp - last_vision_time) / 1e6f;
					last_vision_time = vision.timestamp;

					if (vision_dt > 0.000001f && vision_dt < 0.2f) {
						 //时间大于0，小于0.2
						vision.vx = (vision.x - last_vision_x) / vision_dt;
						vision.vy = (vision.y - last_vision_y) / vision_dt;
						vision.vz = (vision.z - last_vision_z) / vision_dt;

						last_vision_x = vision.x;
						last_vision_y = vision.y;
						last_vision_z = vision.z;

						/* calculate correction for velocity */
						//速度差；
						corr_vision[0][1] = vision.vx - x_est[1];
						corr_vision[1][1] = vision.vy - y_est[1];
						corr_vision[2][1] = vision.vz - z_est[1];

					} else {
						/* assume zero motion */
						 //假设没有发生运动；
						corr_vision[0][1] = 0.0f - x_est[1];
						corr_vision[1][1] = 0.0f - y_est[1];
						corr_vision[2][1] = 0.0f - z_est[1];
					}

					vision_updates++;
				}
			}
/***************************************************************************************/ 
// 运动捕捉 数据 
			/* vehicle mocap position */   //动作捕捉系统， 与视觉相似；
			orb_check(att_pos_mocap_sub, &updated);

			if (updated) {
				orb_copy(ORB_ID(att_pos_mocap), att_pos_mocap_sub, &mocap);

				if (!params.disable_mocap) {
					/* reset position estimate on first mocap update */
					if (!mocap_valid) {
						x_est[0] = mocap.x;
						y_est[0] = mocap.y;
						z_est[0] = mocap.z;

						mocap_valid = true;

						warnx("MOCAP data valid");
						mavlink_log_info(&mavlink_log_pub, "[inav] MOCAP data valid");
					}

					/* calculate correction for position */
					corr_mocap[0][0] = mocap.x - x_est[0];
					corr_mocap[1][0] = mocap.y - y_est[0];
					corr_mocap[2][0] = mocap.z - z_est[0];

					mocap_updates++;
				}
			}

			/* vehicle GPS position */
			orb_check(vehicle_gps_position_sub, &updated);

			if (updated) {
				orb_copy(ORB_ID(vehicle_gps_position), vehicle_gps_position_sub, &gps);

				bool reset_est = false;

				/* hysteresis for GPS quality */
				if (gps_valid) {
					if (gps.eph > max_eph_epv || gps.epv > max_eph_epv || gps.fix_type < 3) {
						 //fix_type < 3 无法定位，或无法定位 3D 信息；
						gps_valid = false;
						mavlink_log_info(&mavlink_log_pub, "[inav] GPS signal lost");
						warnx("[inav] GPS signal lost");
					}

				} else {
					if (gps.eph < max_eph_epv * 0.7f && gps.epv < max_eph_epv * 0.7f && gps.fix_type >= 3) {
						 //信号良好；
						gps_valid = true;
						reset_est = true;
						mavlink_log_info(&mavlink_log_pub, "[inav] GPS signal found");
						warnx("[inav] GPS signal found");
					}
				}

				if (gps_valid) {//GPS分辨率，参考msg信息；
					double lat = gps.lat * 1e-7;
					double lon = gps.lon * 1e-7;
					float alt = gps.alt * 1e-3;

					/* initialize reference position if needed */
					if (!ref_inited) {
						if (ref_init_start == 0) {
							ref_init_start = t;

						} else if (t > ref_init_start + ref_init_delay) {
							ref_inited = true;

							/* set position estimate to (0, 0, 0), use GPS velocity for XY */
							x_est[0] = 0.0f;
							x_est[1] = gps.vel_n_m_s;
							y_est[0] = 0.0f;
							y_est[1] = gps.vel_e_m_s;

							local_pos.ref_lat = lat;
							local_pos.ref_lon = lon;
							local_pos.ref_alt = alt + z_est[0];//地表高度；
							local_pos.ref_timestamp = t;

							/* initialize projection */
							//转为弧度，赋值给ref；
							map_projection_init(&ref, lat, lon);
							// XXX replace this print
							warnx("init ref: lat=%.7f, lon=%.7f, alt=%8.4f", (double)lat, (double)lon, (double)alt);
							mavlink_log_info(&mavlink_log_pub, "[inav] init ref: %.7f, %.7f, %8.4f", (double)lat, (double)lon, (double)alt);
						}
					}

					if (ref_inited) {
						/* project GPS lat lon to plane */
						float gps_proj[2];
						map_projection_project(&ref, lat, lon, &(gps_proj[0]), &(gps_proj[1]));
						 //GPS的坐标转换，没看懂；
						 
						/* reset position estimate when GPS becomes good */
						if (reset_est) {//GPS 信号良好；
							x_est[0] = gps_proj[0];
							x_est[1] = gps.vel_n_m_s;
							y_est[0] = gps_proj[1];
							y_est[1] = gps.vel_e_m_s;
						}

						/* calculate index of estimated values in buffer */
						int est_i = buf_ptr - 1 - min(EST_BUF_SIZE - 1, max(0, (int)(params.delay_gps * 1000000.0f / PUB_INTERVAL)));

						if (est_i < 0) {
							est_i += EST_BUF_SIZE;
						}

						/* calculate correction for position */
						  //由 GPS 得到的位移；
						corr_gps[0][0] = gps_proj[0] - est_buf[est_i][0][0];
						corr_gps[1][0] = gps_proj[1] - est_buf[est_i][1][0];
						corr_gps[2][0] = local_pos.ref_alt - alt - est_buf[est_i][2][0];

						/* calculate correction for velocity */
						if (gps.vel_ned_valid) {
							corr_gps[0][1] = gps.vel_n_m_s - est_buf[est_i][0][1];
							corr_gps[1][1] = gps.vel_e_m_s - est_buf[est_i][1][1];
							corr_gps[2][1] = gps.vel_d_m_s - est_buf[est_i][2][1];

						} else {
							corr_gps[0][1] = 0.0f;
							corr_gps[1][1] = 0.0f;
							corr_gps[2][1] = 0.0f;
						}

						/* save rotation matrix at this moment */
						memcpy(R_gps, R_buf[est_i], sizeof(R_gps));

						w_gps_xy = min_eph_epv / fmaxf(min_eph_epv, gps.eph);
						w_gps_z = min_eph_epv / fmaxf(min_eph_epv, gps.epv);
					}

				} else {
					/* no GPS lock */
					//GPS 搜不到星；
					memset(corr_gps, 0, sizeof(corr_gps));
					ref_init_start = 0;
				}

				gps_updates++;
			}
		}

		matrix::Dcm<float> R = matrix::Quatf(att.q);

		/* check for timeout on FLOW topic 检查各个传感器数据是否超时 */
		if ((flow_valid || lidar_valid) && t > (flow_time + flow_topic_timeout)) {
			flow_valid = false;
			warnx("FLOW timeout");
			mavlink_log_info(&mavlink_log_pub, "[inav] FLOW timeout");
		}

		/* check for timeout on GPS topic */
		if (gps_valid && (t > (gps.timestamp + gps_topic_timeout))) {
			gps_valid = false;
			warnx("GPS timeout");
			mavlink_log_info(&mavlink_log_pub, "[inav] GPS timeout");
		}

		/* check for timeout on vision topic */
		if (vision_valid && (t > (vision.timestamp + vision_topic_timeout))) {
			vision_valid = false;
			warnx("VISION timeout");
			mavlink_log_info(&mavlink_log_pub, "[inav] VISION timeout");
		}

		/* check for timeout on mocap topic */
		if (mocap_valid && (t > (mocap.timestamp + mocap_topic_timeout))) {
			mocap_valid = false;
			warnx("MOCAP timeout");
			mavlink_log_info(&mavlink_log_pub, "[inav] MOCAP timeout");
		}

		/* check for lidar measurement timeout */
		if (lidar_valid && (t > (lidar_time + lidar_timeout))) {
			lidar_valid = false;
			warnx("LIDAR timeout");
			mavlink_log_info(&mavlink_log_pub, "[inav] LIDAR timeout");
		}

		float dt = t_prev > 0 ? (t - t_prev) / 1000000.0f : 0.0f;
		dt = fmaxf(fminf(0.02, dt), 0.0002);		// constrain dt from 0.2 to 20 ms
		t_prev = t;

		/* increase EPH/EPV on each step */
		//定位精度随时间漂移，直到下次校准；
		if (eph < 0.000001f) { //get case where eph is 0 -> would stay 0
			eph = 0.001;
		}

		if (eph < max_eph_epv) {
			eph *= 1.0f + dt;
		}

		if (epv < 0.000001f) { //get case where epv is 0 -> would stay 0
			epv = 0.001;
		}

		if (epv < max_eph_epv) {
			epv += 0.005f * dt;	// add 1m to EPV each 200s (baro drift)
		}

		/* use GPS if it's valid and reference position initialized */
		 //根据设定参数和数据质量，决定是否使用传感器值；
		bool use_gps_xy = ref_inited && gps_valid && params.w_xy_gps_p > MIN_VALID_W;
		bool use_gps_z = ref_inited && gps_valid && params.w_z_gps_p > MIN_VALID_W;
		/* use VISION if it's valid and has a valid weight parameter */
		bool use_vision_xy = vision_valid && params.w_xy_vision_p > MIN_VALID_W;
		bool use_vision_z = vision_valid && params.w_z_vision_p > MIN_VALID_W;
		/* use MOCAP if it's valid and has a valid weight parameter */
		bool use_mocap = mocap_valid && params.w_mocap_p > MIN_VALID_W
				 && params.att_ext_hdg_m == mocap_heading; //check if external heading is mocap

		if (params.disable_mocap) { //disable mocap if fake gps is used
			use_mocap = false;
		}

		/* use flow if it's valid and (accurate or no GPS available) */
		bool use_flow = flow_valid && (flow_accurate || !use_gps_xy);

		/* use LIDAR if it's valid and lidar altitude estimation is enabled */
		use_lidar = lidar_valid && params.enable_lidar_alt_est;

		bool can_estimate_xy = (eph < max_eph_epv) || use_gps_xy || use_flow || use_vision_xy || use_mocap;

		bool dist_bottom_valid = (t < lidar_valid_time + lidar_valid_timeout);
		//调整各个传感器的权重；
		float w_xy_gps_p = params.w_xy_gps_p * w_gps_xy;
		float w_xy_gps_v = params.w_xy_gps_v * w_gps_xy;
		float w_z_gps_p = params.w_z_gps_p * w_gps_z;
		float w_z_gps_v = params.w_z_gps_v * w_gps_z;

		float w_xy_vision_p = params.w_xy_vision_p;
		float w_xy_vision_v = params.w_xy_vision_v;
		float w_z_vision_p = params.w_z_vision_p;

		float w_mocap_p = params.w_mocap_p;

		/* reduce GPS weight if optical flow is good */
		if (use_flow && flow_accurate) {
			w_xy_gps_p *= params.w_gps_flow;
			w_xy_gps_v *= params.w_gps_flow;
		}

		/* baro offset correction */
		if (use_gps_z) {
		//此处 高度差 为低通滤波，corr_gps首先对时间积分得到高度，在乘以权重w；
			float offs_corr = corr_gps[2][0] * w_z_gps_p * dt;
			baro_offset += offs_corr;
			corr_baro += offs_corr;
		}

		/* accelerometer bias correction for GPS (use buffered rotation matrix) */
		float accel_bias_corr[3] = { 0.0f, 0.0f, 0.0f };

		if (use_gps_xy) {//二阶低通滤波，根据位置和速度偏差，得到加速度偏差，用于加速度计校正;
			accel_bias_corr[0] -= corr_gps[0][0] * w_xy_gps_p * w_xy_gps_p;
			accel_bias_corr[0] -= corr_gps[0][1] * w_xy_gps_v;
			accel_bias_corr[1] -= corr_gps[1][0] * w_xy_gps_p * w_xy_gps_p;
			accel_bias_corr[1] -= corr_gps[1][1] * w_xy_gps_v;
		}

		if (use_gps_z) {
			accel_bias_corr[2] -= corr_gps[2][0] * w_z_gps_p * w_z_gps_p;
			accel_bias_corr[2] -= corr_gps[2][1] * w_z_gps_v;
		}

		/* transform error vector from NED frame to body frame */
		for (int i = 0; i < 3; i++) {
			float c = 0.0f;

			for (int j = 0; j < 3; j++) {
				c += R_gps[j][i] * accel_bias_corr[j];
			}

			if (PX4_ISFINITE(c)) {
				acc_bias[i] += c * params.w_acc_bias * dt;
			}//accelerometer_m_s2[] -= acc_bias[];完成加速度计校准（滤波？）；
		}

		/* accelerometer bias correction for VISION (use buffered rotation matrix) */
		accel_bias_corr[0] = 0.0f;
		accel_bias_corr[1] = 0.0f;
		accel_bias_corr[2] = 0.0f;

		if (use_vision_xy) {
			accel_bias_corr[0] -= corr_vision[0][0] * w_xy_vision_p * w_xy_vision_p;
			accel_bias_corr[0] -= corr_vision[0][1] * w_xy_vision_v;
			accel_bias_corr[1] -= corr_vision[1][0] * w_xy_vision_p * w_xy_vision_p;
			accel_bias_corr[1] -= corr_vision[1][1] * w_xy_vision_v;
		}

		if (use_vision_z) {
			accel_bias_corr[2] -= corr_vision[2][0] * w_z_vision_p * w_z_vision_p;
		}

		/* accelerometer bias correction for MOCAP (use buffered rotation matrix) */
		accel_bias_corr[0] = 0.0f;
		accel_bias_corr[1] = 0.0f;
		accel_bias_corr[2] = 0.0f;

		if (use_mocap) {
			accel_bias_corr[0] -= corr_mocap[0][0] * w_mocap_p * w_mocap_p;
			accel_bias_corr[1] -= corr_mocap[1][0] * w_mocap_p * w_mocap_p;
			accel_bias_corr[2] -= corr_mocap[2][0] * w_mocap_p * w_mocap_p;
		}

		/* transform error vector from NED frame to body frame */
		for (int i = 0; i < 3; i++) {
			float c = 0.0f;

			for (int j = 0; j < 3; j++) {
				c += R(j, i) * accel_bias_corr[j];
			}

			if (PX4_ISFINITE(c)) {
				acc_bias[i] += c * params.w_acc_bias * dt;
			}
		}

		/* accelerometer bias correction for flow and baro (assume that there is no delay) */
		accel_bias_corr[0] = 0.0f;
		accel_bias_corr[1] = 0.0f;
		accel_bias_corr[2] = 0.0f;

		if (use_flow) {
			accel_bias_corr[0] -= corr_flow[0] * params.w_xy_flow;
			accel_bias_corr[1] -= corr_flow[1] * params.w_xy_flow;
		}

		if (use_lidar) {
			accel_bias_corr[2] -= corr_lidar * params.w_z_lidar * params.w_z_lidar;

		} else {
			accel_bias_corr[2] -= corr_baro * params.w_z_baro * params.w_z_baro;
		}

		/* transform error vector from NED frame to body frame */
		for (int i = 0; i < 3; i++) {
			float c = 0.0f;

			for (int j = 0; j < 3; j++) {
				c += R(j, i) * accel_bias_corr[j];
			}

			if (PX4_ISFINITE(c)) {
				acc_bias[i] += c * params.w_acc_bias * dt;
			}
		}

		/* inertial filter prediction for altitude */
		 //使用加速度值，预测速度和位移；
		inertial_filter_predict(dt, z_est, acc[2]);

		if (!(PX4_ISFINITE(z_est[0]) && PX4_ISFINITE(z_est[1]))) {
			write_debug_log("BAD ESTIMATE AFTER Z PREDICTION", dt, x_est, y_est, z_est, x_est_prev, y_est_prev, z_est_prev,
					acc, corr_gps, w_xy_gps_p, w_xy_gps_v, corr_mocap, w_mocap_p,
					corr_vision, w_xy_vision_p, w_z_vision_p, w_xy_vision_v);
			memcpy(z_est, z_est_prev, sizeof(z_est));
		}

		/* inertial filter correction for altitude */
		if (use_lidar) {
			inertial_filter_correct(corr_lidar, dt, z_est, 0, params.w_z_lidar);

		} else {
			inertial_filter_correct(corr_baro, dt, z_est, 0, params.w_z_baro);
		}

		if (use_gps_z) {
			epv = fminf(epv, gps.epv);

			inertial_filter_correct(corr_gps[2][0], dt, z_est, 0, w_z_gps_p);
			inertial_filter_correct(corr_gps[2][1], dt, z_est, 1, w_z_gps_v);
		}

		if (use_vision_z) {
			epv = fminf(epv, epv_vision);
			inertial_filter_correct(corr_vision[2][0], dt, z_est, 0, w_z_vision_p);
		}

		if (use_mocap) {
			epv = fminf(epv, epv_mocap);
			inertial_filter_correct(corr_mocap[2][0], dt, z_est, 0, w_mocap_p);
		}

		if (!(PX4_ISFINITE(z_est[0]) && PX4_ISFINITE(z_est[1]))) {
			write_debug_log("BAD ESTIMATE AFTER Z CORRECTION", dt, x_est, y_est, z_est, x_est_prev, y_est_prev, z_est_prev,
					acc, corr_gps, w_xy_gps_p, w_xy_gps_v, corr_mocap, w_mocap_p,
					corr_vision, w_xy_vision_p, w_z_vision_p, w_xy_vision_v);
			memcpy(z_est, z_est_prev, sizeof(z_est));
			memset(corr_gps, 0, sizeof(corr_gps));
			memset(corr_vision, 0, sizeof(corr_vision));
			memset(corr_mocap, 0, sizeof(corr_mocap));
			corr_baro = 0;

		} else {
			memcpy(z_est_prev, z_est, sizeof(z_est));
		}

		if (can_estimate_xy) {
			/* inertial filter prediction for position */
			inertial_filter_predict(dt, x_est, acc[0]);
			inertial_filter_predict(dt, y_est, acc[1]);

			if (!(PX4_ISFINITE(x_est[0]) && PX4_ISFINITE(x_est[1]) && PX4_ISFINITE(y_est[0]) && PX4_ISFINITE(y_est[1]))) {
				write_debug_log("BAD ESTIMATE AFTER PREDICTION", dt, x_est, y_est, z_est, x_est_prev, y_est_prev, z_est_prev,
						acc, corr_gps, w_xy_gps_p, w_xy_gps_v, corr_mocap, w_mocap_p,
						corr_vision, w_xy_vision_p, w_z_vision_p, w_xy_vision_v);
				memcpy(x_est, x_est_prev, sizeof(x_est));
				memcpy(y_est, y_est_prev, sizeof(y_est));
			}

			/* inertial filter correction for position */
			if (use_flow) {
				eph = fminf(eph, eph_flow);

				inertial_filter_correct(corr_flow[0], dt, x_est, 1, params.w_xy_flow * w_flow);
				inertial_filter_correct(corr_flow[1], dt, y_est, 1, params.w_xy_flow * w_flow);
			}

			if (use_gps_xy) {
				eph = fminf(eph, gps.eph);

				inertial_filter_correct(corr_gps[0][0], dt, x_est, 0, w_xy_gps_p);
				inertial_filter_correct(corr_gps[1][0], dt, y_est, 0, w_xy_gps_p);

				if (gps.vel_ned_valid && t < gps.timestamp + gps_topic_timeout) {
					inertial_filter_correct(corr_gps[0][1], dt, x_est, 1, w_xy_gps_v);
					inertial_filter_correct(corr_gps[1][1], dt, y_est, 1, w_xy_gps_v);
				}
			}

			if (use_vision_xy) {
				eph = fminf(eph, eph_vision);

				inertial_filter_correct(corr_vision[0][0], dt, x_est, 0, w_xy_vision_p);
				inertial_filter_correct(corr_vision[1][0], dt, y_est, 0, w_xy_vision_p);

				if (w_xy_vision_v > MIN_VALID_W) {
					inertial_filter_correct(corr_vision[0][1], dt, x_est, 1, w_xy_vision_v);
					inertial_filter_correct(corr_vision[1][1], dt, y_est, 1, w_xy_vision_v);
				}
			}

			if (use_mocap) {
				eph = fminf(eph, eph_mocap);

				inertial_filter_correct(corr_mocap[0][0], dt, x_est, 0, w_mocap_p);
				inertial_filter_correct(corr_mocap[1][0], dt, y_est, 0, w_mocap_p);
			}

			if (!(PX4_ISFINITE(x_est[0]) && PX4_ISFINITE(x_est[1]) && PX4_ISFINITE(y_est[0]) && PX4_ISFINITE(y_est[1]))) {
				write_debug_log("BAD ESTIMATE AFTER CORRECTION", dt, x_est, y_est, z_est, x_est_prev, y_est_prev, z_est_prev,
						acc, corr_gps, w_xy_gps_p, w_xy_gps_v, corr_mocap, w_mocap_p,
						corr_vision, w_xy_vision_p, w_z_vision_p, w_xy_vision_v);
				memcpy(x_est, x_est_prev, sizeof(x_est));
				memcpy(y_est, y_est_prev, sizeof(y_est));
				memset(corr_gps, 0, sizeof(corr_gps));
				memset(corr_vision, 0, sizeof(corr_vision));
				memset(corr_mocap, 0, sizeof(corr_mocap));
				memset(corr_flow, 0, sizeof(corr_flow));

			} else {
				memcpy(x_est_prev, x_est, sizeof(x_est));
				memcpy(y_est_prev, y_est, sizeof(y_est));
			}

		} else {
			/* gradually reset xy velocity estimates */
				//当 xy轴位置 无法估计时，逐渐将速度归零; 
			inertial_filter_correct(-x_est[1], dt, x_est, 1, params.w_xy_res_v);
			inertial_filter_correct(-y_est[1], dt, y_est, 1, params.w_xy_res_v);
		}

		/* run terrain estimator */
	
		  //地形估计;（对地高度）（卡尔曼滤波）
		terrain_estimator.predict(dt, &att, &sensor, &lidar);
		//根据加速度信息预测；
		terrain_estimator.measurement_update(hrt_absolute_time(), &gps, &lidar, &att);
        //根据 GPS 和 lidar 校正预测信息;
		if (inav_verbose_mode) {//打印详细信息
			/* print updates rate */
			if (t > updates_counter_start + updates_counter_len) {
				float updates_dt = (t - updates_counter_start) * 0.000001f;
				warnx(
					"updates rate: accelerometer = %.1f/s, baro = %.1f/s, gps = %.1f/s, attitude = %.1f/s, flow = %.1f/s, vision = %.1f/s, mocap = %.1f/s",
					(double)(accel_updates / updates_dt),
					(double)(baro_updates / updates_dt),
					(double)(gps_updates / updates_dt),
					(double)(attitude_updates / updates_dt),
					(double)(flow_updates / updates_dt),
					(double)(vision_updates / updates_dt),
					(double)(mocap_updates / updates_dt));
				updates_counter_start = t;
				accel_updates = 0;
				baro_updates = 0;
				gps_updates = 0;
				attitude_updates = 0;
				flow_updates = 0;
				vision_updates = 0;
				mocap_updates = 0;
			}
		}

		if (t > pub_last + PUB_INTERVAL) {
			pub_last = t;

			/* push current estimate to buffer */
			est_buf[buf_ptr][0][0] = x_est[0];
			est_buf[buf_ptr][0][1] = x_est[1];
			est_buf[buf_ptr][1][0] = y_est[0];
			est_buf[buf_ptr][1][1] = y_est[1];
			est_buf[buf_ptr][2][0] = z_est[0];
			est_buf[buf_ptr][2][1] = z_est[1];

			/* push current rotation matrix to buffer */
			memcpy(R_buf[buf_ptr], &R._data[0][0], sizeof(R._data));

			buf_ptr++;

			if (buf_ptr >= EST_BUF_SIZE) {
				buf_ptr = 0;
			}


			/* publish local position */
			
		    //将位置和速度信息置入 local_pos 准备发布；
			local_pos.xy_valid = can_estimate_xy;
			local_pos.v_xy_valid = can_estimate_xy;
			local_pos.xy_global = local_pos.xy_valid && use_gps_xy;
			local_pos.z_global = local_pos.z_valid && use_gps_z;
			local_pos.x = x_est[0];
			local_pos.vx = x_est[1];
			local_pos.y = y_est[0];
			local_pos.vy = y_est[1];
			local_pos.z = z_est[0];
			local_pos.vz = z_est[1];
			matrix::Eulerf euler(R);
			local_pos.yaw = euler.psi();
			local_pos.dist_bottom_valid = dist_bottom_valid;
			local_pos.eph = eph;
			local_pos.epv = epv;

			if (local_pos.dist_bottom_valid) {
				local_pos.dist_bottom = dist_ground;
				local_pos.dist_bottom_rate = - z_est[1];// z轴 正方向向下;
			}

			local_pos.timestamp = t;
			//发布位置信息
			orb_publish(ORB_ID(vehicle_local_position), vehicle_local_position_pub, &local_pos);

			if (local_pos.xy_global && local_pos.z_global) {
				/* publish global position */
				global_pos.timestamp = t;
				global_pos.time_utc_usec = gps.time_utc_usec;

				double est_lat, est_lon;
				map_projection_reproject(&ref, local_pos.x, local_pos.y, &est_lat, &est_lon);
				 //将 NED 坐标信息映射到 GPS 坐标;
				global_pos.lat = est_lat;
				global_pos.lon = est_lon;
				global_pos.alt = local_pos.ref_alt - local_pos.z;

				global_pos.vel_n = local_pos.vx;
				global_pos.vel_e = local_pos.vy;
				global_pos.vel_d = local_pos.vz;

				global_pos.yaw = local_pos.yaw;

				global_pos.eph = eph;
				global_pos.epv = epv;

				if (terrain_estimator.is_valid()) {
					global_pos.terrain_alt = global_pos.alt - terrain_estimator.get_distance_to_ground();
					global_pos.terrain_alt_valid = true;

				} else {
					global_pos.terrain_alt_valid = false;
				}

				global_pos.pressure_alt = sensor.baro_alt_meter;

				if (vehicle_global_position_pub == nullptr) {
					vehicle_global_position_pub = orb_advertise(ORB_ID(vehicle_global_position), &global_pos);

				} else {
					orb_publish(ORB_ID(vehicle_global_position), vehicle_global_position_pub, &global_pos);
				}
			}
		}
	}

	warnx("stopped");
	mavlink_log_info(&mavlink_log_pub, "[inav] stopped");
	thread_running = false;
	return 0;
}
