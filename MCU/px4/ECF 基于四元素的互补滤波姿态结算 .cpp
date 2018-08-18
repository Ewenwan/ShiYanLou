/****************************************************************************
 \Firmware\src\modules\attitude_estimator_q
 
 http://blog.csdn.net/luoshi006/article/details/51513580
 
 名词解释

1、陀螺仪
陀螺仪，测量角速度，具有高动态特性，它是一个间接测量角度的器件。
它测量的是角度的导数，即角速度，要将角速度对时间积分才能得到角度。
由于噪声等误差影响，在积分作用下不断积累，最终导致陀螺仪的低频干扰和漂移。


2、加速度计
输出当前加速度（包含重力加速度 g）的方向【也叫重力感应器】，在悬停时，输出为 g。
由其测量原理导致的高频信号敏感，使得加速度计在振动环境中高频干扰较大。

3、磁力计
输出为当前机体与地磁场的夹角。测量原理与指南针相似。低频特性较好，易受周围磁场干扰。 
磁力计的工作原理参考：WalkAnt的博客

4、坐标系
导航坐标系：在多旋翼中，又叫地球坐标系、地理坐标系。通常，采用北东地（NED）构成坐标系的 X,Y,Z 轴。
机体坐标系 ：固联在多旋翼飞行器上，即，坐标系原点定位于飞行器中心点（假设中心点与重心点重合）。
关于航空导航用到的坐标系，请参考 AIAA 系列丛书。在多旋翼中，因为只在低空飞行，且时间较短，只需要以上两个。


5、姿态表示
欧拉角 ：较直观，描述刚体在三维欧式空间中的姿态。此时，坐标系为机体坐标系，随着刚体的旋转而旋转。缺点：万向节锁。 
四元数：由一组四维向量，表示刚体的三维旋转。适合用于计算机计算。 
方向余弦矩阵DCM：用欧拉角余弦值或四元数，表示的坐标系的旋转。

6、mahony 滤波原理
互补滤波要求两个信号的干扰噪声处在不同的频率，通过设置两个滤波器的截止频率，确保融合后的信号能够覆盖需求频率。 
在 IMU 的姿态估计中，互补滤波器对陀螺仪（低频噪声）使用高通滤波；对加速度/磁力计（高频噪声）使用低频滤波。 
（此处尚未对传感器数据实测，噪声和有用频率未知。。。。待后期补足）

互补滤波中，两个滤波器的截止频率一致，此时就需要根据有用频率的值对其进行取舍。
机体水平时，加速度计无法测量绕 Z 轴的旋转量，即偏航角。磁力计也有同样问题，无法测得要磁轴的旋转量。
故，需要加速度计和磁力计同时对陀螺仪进行校正。


7 预测- 四元数的预测 
与卡尔曼滤波相似，mahony 滤波也分为预测-校正更新。 
在预测环节，由三轴陀螺仪测得的角速度，通过式（1）计算出四元数姿态预测。
qbe 表示从地球坐标系到机体坐标系，或机体坐标系姿态在地球坐标系下的表示。
 
8  校正
在预测环节得到的四元数 qbe(k) ，通过加速度计和磁力计的值进行校正。该环节通常分为两部分：
加速度 校正四元素中 的 横滚 roll  以及 俯仰 pitch
磁力计 校正四元素中 的 偏航 yaw  
 
    加速度 校正
        加速度计信号首先经过低通滤波器（消除高频噪声）：
           y(k)  =  RC/(T+RC)*y(k-1) + T/(T+RC)*x(k)
        然后，对得到的结果进行归一化（normalized）
           det_q_acc /= ||det_q_acc||  
        计算偏差：
		   e_acc =    det_q_acc 叉乘  v  式中， v 表示重力向量在机体坐标系的向量
		   v =[vx vy vz]转置 = [2(q1*q3-q0*q2) 2(q2*q3+q0*q1)  q0^2-q1^2-q2^2+q3^2] 转置 
	    此时，由 v 与加速度计向量垂直分量叉乘，得到误差值。【两个物理意义相同的向量，理论上叉乘为零】
		   
    磁力计 校正 
         数据预处理与加速度计相同，先滤波，然后归一化得到 det_q_mag
		 1. 无 GPS 校准时：
		    计算误差： e_mag = det_q_mag 叉乘  w
			    w 计算方法： 磁力计输出 机体坐标系 转化到 导航坐标系  乘系数 Cb-n   H = C M
    			             计算XOZ平面的投影   B   bx = sqrt(hx^2 + hy^2)   bz = hz  by = 0
							 再次变换到机体坐标系： W = Cn-b * B			 
 		2. 有 GPS 校准时：
 		    计算误差： e_mag = det_q_mag 叉乘  w
 		       w 计算方法： 在 px4 中，磁力计使用 GPS 信息 [0,0,mag] 进行校准，故，公式与加速度计相同。
 		                    W = Cn-b * [0,0,mag]
     	此时，由 w 与磁力计向量叉乘，得到误差值。【两个物理意义相同的向量，理论上叉乘为零】

9 更新四元数

由加速度计和磁力计校准得到的误差值：
    e=e_acc+e_mag
由该误差值得到修正值：
    δ = Kp * e  + Ki*sum(e)  比例积分器
    
修正后的角速度值：
ω= w_gyro+δ		 
 
 根据一阶龙格库塔方法求解一阶微分方程：
     q˙=f(q,ω)
     q(t+T)=q(t)+T?f(q,ω)	 
	 
可以求出四元数微分方程的差分形式：

q0(t+T)=q0(t)+T/2[-w_x*q1(t)-w_y*q2(t)-w_z*q3(t)]	 

 四元数规范化：
 	  q = (q0+q1*i+q2*j+q3*k)/sqrt(q0^2 + q1^2 + q2^2 + q3^2)




	 
	 	
 基于四元素的姿态估计    姿态解算这一部分很重要，主要的基础就是惯性导航和多传感器数据融合
 http://blog.csdn.net/gen_ye/article/details/52522721
 http://blog.csdn.net/gen_ye/article/details/52524190
 
http://blog.csdn.net/qq_21842557/article/details/50923863 
 
  姿态解算常用的算法有欧拉角法、方向余弦法和四元数法。 欧拉角法在求解姿态时存在奇点（万向节死锁），不能用于全姿态的解算；
   方向余弦可用于全姿态的解算但计算量大，不能满足实时性要求。 
  四元数法，其计算量小，无奇点且可以满足飞行器运动过程中姿态的实时解算。

   姿态解算的原理：对于一个确定的向量，用不同的坐标系表示时，他们所表示的大小和方向一定是相同的。
   但是由于这两个坐标系的旋转矩阵存在误差，那么当一个向量经过这么一个有误差存在的旋转矩阵后，
   在另一个坐标系中肯定和理论值是有偏差的，我们通过这个偏差来修正这个旋转矩阵。这个旋转矩阵的元素是四元数，
   我们修正的就是四元数，这样姿态就被修正了。

   陀螺仪动态响应特性良好，但计算姿态时会产生累积误差。 
   磁力计和加速度计测量姿态没有累积误差，但动态响应较差。因此他们在频域上特性互补，
   所以采用互补滤波器融合这三种传感器的数据，提高测量精度和系统的动态性能。
   
 ****************************************************************************/

/*
 * @file attitude_estimator_q_main.cpp
 *
 * Attitude estimator (quaternion based)
 *
 * @author Anton Babushkin <anton.babushkin@me.com>
 */

#include <px4_config.h>
#include <px4_posix.h>
#include <px4_tasks.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <poll.h>
#include <fcntl.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <uORB/uORB.h>       //消息发布与订阅机制 
#include <uORB/topics/sensor_combined.h>         // 传感器消息 汇总话题 
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/control_state.h>
#include <uORB/topics/vehicle_control_mode.h>
#include <uORB/topics/vehicle_global_position.h>
#include <uORB/topics/vision_position_estimate.h>
#include <uORB/topics/att_pos_mocap.h>
#include <uORB/topics/airspeed.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/estimator_status.h>
#include <drivers/drv_hrt.h>

#include <mathlib/mathlib.h>
#include <mathlib/math/filter/LowPassFilter2p.hpp>
#include <lib/geo/geo.h>

#include <systemlib/param/param.h>
#include <systemlib/perf_counter.h>
#include <systemlib/err.h>
#include <systemlib/mavlink_log.h>

//这是因为c++有重载，但是c没有，所以c代码需要作出声明，好进行链接
extern "C" __EXPORT int attitude_estimator_q_main(int argc, char *argv[]);
//此处，extern “C” 表示以 C 格式编译； __EXPORT 表示 将函数名输出到链接器（Linker）；

// using 关键字 表示引入名称到 using 说明出现的声明区域。。
using math::Vector;     //向量 
using math::Matrix;     //矩阵 
using math::Quaternion; //四元素 

class AttitudeEstimatorQ;
//定义命名空间，通过命名空间调用instance;
namespace attitude_estimator_q
{
AttitudeEstimatorQ *instance;
}


class AttitudeEstimatorQ //类定义;
{
public:
	/**
	 * Constructor
	 */
	AttitudeEstimatorQ();//构造函数，初始化参数;

	/**
	 * Destructor, also kills task.
	 */
	~AttitudeEstimatorQ();//析构函数，杀掉所有任务;

	/**
	 * Start task.
	 *
	 * @return		OK on success.
	 */
	 //开始任务，成功--返回OK；
	int		start();// 启动【attitude_estimator_q】进程，主函数入口： task_main_trampoline

	static void	task_main_trampoline(int argc, char *argv[]);
//跳转到 task_main() ，未使用传入参数；static函数只能被本文件中的函数调用；
	void		task_main();//主任务进程;

	void		print();//打印姿态信息；

private:
	static constexpr float _dt_max = 0.02;//最大时间间隔;
	 //constexpr(constant expression) 常数表达式，c11新关键字；
    //优化语法检查和编译速度；
    
	bool		_task_should_exit = false;	 //如果为 true ，任务退出；	/**< if true, task should exit */
	int		_control_task = -1;	//进程ID, 默认-1表示没有任务；	/**< task handle for task */
//订阅的话题 
	int		_sensors_sub = -1;//sensor_combined subscribe(订阅);  传感器信息 
	int		_params_sub = -1; //parameter_update subscribe;       参数信息 
	int		_vision_sub = -1; //视觉位置估计;
	int		_mocap_sub = -1;  //vicon姿态位置估计;  动作捕捉 
	int		_airspeed_sub = -1;//airspeed subscribe; 空速 
	int		_global_pos_sub = -1;//vehicle_global_position subscribe; 全球位置 
	
// 发布的话题	
	orb_advert_t	_att_pub = nullptr;//vehicle_attitude publish(发布);
	orb_advert_t	_ctrl_state_pub = nullptr;//  发布控制状态主题control_state;
	orb_advert_t	_est_state_pub = nullptr; //  发布估计状态主题 estimator_status

	struct {
		param_t	w_acc;//参数意义如下所示 
		param_t	w_mag;
		param_t	w_ext_hdg;
		param_t	w_gyro_bias;
		param_t	mag_decl;
		param_t	mag_decl_auto;
		param_t	acc_comp;
		param_t	bias_max;
		param_t	ext_hdg_mode;
		param_t airspeed_mode;
	}		_params_handles; //有用参数的句柄；	/**< handles for interesting parameters */

	float		_w_accel = 0.0f;   //加速度计权重
	float		_w_mag = 0.0f;     //磁力计权重
	float		_w_ext_hdg = 0.0f; //外部航向权重
	float		_w_gyro_bias = 0.0f;//陀螺仪偏差权
	float		_mag_decl = 0.0f;	//磁偏角（°）
	bool		_mag_decl_auto = false;// 启用基于GPS的自动磁偏角校正  不启用 
	bool		_acc_comp = false;	//启用基于GPS速度的加速度补偿	   不启用 
	float		_bias_max = 0.0f; 	//陀螺仪偏差上限
	int		_ext_hdg_mode = 0;		//外部航向模式
	int 	_airspeed_mode = 0;		//空速模式 

	Vector<3>	_gyro; //陀螺仪;
	Vector<3>	_accel;//陀螺仪;
	Vector<3>	_mag;  //陀螺仪;

	vision_position_estimate_s _vision = {};//视觉 
	Vector<3>	_vision_hdg;

	att_pos_mocap_s _mocap = {};//vicon姿态位置估计;  动作捕捉
	Vector<3>	_mocap_hdg;

	airspeed_s _airspeed = {};//空速 

	Quaternion	_q;//四元数;
	Vector<3>	_rates;//姿态角速度;
	Vector<3>	_gyro_bias;//陀螺仪偏差;

	vehicle_global_position_s _gpos = {};// 全球位置 
	Vector<3>	_vel_prev;//前一时刻的速度：
	Vector<3>	_pos_acc; //加速度（body frame??）

	/* Low pass filter for accel、gyro */   //姿态 二阶低通滤波器；
	// 陀螺仪和加速度计的 低通滤波参数  
	math::LowPassFilter2p _lp_accel_x;
	math::LowPassFilter2p _lp_accel_y;
	math::LowPassFilter2p _lp_accel_z;
	math::LowPassFilter2p _lp_gyro_x;
	math::LowPassFilter2p _lp_gyro_y;
	math::LowPassFilter2p _lp_gyro_z;
//绝对时间(ms)
	hrt_abstime _vel_prev_t = 0;//前一时刻计算速度时的绝对时间;

	bool		_inited = false;//初始化标识;
	bool		_data_good = false;//初始化标识;
	bool		_ext_hdg_good = false;//外部航向可用;

	orb_advert_t	_mavlink_log_pub = nullptr;//mavlink log advert;

	void update_parameters(bool force);//更新参数; 
//true: 获取新参数, 并由磁偏角更新四元数；  false: 查看参数是否更新；

	int update_subscriptions();//未使用【待查？？】

	bool init();// 由加速度计和磁力计初始化旋转矩阵，有GPS时，校正磁偏角。

	bool update(float dt);//调用init(); 互补滤波

	// Update magnetic declination (in rads) immediately changing yaw rotation
	void update_mag_declination(float new_declination);//使用磁偏角更新四元数
};

//构造函数，初始化参数;
AttitudeEstimatorQ::AttitudeEstimatorQ() :
	_vel_prev(0, 0, 0),//前一时刻的速度：
	_pos_acc(0, 0, 0), //加速度（body frame??）
	_lp_accel_x(250.0f, 30.0f),// 陀螺仪和加速度计的 低通滤波参数 
	_lp_accel_y(250.0f, 30.0f), //低通滤波（采样频率,截止频率）;
	_lp_accel_z(250.0f, 30.0f),
	_lp_gyro_x(250.0f, 30.0f),
	_lp_gyro_y(250.0f, 30.0f),
	_lp_gyro_z(250.0f, 30.0f)
{  ////磁力计超时;??? 
	_params_handles.w_acc		= param_find("ATT_W_ACC");   //这些就是之前提到的匹配系统参数
	_params_handles.w_mag		= param_find("ATT_W_MAG");
	_params_handles.w_ext_hdg	= param_find("ATT_W_EXT_HDG");
	_params_handles.w_gyro_bias	= param_find("ATT_W_GYRO_BIAS");
	_params_handles.mag_decl	= param_find("ATT_MAG_DECL");
	_params_handles.mag_decl_auto	= param_find("ATT_MAG_DECL_A");
	_params_handles.acc_comp	= param_find("ATT_ACC_COMP");
	_params_handles.bias_max	= param_find("ATT_BIAS_MAX");
	_params_handles.ext_hdg_mode	= param_find("ATT_EXT_HDG_M");
	_params_handles.airspeed_mode = param_find("FW_ARSP_MODE");
}

/**
 * Destructor, also kills task.  析构函数，杀掉所有任务;
 */
AttitudeEstimatorQ::~AttitudeEstimatorQ()
{
	if (_control_task != -1) {
		/* task wakes up every 100ms or so at the longest */
		_task_should_exit = true;

		/* wait for a second for the task to quit at our request */
		unsigned i = 0;

		do {
			/* wait 20ms */
			usleep(20000);

			/* if we have given up, kill it */
			if (++i > 50) {
				px4_task_delete(_control_task);
				break;
			}
		} while (_control_task != -1);
	}

	attitude_estimator_q::instance = nullptr;
}

int AttitudeEstimatorQ::start()//启动【attitude_estimator_q】进程，主函数入口： task_main_trampoline
{
	ASSERT(_control_task == -1);

	/* start the task 开启任务  //启动任务，返回进程ID; */
	_control_task = px4_task_spawn_cmd("attitude_estimator_q",//创建任务  任务名 
					   SCHED_DEFAULT,//默认调度
					   SCHED_PRIORITY_MAX - 5,//优先级 
					   2100, //栈大小  修改  2000  2500
					   (px4_main_t)&AttitudeEstimatorQ::task_main_trampoline,//线程入口函数 
					   nullptr);//主函数无参数 

	if (_control_task < 0) {
		warn("task start failed");
		return -errno;
	}

	return OK;
}

//打印姿态信息；
void AttitudeEstimatorQ::print()
{
    warnx("gyro status:");  // 添加  
	_voter_gyro.print();
	warnx("accel status:");
	_voter_accel.print();
	warnx("mag status:");
	_voter_mag.print();
}

void AttitudeEstimatorQ::task_main_trampoline(int argc, char *argv[])
{
	attitude_estimator_q::instance->task_main();
}


/*线程主函数入口*/ 
void AttitudeEstimatorQ::task_main()
{

#ifdef __PX4_POSIX
//记录事件执行所花费的时间，performance counters;
	perf_counter_t _perf_accel(perf_alloc_once(PC_ELAPSED, "sim_accel_delay"));
	perf_counter_t _perf_mpu(perf_alloc_once(PC_ELAPSED, "sim_mpu_delay"));
	perf_counter_t _perf_mag(perf_alloc_once(PC_ELAPSED, "sim_mag_delay"));
#endif

	_sensors_sub = orb_subscribe(ORB_ID(sensor_combined));        // 订阅传感器消息话题 
     //订阅传感器读数，包含数据参见：Firmware/msg/sensor_combined.msg
	_vision_sub = orb_subscribe(ORB_ID(vision_position_estimate));// 订阅视觉 位置估计话题 
	_mocap_sub = orb_subscribe(ORB_ID(att_pos_mocap));            // vicon  姿态位置运动 捕捉话题 

	_airspeed_sub = orb_subscribe(ORB_ID(airspeed));              // 空速话题 

	_params_sub = orb_subscribe(ORB_ID(parameter_update));        // 参数更新话题  //bool saved;
	_global_pos_sub = orb_subscribe(ORB_ID(vehicle_global_position));//飞机全局位置话题  位置估计值(GPS);

	update_parameters(true);//参数自己跟踪进去看看  //获取新参数;

	hrt_abstime last_time = 0;

	px4_pollfd_struct_t fds[1] = {};
	fds[0].fd = _sensors_sub;    //文件描述符; 阻塞等待   sensor_combined 话题消息 
	fds[0].events = POLLIN;      //读取事件标识;

	while (!_task_should_exit) {
		 /*poll函数调用阻塞的时间为1s*/  //timeout = 1000; fds_size = 1; 详见Linux的poll函数;
		int ret = px4_poll(fds, 1, 1000);// 监控文件描述符  struct pollfd结构类型的数组  struct pollfd结构类型的数组  是poll函数调用阻塞的时间，单位：毫秒
		// 返回值：
    //>0：数组fds中准备好读、写或出错状态的那些socket描述符的总数量；
    //==0:poll()函数会阻塞timeout所指定的毫秒时间长度之后返回;
    //-1:poll函数调用失败；同时会自动设置全局变量errno；

		if (ret < 0) {
			// Poll error, sleep and try again
			usleep(10000);
			PX4_WARN("Q POLL ERROR");
			continue;

		} else if (ret == 0) {
			// Poll timeout, do nothing
			PX4_WARN("Q POLL TIMEOUT");
			continue;
		}

		update_parameters(false);//检查orb是否更新;

		// Update sensors
		sensor_combined_s sensors;//定义订阅话题 sensor_combined 对应的消息类型 的 变量  sensors

		if (!orb_copy(ORB_ID(sensor_combined), _sensors_sub, &sensors)) {//将话题上收到的数据拷贝到  sensors变量内存中 
			// Feed validator with recent sensor data

// 低通滤波处理 
			if (sensors.timestamp > 0) {// 陀螺仪数据时间戳 为 消息本身的 时间戳  sensors.timestamp
				// 陀螺仪数据滤波处理 Filter gyro signal since it is not fildered in the drivers.
				_gyro(0) = _lp_gyro_x.apply(sensors.gyro_rad[0]);  // //把陀螺仪的值先积分然后再微分，这个其实就是求平均  这样更稳更可靠
				_gyro(1) = _lp_gyro_y.apply(sensors.gyro_rad[1]);
				_gyro(2) = _lp_gyro_z.apply(sensors.gyro_rad[2]);
			}
             // 加速度数据的时间戳 为  accelerometer_timestamp_relative + timestamp =  Accelerometer timestamp
			if (sensors.accelerometer_timestamp_relative != sensor_combined_s::RELATIVE_TIMESTAMP_INVALID) {
				// 加速度数据滤波处理  Filter accel signal since it is not fildered in the drivers.
				_accel(0) = _lp_accel_x.apply(sensors.accelerometer_m_s2[0]);
				_accel(1) = _lp_accel_y.apply(sensors.accelerometer_m_s2[1]);
				_accel(2) = _lp_accel_z.apply(sensors.accelerometer_m_s2[2]);

				if (_accel.length() < 0.01f) { //退化，即非满秩，此处为奇异值（0）;
					PX4_DEBUG("WARNING: degenerate accel!"); //degenerate 恶化 变质 就是数据有问题 可能硬件有问题
					continue;
				}
			}
            // 磁力计消息 
			if (sensors.magnetometer_timestamp_relative != sensor_combined_s::RELATIVE_TIMESTAMP_INVALID) {
				_mag(0) = sensors.magnetometer_ga[0];//未预处理 
				_mag(1) = sensors.magnetometer_ga[1];
				_mag(2) = sensors.magnetometer_ga[2];

				if (_mag.length() < 0.01f) { //退化，即非满秩，此处为奇异值（0）;
					PX4_DEBUG("WARNING: degenerate mag!");//degenerate 恶化 变质 就是数据有问题 可能硬件有问题
					continue;
				}
			}

			_data_good = true;//数据可用;
		}

////////////////////// 更新视觉 数据     检查  Update vision and motion capture heading
		bool vision_updated = false;
		orb_check(_vision_sub, &vision_updated);
        // 更新 动捕获数据   检查 
		bool mocap_updated = false;
		orb_check(_mocap_sub, &mocap_updated);

        // 更新视觉 数据
        //如果视觉更新  拷出视觉信息 ，视觉也是一个结构体里面有很多信息 包括想x,y,x,vx,vy,vz,q[4] 就是根据视觉也可以获取位置信息
		if (vision_updated) {
			orb_copy(ORB_ID(vision_position_estimate), _vision_sub, &_vision);//复制话题消息到本地内存 
			math::Quaternion q(_vision.q);//四元素 
                                                    // R-vision 基于视觉 得到的转换矩阵
			math::Matrix<3, 3> Rvis = q.to_dcm();// 四元素Quaternion 变换到 方向余弦矩阵(Direction Cosine Matrix)也叫 旋转矩阵(Rotation Matrix) 
			math::Vector<3> v(1.0f, 0.0f, 0.4f);
			
           //  http://www.07net01.com/2016/04/1472117.html 刚体在三维空间的旋转
		    
			// Rvis is Rwr (robot respect to world) while v is respect to world.
			// Hence Rvis must be transposed having (Rwr)' * Vw
			// Rrw * Vw = vn. This way we have consistency
			 //通过视觉得到的姿态估计q->Rvis，将v转换到机体坐标系;
			_vision_hdg = Rvis.transposed() * v;//transposed转置 因为R是标准正交阵 转置=逆  原来由地理->机体，转置后就是 机体->地理
			 //猜测应该类似与重力的处理 *【0 0 1】，无关怎么转换 最后得到的hdg应该就是用此种传感器校准的误差
		}
		
        // 更新 动捕获数据   Update  motion capture heading航向
        
		if (mocap_updated) {
			orb_copy(ORB_ID(att_pos_mocap), _mocap_sub, &_mocap);//复制话题消息到本地内存
			math::Quaternion q(_mocap.q);          //R-mocap 基于动作捕捉 得到的转换矩阵
			math::Matrix<3, 3> Rmoc = q.to_dcm();// 四元素Quaternion 变换到 方向余弦矩阵(Direction Cosine Matrix)也叫 旋转矩阵(Rotation Matrix) 

			math::Vector<3> v(1.0f, 0.0f, 0.4f);

			// Rmoc is Rwr (robot respect to world) while v is respect to world.
			// Hence Rmoc must be transposed having (Rwr)' * Vw
			// Rrw * Vw = vn. This way we have consistency
			_mocap_hdg = Rmoc.transposed() * v;  // Hdg就是heading
		}

		// 更新空速信息  Update airspeed
		bool airspeed_updated = false;
		orb_check(_airspeed_sub, &airspeed_updated);

		if (airspeed_updated) {
			orb_copy(ORB_ID(airspeed), _airspeed_sub, &_airspeed);
		}

		// Check for timeouts on data
		if (_ext_hdg_mode == 1) {
			_ext_hdg_good = _vision.timestamp > 0 && (hrt_elapsed_time(&_vision.timestamp) < 500000);

		} else if (_ext_hdg_mode == 2) {
			_ext_hdg_good = _mocap.timestamp > 0 && (hrt_elapsed_time(&_mocap.timestamp) < 500000);
		}

//  //飞机全局位置数据 
/********GPS位置估计值校正磁偏角**********/ 
		bool gpos_updated;//global position 全球位置
		orb_check(_global_pos_sub, &gpos_updated);

		if (gpos_updated) { //global position 全球位置
			orb_copy(ORB_ID(vehicle_global_position), _global_pos_sub, &_gpos);

			if (_mag_decl_auto && _gpos.eph < 20.0f && hrt_elapsed_time(&_gpos.timestamp) < 1000000) {
				/* set magnetic declination automatically  自动获取磁偏角 因为不同重力 不同位置磁偏角不同 根据经度和维度 自动获取磁偏角 */
				update_mag_declination(math::radians(get_mag_declination(_gpos.lat, _gpos.lon))); //latitude纬度 longitude经度
			}
			 //磁偏自动校正，且水平偏差的标准差小于20，根据位置估计值(GPS)（vehicle_global_position）校正磁偏角;
            //get_mag_declination()函数查表得到地磁偏角，进行补偿。
		}
		
/********GPS速度估计值计算加速度值**********/ 
		if (_acc_comp && _gpos.timestamp != 0 && hrt_absolute_time() < _gpos.timestamp + 20000 && _gpos.eph < 5.0f && _inited) {
		
			/* 实际的位置数据 在 北 东 地 坐标系下 position data is actual */
		   //基于GPS的位置速度 信息，微分得到加速度值;
			if (gpos_updated) {
				Vector<3> vel(_gpos.vel_n, _gpos.vel_e, _gpos.vel_d);// 在 北 东 地 坐标系下 的速度 下面根据它求加速度

				/* velocity updated */
				if (_vel_prev_t != 0 && _gpos.timestamp != _vel_prev_t) {//速度更新了 
					float vel_dt = (_gpos.timestamp - _vel_prev_t) / 1000000.0f;//时间间隔，单位（s）
					
					/* 计算载体坐标系上的加速度 calculate acceleration in body frame */
					_pos_acc = _q.conjugate_inversed((vel - _vel_prev) / vel_dt);
					//由ned坐标系下的速度求出机体坐标系下的加速度;
				}

				_vel_prev_t = _gpos.timestamp;//更新时间 
				_vel_prev = vel;              //更新速度保留值 
			}

		} else {
			/* position data is outdated, reset acceleration */
			_pos_acc.zero();
			_vel_prev.zero();
			_vel_prev_t = 0;
		}

		/* time from previous iteration */
		hrt_abstime now = hrt_absolute_time();
		//时间间隔，单位（s）
		float dt = (last_time > 0) ? ((now  - last_time) / 1000000.0f) : 0.00001f;//用极小值0.00001表示零，预防除零错误;
		last_time = now;
		
        // 时间间隔 限幅 
		if (dt > _dt_max) {
			dt = _dt_max;//时间间隔上限;
		}

/*！！ update 就是姿态更新的函数，先利用视觉 mcap 加速度 磁力计 修正陀螺仪，

再利用四元数的微分方程 实时更新解算姿态信息， 此函数后就是得到更新后的姿态信息了  */

		if (!update(dt)) { //调用update(dt)，**互补滤波**，更新四元数;
			continue;
		}

		Vector<3> euler = _q.to_euler();
		//用更新的四元数（_q）求出欧拉角，以便在控制过程中实现完美的控制，控制还是需要用直接明了的欧拉角。

		struct vehicle_attitude_s att = {};// 飞机姿态 欧拉角    新
		// 注意 新 版本vehicle_attitude_s消息类型 仅包含  float32 rollspeed  float32 pitchspeed float32 yawspeed  float32[4] q
		att.timestamp = sensors.timestamp;

//##########################################################  wyw添加 
	//	att.roll = euler(0); //获取的欧拉角赋值给roll、pitch、yaw 角度 
	//	att.pitch = euler(1);
	//	att.yaw = euler(2);
//##################################################################

		att.rollspeed = _rates(0);  //获取roll、pitch、yaw得旋转速度
		att.pitchspeed = _rates(1);
		att.yawspeed = _rates(2);

//################################################################## wyw添加 
	//	for (int i = 0; i < 3; i++) {
	//		att.g_comp[i] = _accel(i) - _pos_acc(i);////获取导航坐标系的重力加速度，前面介绍过  加速度测量值-运动加速度= 重力加速度
	//	}
//##########################################################  

    //下面就是对更新后的姿态信息 进行重新写出系统 方便下一次的使用
		memcpy(&att.q[0], _q.data, sizeof(att.q));//初始化为 0  为发布话题数做准备  
		/* the instance count is not used here */
		int att_inst;
		orb_publish_auto(ORB_ID(vehicle_attitude), &_att_pub, &att, &att_inst, ORB_PRIO_HIGH);
		 //广播姿态信息;
		{ //使用当前姿态，更新control_state，并发布;
			struct control_state_s ctrl_state = {};  // 控制状态话题  消息类型变量 

			ctrl_state.timestamp = sensors.timestamp;

			/* attitude quaternions for control state */
			ctrl_state.q[0] = _q(0); // 控制 状态  四元素 
			ctrl_state.q[1] = _q(1);
			ctrl_state.q[2] = _q(2);
			ctrl_state.q[3] = _q(3);

			ctrl_state.x_acc = _accel(0); //加速度 
			ctrl_state.y_acc = _accel(1);
			ctrl_state.z_acc = _accel(2);

			/* attitude rates for control state */
			ctrl_state.roll_rate = _rates(0); // 角速度 
			ctrl_state.pitch_rate = _rates(1);
			ctrl_state.yaw_rate = _rates(2);

			/* TODO get bias estimates from estimator */
			ctrl_state.roll_rate_bias = 0.0f;   //补偿偏置  
			ctrl_state.pitch_rate_bias = 0.0f;
			ctrl_state.yaw_rate_bias = 0.0f;

			ctrl_state.airspeed_valid = false;

			if (_airspeed_mode == control_state_s::AIRSPD_MODE_MEAS) {
				// use measured airspeed
				if (PX4_ISFINITE(_airspeed.indicated_airspeed_m_s) && hrt_absolute_time() - _airspeed.timestamp < 1e6
				    && _airspeed.timestamp > 0) {
					ctrl_state.airspeed = _airspeed.indicated_airspeed_m_s;
					ctrl_state.airspeed_valid = true;
				}
			}

			else if (_airspeed_mode == control_state_s::AIRSPD_MODE_EST) {
				// use estimated body velocity as airspeed estimate
				if (hrt_absolute_time() - _gpos.timestamp < 1e6) {
					ctrl_state.airspeed = sqrtf(_gpos.vel_n * _gpos.vel_n + _gpos.vel_e * _gpos.vel_e + _gpos.vel_d * _gpos.vel_d);
					ctrl_state.airspeed_valid = true;
				}

			} else if (_airspeed_mode == control_state_s::AIRSPD_MODE_DISABLED) {
				// do nothing, airspeed has been declared as non-valid above, controllers
				// will handle this assuming always trim airspeed
			}

			/* the instance count is not used here */
			int ctrl_inst;
			
			/* publish to control state topic */
			orb_publish_auto(ORB_ID(control_state), &_ctrl_state_pub, &ctrl_state, &ctrl_inst, ORB_PRIO_HIGH);
		}

		{
			//struct estimator_status_s est = {};

			//est.timestamp = sensors.timestamp;

			/* the instance count is not used here */
			//int est_inst;
			/* publish to control state topic */
			// TODO handle attitude states in position estimators instead so we can publish all data at once
			// or we need to enable more thatn just one estimator_status topic
			// orb_publish_auto(ORB_ID(estimator_status), &_est_state_pub, &est, &est_inst, ORB_PRIO_HIGH);
		}
	}

#ifdef __PX4_POSIX
	perf_end(_perf_accel);
	perf_end(_perf_mpu);
	perf_end(_perf_mag);
#endif

	orb_unsubscribe(_sensors_sub);
	orb_unsubscribe(_vision_sub);
	orb_unsubscribe(_mocap_sub);
	orb_unsubscribe(_airspeed_sub);
	orb_unsubscribe(_params_sub);
	orb_unsubscribe(_global_pos_sub);
}

void AttitudeEstimatorQ::update_parameters(bool force)
{
	bool updated = force;

	if (!updated) {
		orb_check(_params_sub, &updated);
	}

	if (updated) {
		parameter_update_s param_update;
		orb_copy(ORB_ID(parameter_update), _params_sub, &param_update);

		param_get(_params_handles.w_acc, &_w_accel);
		param_get(_params_handles.w_mag, &_w_mag);
		param_get(_params_handles.w_ext_hdg, &_w_ext_hdg);
		param_get(_params_handles.w_gyro_bias, &_w_gyro_bias);
		float mag_decl_deg = 0.0f;
		param_get(_params_handles.mag_decl, &mag_decl_deg);
		update_mag_declination(math::radians(mag_decl_deg));
		int32_t mag_decl_auto_int;
		param_get(_params_handles.mag_decl_auto, &mag_decl_auto_int);
		_mag_decl_auto = mag_decl_auto_int != 0;
		int32_t acc_comp_int;
		param_get(_params_handles.acc_comp, &acc_comp_int);
		_acc_comp = acc_comp_int != 0;
		param_get(_params_handles.bias_max, &_bias_max);
		param_get(_params_handles.ext_hdg_mode, &_ext_hdg_mode);
		param_get(_params_handles.airspeed_mode, &_airspeed_mode);
	}
}
//构建一个初始的四元数，用四元数来表示姿态时 通过微分方程实时更新解算姿态 但是这种求解也是需要一个初值啊，
//至于是行是列不重要，这里加速度的测量值作为第三行 说明这个四元数是由 载体转换到地理，如果是第三列还记得之前的[0 0 1]g
//得到的第三列吗，这里行不就是转置吗
//i= （_mag - k * (_mag * k))之所以这么复杂 只是在强制使k i叉乘为零，相互垂直。/
// k= -_accel 然后归一化k，k为加速度传感器测量到加速度方向向量，由于第一次测量数据时无人机一般为平稳状态无运动状态，
//所以可以直接将测到的加速度视为重力加速度g，以此作为dcm旋转矩阵的第三行k（这个介绍过了）。
// i= （_mag - k * (_mag * k)) _mag向量指向正北方，k*(_mag*k) 正交correction值，对于最终的四元数归一化以后的范数可以在正负5%以内；
//感觉不如《DCM IMU:Theory》中提出的理论“强制正交化”修正的好，Renormalization算法在ardupilot的上层应用AP_AHRS_DCM中使用到了。
// j= k%i ：外积、叉乘。关于上面的Vector<3>k = -_accel，Vector<3>相当于一个类型（int）定义一个变量k，然后把-_accel负值给k，
//在定义_accel时也是使用Vector<3>，属于同一种类型的，主要就是为了考虑其实例化过程（类似函数重载）


// 互补滤波得到  角速度 roll  pitch yaw 
// 由加速度计和磁力计初始化旋转矩阵和四元数; 
bool AttitudeEstimatorQ::init()
{
	// Rotation matrix can be easily constructed from acceleration and mag field vectors
	// 'k' is Earth Z axis (Down) unit vector in body frame
	// k 为导航坐标系（NED）的 z 轴（D）在机体坐标系中的表示;  导航系中，D正方向朝下;
	Vector<3> k = -_accel;
	k.normalize();

	// 'i' is Earth X axis (North) unit vector in body frame, orthogonal with 'k'
	// i 为导航坐标系（NED）的 x 轴（N）在机体坐标系; 
	Vector<3> i = (_mag - k * (_mag * k));//施密特正交化;
	i.normalize();

	// 'j' is Earth Y axis (East) unit vector in body frame, orthogonal with 'k' and 'i'
	Vector<3> j = k % i;//j 为导航坐标系（NED）的 y 轴（E）在机体坐标系; 

	// Fill rotation matrix  // 构造旋转矩阵 R 
	Matrix<3, 3> R;
	R.set_row(0, i);
	R.set_row(1, j);
	R.set_row(2, k);

	// 旋转矩阵 到四元素  Convert to quaternion
	_q.from_dcm(R);

	// Compensate for magnetic declination  // 转换为四元数 q ，根据设定校正磁偏，归一化;
	Quaternion decl_rotation;
	decl_rotation.from_yaw(_mag_decl);
	_q = decl_rotation * _q;

	_q.normalize();

	if (PX4_ISFINITE(_q(0)) && PX4_ISFINITE(_q(1)) &&
	    PX4_ISFINITE(_q(2)) && PX4_ISFINITE(_q(3)) &&
	    _q.length() > 0.95f && _q.length() < 1.05f) {
		_inited = true;

	} else {
		_inited = false;
	}

	return _inited;
}

// 由加速度计和磁力计初始化旋转矩阵，有GPS时，校正磁偏角。  互补滤波 
bool AttitudeEstimatorQ::update(float dt) //用于对四元数向量_q进行初始化赋值 或者 更新。
{
	if (!_inited) {// 首先判断是否是第一次进入该函数，第一次进入该函数先进入init函数初始化

		if (!_data_good) {
			return false;
		}

		return init();  //执行一次;
	}

//如果不是第一次进入该函数，则判断是使用什么mode做修正的，比如vision、mocap、acc和mag
//（DJI精灵4用的视觉壁障应该就是这个vision），Hdg就是heading。


    //保存上一状态，以便恢复;
	Quaternion q_last = _q; //这里_q就是init()初始化得到的那个 是由 载体->地理坐标系

	// Angular rate of correction  校准
	//corr包含磁力计修正、加速度计修正、（vision、mocap修正）、gyro中测量到的角速度偏转量，
    //且因为corr为update函数中定义的变量，所以每次进入update函数中时会刷新corr变量的数据； _rate也会刷新其中的数据，含义为三个姿态角的角速度（修正后）；
    //_q为外部定义的变量，在这个函数中只会+=不会重新赋值，如果计算出现错误会返回上一次计算出的_q。
    
	Vector<3> corr;//初始化元素为0;  校准参数 
	float spinRate = _gyro.length();

	if (_ext_hdg_mode > 0 && _ext_hdg_good) {//_ext_hdg_good表示外部航向数据可用;
		
		 //  _ext_hdg_mode== 1、2时都是利用vision数据和mocap数据对gyro数据进行修正，下面的global frame就是所谓的earthframe。
		
		if (_ext_hdg_mode == 1) {
			// Vision heading correction
			//视觉航向校正;
			// Project heading to global frame and extract XY component
			//将航向投影到导航坐标系，提取XY分量;
			Vector<3> vision_hdg_earth = _q.conjugate(_vision_hdg); 
			//_q.conjugate先b系到n系，后conjugate_inversedn系到b系，再补回到b系
			float vision_hdg_err = _wrap_pi(atan2f(vision_hdg_earth(1), vision_hdg_earth(0)));
			// Project correction to body frame
			corr += _q.conjugate_inversed(Vector<3>(0.0f, 0.0f, -vision_hdg_err)) * _w_ext_hdg;
		}

		if (_ext_hdg_mode == 2) {
			// Mocap heading correction
			// 运动捕捉  校正 
			// Project heading to global frame and extract XY component
			Vector<3> mocap_hdg_earth = _q.conjugate(_mocap_hdg);
			float mocap_hdg_err = _wrap_pi(atan2f(mocap_hdg_earth(1), mocap_hdg_earth(0)));
			// Project correction to body frame
			corr += _q.conjugate_inversed(Vector<3>(0.0f, 0.0f, -mocap_hdg_err)) * _w_ext_hdg;
			 //计算corr值等于单位化的旋转矩阵R（b系转n系）的转置（可以理解为 R（n系转b系））乘以（0,0，-mag_err），
			 //相当于机体坐标系绕地理坐标系N轴（Z轴）转动arctan（mag_earth(1), mag_earth(0)）度。
		}
	}

	if (_ext_hdg_mode == 0  || !_ext_hdg_good) {
		
		//_ext_hdg_mode== 0利用磁力计修正。 Magnetometer磁力计 correction
		
		// Magnetometer correction
		// Project mag field vector to global frame and extract XY component
		// 不使用外部航向，或外部航向异常时，采用磁力计校准; 
		Vector<3> mag_earth = _q.conjugate(_mag); //b系到n系  将磁力计读数从机体坐标系转换到导航坐标系;  Rn-b * _mag 
		float mag_err = _wrap_pi(atan2f(mag_earth(1), mag_earth(0)) - _mag_decl);
		//将磁力计的读数由向量换算到角度; 该角度减去磁偏，得到磁场偏差; 
		//_mag_decl 由GPS得到; 
		//**atan2f: 2 表示两个输入参数; 支持四象限内角度换算; 输出弧度值; 
		//**_wrap_pi: 将(0~2pi)的角度映射到(-pi~pi);
		// 只考虑Vector<3> mag_earth中的前两维的数据mag_earth(1)和mag_earth(0)（即x、y，忽略z轴上的偏移），
		// 通过arctan得到的角度和前面根据经纬度获取的磁偏角做差值得到纠偏误差角度mag_err ，_wrap_pi函数是用于限定结果-pi到pi的函数
		float gainMult = 1.0f;
		const float fifty_dps = 0.873f;

		if (spinRate > fifty_dps) {
			gainMult = fmin(spinRate / fifty_dps, 10.0f);
		}

		// Project magnetometer correction to body frame
		// 将磁场偏差转换到机体坐标系，并乘以其对应权重  mag_err  GPS 测得; 
		corr += _q.conjugate_inversed(Vector<3>(0.0f, 0.0f, -mag_err)) * _w_mag * gainMult;    ////n系到b系
	}

	_q.normalize();//四元数归一化;   这里的归一化用于校正update_mag_declination后的偏差。


	// // 加速度修正，上面都是用来修正航向的 z轴，但是还有x y靠谁修正， Accelerometer correction
	// Project 'k' unit vector of earth frame to body frame
	// Vector<3> k = _q.conjugate_inversed(Vector<3>(0.0f, 0.0f, 1.0f));
	// Optimized version with dropped zeros
	// 把归一化的n系重力加速度通过旋转矩阵R左乘旋转到b系  Vector<3> k = _q.conjugate_inversed(Vector<3>(0.0f, 0.0f, 1.0f)); R(n->b) 乘以（0,0，1）g
	Vector<3> k(
		2.0f * (_q(1) * _q(3) - _q(0) * _q(2)),
		2.0f * (_q(2) * _q(3) + _q(0) * _q(1)),
		(_q(0) * _q(0) - _q(1) * _q(1) - _q(2) * _q(2) + _q(3) * _q(3))
	);

	corr += (k % (_accel - _pos_acc).normalized()) * _w_accel;//w开头都是权重
	//向量 k 是重力加速度向量(0,0,1)转换到机体坐标系; 
	//_accel 是加速度计的读数; 
	//_pos_acc 是由位置估计(GPS) 微分得到的加速度; 
	//_accel - _pos_acc 表示飞行器加速度向量减去其水平分量; 
	//k 与 (_accel - _pos_acc)叉乘，得到偏差;    e=k×[机体测量重力加速度]
	 
//这就是mahony算法中的的计算过程，只是换了个包装 {k%（_accel“加速度计的测量值”-位移加速度）的单位化）<约等于重力加速度g>}*权重。 
//这里考虑了运动加速度，减去它更可靠，之前都忽略了。（加速度计测量的是 重力加速度+运动加速度）
//总加速度（加速度获取）减去机体运动加速度（第五部分）获取重力加速度，然后姿态矩阵的不是行就是列来与纯重力加速度来做叉积，算出误差。
//因为运动加速度是有害的干扰，必须减掉。算法的理论基础是[0,0,1]与姿态矩阵相乘。该差值获取的重力加速度的方向是导航坐标系下的z轴，
//加上运动加速度之后，总加速度的方向就不是与导航坐标系的天或地平行了，所以要消除这个误差，即“_accel-_pos_acc”。
//然后叉乘z轴向量得到误差，进行校准



	// // 陀螺仪误差估计 Gyro bias estimation 
	if (spinRate < 0.175f) {
		_gyro_bias += corr * (_w_gyro_bias * dt);  // gyro_bias+=[Mag*wmag+Acc*wacc]*w_gyro*dt
		//PI控制器中的I（积分）效果。然后对_gyro_bias做约束处理。 _w开头都是weigh都是权重 这里类似与KI的效果
		
		// 得到陀螺仪修正值 
		// 此处修正值为 mahony 滤波的 pi 控制器的 积分部分; 
		// 因为 _gyro_bias 不归零，故不断累积; 

		for (int i = 0; i < 3; i++) {
			_gyro_bias(i) = math::constrain(_gyro_bias(i), -_bias_max, _bias_max);
		}

	}

	_rates = _gyro + _gyro_bias;//角速度 = 陀螺仪的测量值 + 误差校准

	// Feed forward gyro
	corr += _rates; // 前馈 Feed forward gyro  个人认为这里的 corr 才是更新后的角速度;

	// Apply correction to state
	//最后就是使用修正的数据更新四元数，并把_rates和_gyro_bias置零便于下次调用时使用。
    // 对状态进行修正，实际上 就是关键的姿态更新 Apply correction to state
	_q += _q.derivative(corr) * dt;//更新四元数，derivative为求导函数，使用一阶龙格库塔求导。 
	
    // 非常重要，又用到了微分方程离散化的思想。以前讲过DCM矩阵更新过程中也是用到了该思想。
     // 先看看代码，有点怪，怪就怪在derivative（衍生物）这个名字上，平时一大推的论文和期刊上面都是用的omga *Q 的形式，
   //而这里的代码实现确是用的Q * omga的形式，所以构造的4*4矩阵的每一列的符号就不一样了。
    //http://blog.csdn.NET/qq_21842557/article/details/51058206最后


	// Normalize quaternion  正则化 
	_q.normalize();

	if (!(PX4_ISFINITE(_q(0)) && PX4_ISFINITE(_q(1)) &&
	      PX4_ISFINITE(_q(2)) && PX4_ISFINITE(_q(3)))) {
		  // Reset quaternion to last good state
		  _q = q_last;  //并把_rates和_gyro_bias置零便于下次调用时使用。
		  _rates.zero();
		  _gyro_bias.zero();
		  return false;
	}

	return true;
}


// 使用磁偏角更新四元数
void AttitudeEstimatorQ::update_mag_declination(float new_declination)
{
	// Apply initial declination or trivial rotations without changing estimation
	if (!_inited || fabsf(new_declination - _mag_decl) < 0.0001f) { //忽略微小旋转; 
		_mag_decl = new_declination;

	} else {
		// Immediately rotate current estimation to avoid gyro bias growth
		Quaternion decl_rotation;////磁偏超过一定值后，校正姿态;
		decl_rotation.from_yaw(new_declination - _mag_decl);
		_q = decl_rotation * _q;  //由磁偏角度转化为四元数; 
		_mag_decl = new_declination;//   //四元数相乘表示角度相加;
	}
}



int attitude_estimator_q_main(int argc, char *argv[])
{
	// 命令行参数解析 
	//外部调用接口;
	if (argc < 2) {
		warnx("usage: attitude_estimator_q {start|stop|status}");
		return 1;
	}

	if (!strcmp(argv[1], "start")) {//启动 

		if (attitude_estimator_q::instance != nullptr) {
			warnx("already running");
			return 1;
		}
		
        //实例化，instance;
		attitude_estimator_q::instance = new AttitudeEstimatorQ;

		if (attitude_estimator_q::instance == nullptr) {
			warnx("alloc failed");
			return 1;
		}

		if (OK != attitude_estimator_q::instance->start()) {
			delete attitude_estimator_q::instance;
			attitude_estimator_q::instance = nullptr;
			warnx("start failed");
			return 1;
		}

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {//停止  
		if (attitude_estimator_q::instance == nullptr) {
			warnx("not running");
			return 1;
		}

		delete attitude_estimator_q::instance;//删除实例化对象，指针置空;
		attitude_estimator_q::instance = nullptr;
		return 0;
	}

	if (!strcmp(argv[1], "status")) { //打印当前姿态信息;
		if (attitude_estimator_q::instance) {
			attitude_estimator_q::instance->print();
			warnx("running");
			return 0;

		} else {
			warnx("not running");
			return 1;
		}
	}

	warnx("unrecognized command");
	return 1;
}


/*
 最后说两个问题：数值误差的处理和陀螺仪的飘逸即PI反馈修正陀螺仪：

       由准确的陀螺信号进行准确的符号积分将会得到准确的、正确的旋转矩阵。即使采用准确的陀螺信号，数值积分仍会引入两类数值误差：
1. 积分误差。数值积分采用有限时间步长和具有有限采样率的数据。根据所使用的数值积分方法，对采样的数据做特定的假设。
我们所使用的方法假设在每个时间步长内旋转速度恒定不变。这将引入正比于旋转加速度的误差。
2. 量化误差。无论使用哪种方法表示量值，这些表达都是有限的，所以会存在量化误差。从模数转换开始，
到执行任何无法保留计算结果所有位数的计算，量化误差都将不断累积。
旋转矩阵的一个基本特征是它的正交性，即如果两个向量在某个参考系中是垂直的，那么它们在任意的参考系中都是垂直的。
同样，不同参考系下的同一向量长度相同。然而数值误差会破坏这一特征，例如，旋转矩阵的行列都是单位向量，它们的长度应该等于1，
但是数值误差会导致它们的长度变小或者变大。不断累积下去，它们的长度将会变为0或者无穷大。另外，旋转矩阵的行列应该是相互垂直的，
数值误差会导致它们相互倾斜，如下图所示：

重规范化
由于数值误差的存在，方向余弦矩阵不再满足正交性，即式(5)右端不再严格等于单位矩阵。事实上，此时坐标系不再描述一个刚体。
幸运的是，数值误差积累地非常缓慢，所以及时修正误差不是一件困难的事情。
我们把使方向余弦矩阵满足正交性的操作称为重规范化。有很多种方法可以实现重规范化操作。仿真结果显示它们的效果都比较好，
这里给出最简单的一种方法，操作流程如下：首先计算方向余弦矩阵X轴与Y轴的内积，
如果矩阵严格正交，那么这个结果应该是0，所以这个结果实际上反映了X轴与Y轴相互旋转靠近的程度。


陀螺仪的漂移问题，PID负反馈检测回路给陀螺补偿误差：

互补滤波就是在短时间内采用陀螺仪得到的角度做为最优值，定时对加速度采样来的加速度值进行取平均值来校正陀螺仪的得到的角度。
短时间内用陀螺仪比较准确，以它为主；长时间用加速度计比较准确，这时候加大它的比重，这就是互补了，不过加速度计要滤掉高频信号，
陀螺仪要滤掉低频信号，互补滤波器就是根据传感器特性不同，通过不同的滤波器（高通或低通，互补的），然后再相加得到整个频带的信号。
例如，加速度计测加速度值，其动态响应较慢，在高频时信号不可用，所以可通过低通滤波器抑制高频干扰；陀螺仪响应快，积分后可测倾角，
不过由于零点漂移等，在低频段信号不好，通过高通滤波器可抑制低频干扰。将两者结合，就将陀螺仪和加速度计的优点融合起来，
得到在高频和低频都较好的信号，互补滤波需要选择切换的频率点，即高通和低通的频率。

    虽然陀螺表现相当好，在指令下面每秒的误差很小，但最终对陀螺仪的漂移我们不得不做一些事情。
	所要做的就是利用其它方向的参考来探测陀螺偏移，提供一个经典的PID负反馈检测回路给陀螺补偿误差。如图1所示。步骤如下：

    1. 使用方向参考向量探测定向误差，通过计算一个旋转矢量，将测量值和计算值的参考矢量调整。

    2. 通过一个比例积分（ PI） 反馈控制器来产生的陀螺旋转校准速度，将旋转矢量误差反馈。（ PI调节器是常用的
PID反馈调节器的一个特例，D代表微分。在我们的这个例子中，我们不需要用到微分项。）

    3. 加上（ 或者减去，这依赖于你对旋转误差的符号约定） 比例积分控制器的输出到实际的陀螺仪信号。

    对方向参考向量的主要要求是：它不漂移。其瞬态性能并不那么重要是因为陀螺对方向估计有瞬态保真性。
GPS和加速度计为我们提供了两个参考向量提供。磁力计也是非常有用的，特别对偏航的控制，但是对飞行器飞行的
指向，仅一个全球定位系统就做得很好。如果你使用磁力计，你应该使用一个三轴磁力计提供一个矢量参考。 低成
本三轴磁力计在市场上可以很方便的买到。

    对于两个参考向量中的任一个，通过测得的向量的交叉乘积检测方向误差，这个测得的矢量用方向余弦矩阵估计。
	这个叉积特别合适有两个理由。它的大小与两个矢量夹角是正弦成正比，同时它的方向垂直这两个矢量。
所以它代表一个旋转轴和旋转量，需要旋转这个测量的矢量使它变得与这个估计的矢量平行。
    换句话说，它等于方向旋转误差的负值。通过比例积分控制器把它反馈到陀螺仪，定位估计逐渐被迫跟踪参考向量，
	这样陀螺的漂移就被消除了。测量从方向余弦矩阵计算出相应的参考矢量的叉积是一个错误的指示。它大约等于这个旋转，
	将不得不用这个参考矢量和这个计算矢量校准。我们感兴趣的是旋转校正量，这个量我们需要用在方向余弦矩阵中，
	它等于旋转误差的负值。通过交换叉积的秩序可以很方便的计算修正。矫正旋转等于通过参考矢量的方向余弦的矢量估计的叉积。
	我们用比例积分控制器来矫正陀螺旋转，因为它稳定，同时这个积分项完全消除了陀螺仪的漂移，包括热漂移，零残余方向误差。
	参考矢量误差是通过方向余弦矩阵的方式映射到陀螺的。所以这样的映射取决于惯性测量单元。
	例如，GPS参考矢量可能矫正X、Y、Z或者X、Y和Z轴陀螺信号，取决于地球坐标系轴的方向。

*/




