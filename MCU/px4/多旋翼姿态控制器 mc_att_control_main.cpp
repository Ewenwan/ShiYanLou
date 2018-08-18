/****************************************************************************
 *mc_att_control   Multicopter Attitude Control
  多旋翼姿态控制
 http://blog.csdn.net/qq_21842557/article/details/51439171
 http://blog.csdn.net/czyv587/article/details/51548377
 http://blog.csdn.net/oqqenvy12/article/details/72284457
 
 姿态控制有：fw_att_control、mc_att_control、mc_att_control_multiplatform、vtol_att_control
 位置控制有：fw_pos_control_l1、fw_pos_control_l1、mc_pos_control_multiplatform
 位置估计有：ekf_att_pos_estimator、local_position_estimator、position_estimator_inav
 
 主要就是通过改变旋翼的角速度来控制四旋翼无人机。
 每个旋翼产生一个推力（F1、F2、F3、F4）和一个力矩，
 其共同作用构成四旋翼无人机的主推力、偏航力矩、俯仰力矩和滚转力矩。
 在四旋翼无人机中，正对的一对旋翼旋转方向一致，另外一对与之相反，
 来抵消静态平稳飞行时的回转效应和气动力矩。升降以及RPY的实现不在赘述。
 控制对象就是四旋翼无人机，其动力学模型可以描述为：
 将其视为有一个力和三个力矩的三维刚体。
 
  控制流程：

        1）、预处理：各参数的初始化。

        2）、稳定roll-pitch的角速度。   即 对正Z轴

        3）、稳定roll-pitch的角度。

        4）、稳定yaw的角速度。

        5）、稳定yaw的角度。
		
  其中在第五步中有一个yaw的前馈控制（MC_YAW_FF）（因为 yaw 偏航响应会比较慢，所以使用前馈控制，提高响应速度）：
  
   MC_YAW_FF  yaw rate 偏航角速度 的前馈控制器的 参数 ：
     0 表示 非常慢的控制 控制器只有在看到偏航位置误差时才开始执行（移动）偏航角
	 1 表示快速响应控制，但是随着一些超控，控制器将立即偏航，始终保持偏航误差接近零。
	 该参数不是关键的，可以是在飞行中调整，在最差的情况下，偏航响应将迟缓或太快。
	 有效范围为0 到1。典型值为0.8 ... 0.9。 （Foraerial视频最佳值可能要小得多以获得顺利的响应）。
	 
	 Yawovershot不得超过2-5％。 偏航超调量不得超过 2-5%
	 
 首先感性认识一下姿态控制部分的框架，控制部分分为内外环控制，内环控制角速度、外环控制角度。
 控制过程是先根据目标姿态（target）和当前姿态（current）求出偏差角，
 然后通过角速度来修正这个偏差角，最终到达目标姿态。
 
 控制器频率  250Hz


 *
  先提一下pixhawk的整体逻辑：

 commander和navigator产生期望位置
 position_estimator是当前位置
 通过pos_ctrl产生期望姿态 或者由手动设定值 产生期望姿态
 attitude_estimator是当前姿态
 通过att_ctrl产生pwm的数值
 最后通过mixer和motor_driver控制4个电机
 
 ****************************************************************************/

/**
 包含两个反馈控制回路
   外环  P  比例控制器     控制 角度   输入：期望姿态角度(mc_pos_control姿态控制产生期望姿态，或者由手动设定值 产生期望姿态)
									         _v_att_sp.q_d[0], _v_att_sp.q_d[1], _v_att_sp.q_d[2], _v_att_sp.q_d[3]
                                       反馈：当前姿态(attitude_estimator是当前姿态)
											 _v_att_sp.q_d[0], _v_att_sp.q_d[1], _v_att_sp.q_d[2], _v_att_sp.q_d[3]
									   输出：角速度设定值
											 _rates_sp(0)  _rates_sp(1)   _rates_sp(2) 
   内环  PID 比例积分微分控制器 控制 角速度 误差
										输入：期望姿态角速度  _rates_sp(0)  _rates_sp(1)   _rates_sp(2)
										反馈：当前姿态角速度
										      	if (_selected_gyro == 0) {
		                  rates(0) = (_sensor_gyro.x - _sensor_correction.gyro_offset_0[0]) * _sensor_correction.gyro_scale_0[0];
						  rates(1) = (_sensor_gyro.y - _sensor_correction.gyro_offset_0[1]) * _sensor_correction.gyro_scale_0[1];
						  rates(2) = (_sensor_gyro.z - _sensor_correction.gyro_offset_0[2]) * _sensor_correction.gyro_scale_0[2];
										输出：执行器控制量 PWM 
						  	_actuators.control[0] = (PX4_ISFINITE(_att_control(0))) ? _att_control(0) : 0.0f; // 滚转 力矩 控制量
				            _actuators.control[1] = (PX4_ISFINITE(_att_control(1))) ? _att_control(1) : 0.0f; // 俯仰 力矩 控制量
							_actuators.control[2] = (PX4_ISFINITE(_att_control(2))) ? _att_control(2) : 0.0f; // 偏航 力矩 控制量
							_actuators.control[3] = (PX4_ISFINITE(_thrust_sp)) ? _thrust_sp : 0.0f;           // 油门 控制量
	 注意： yaw 偏航响应会比较慢  慢于 roll/pitch 横滚 和 俯仰 响应
 * The controller has two loops: P loop for angular error and PD loop for angular rate error.
 * Desired rotation calculated keeping in mind that yaw response is normally slower than roll/pitch.
 
 
 根据经验所得，控制toll-pitch比控制yaw更容易实现。
 
 比如同样是实现10°的变化，roll-pitch需要60ms左右；但是yaw控制器却需要接近150ms。

 
 小角度旋转 小偏差  旋转轴不固定 
大角度旋转 大偏差  转转轴固定
 * For small deviations controller rotates copter to have shortest path of thrust vector and independently rotates around yaw,
 * so actual rotation axis is not constant. For large deviations controller rotates copter around fixed axis.
 
这两种方法根据角度误差无缝地融合重量。 
 * These two approaches fused seamlessly with weight depending on angular error.
 
 * When thrust vector directed near-horizontally (e.g. roll ~= PI/2) yaw setpoint ignored because of singularity.
 
 控制器不使用欧拉角度进行工作，它们仅用于更加人性化的控制和记录。
 * Controller doesn't use Euler angles for work, they generated only for more human-friendly control and logging.
 
 如果旋转矩阵设定值无效，它将从欧拉角产生，以与旧位置控制器兼容。
 * If rotation matrix setpoint is invalid it will be generated from Euler angles for compatibility with old position controllers.
 */

#include <px4_config.h>
#include <px4_defines.h>
#include <px4_tasks.h>
#include <px4_posix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <drivers/drv_hrt.h>
#include <arch/board/board.h>
#include <uORB/uORB.h>
#include <uORB/topics/vehicle_attitude_setpoint.h> // 姿态角度设定值    
#include <uORB/topics/manual_control_setpoint.h>   // 人为控制 设定值
#include <uORB/topics/actuator_controls.h>         // 执行器控制
#include <uORB/topics/vehicle_rates_setpoint.h>    // 姿态角速度设定值？？
#include <uORB/topics/fw_virtual_rates_setpoint.h> // 固定翼 虚拟角速度设定值？？
#include <uORB/topics/mc_virtual_rates_setpoint.h> // 多旋翼 虚拟角速度设定值？？
#include <uORB/topics/control_state.h>             // 控制状态
#include <uORB/topics/vehicle_control_mode.h>      // 控制模式
#include <uORB/topics/vehicle_status.h>            // 飞行器状态
#include <uORB/topics/actuator_armed.h>            // 执行器解锁否
#include <uORB/topics/parameter_update.h>          // 参数是否更新
#include <uORB/topics/multirotor_motor_limits.h>   // 多旋翼电机限制  
#include <uORB/topics/mc_att_ctrl_status.h>        // 多旋翼姿态控制状态
#include <uORB/topics/battery_status.h>            // 电池状态
#include <uORB/topics/sensor_gyro.h>               // 陀螺仪
#include <uORB/topics/sensor_correction.h>         // 传感器校正后的数据

#include <systemlib/param/param.h>
#include <systemlib/err.h>
#include <systemlib/perf_counter.h>
#include <systemlib/systemlib.h>
#include <systemlib/circuit_breaker.h>
#include <lib/mathlib/mathlib.h>
#include <lib/geo/geo.h>
#include <lib/tailsitter_recovery/tailsitter_recovery.h>
#include <conversion/rotation.h>

/**
 * Multicopter attitude control app start / stop handling function
 *
 * @ingroup apps
 */
// 按照C++语言的格式引用C语言的main函数   起点函数  在最后
extern "C" __EXPORT int mc_att_control_main(int argc, char *argv[]);

#define YAW_DEADZONE	0.05f
#define MIN_TAKEOFF_THRUST    0.2f
#define TPA_RATE_LOWER_LIMIT 0.05f
#define MANUAL_THROTTLE_MAX_MULTICOPTER	0.9f
#define ATTITUDE_TC_DEFAULT 0.2f

#define AXIS_INDEX_ROLL 0
#define AXIS_INDEX_PITCH 1
#define AXIS_INDEX_YAW 2
#define AXIS_COUNT 3

#define MAX_GYRO_COUNT 3

class MulticopterAttitudeControl
{
public:
	/**
	 * Constructor
	 */
	MulticopterAttitudeControl();//构造函数，初始化参数;

	/**
	 * Destructor, also kills the main task
	 */
	~MulticopterAttitudeControl();//析构函数，杀掉所有任务;

	/**
	 * Start the multicopter attitude control task.
	 *
	 * @return		OK on success.
	 */
	int		start();//启动 多旋翼姿态控制 任务 

private:

	bool	_task_should_exit;		/**< if true, task_main() should exit */
	int		_control_task;			/**< task handle */

	int		_ctrl_state_sub;		/**< control state subscription */
	int		_v_att_sp_sub;			/**< vehicle attitude setpoint subscription */
	int		_v_rates_sp_sub;		/**< vehicle rates setpoint subscription */
	int		_v_control_mode_sub;	/**< vehicle control mode subscription */
	int		_params_sub;			/**< parameter updates subscription */
	int		_manual_control_sp_sub;	/**< manual control setpoint subscription */
	int		_armed_sub;				/**< arming status subscription */
	int		_vehicle_status_sub;	/**< vehicle status subscription */
	int 	_motor_limits_sub;		/**< motor limits subscription */
	int 	_battery_status_sub;	/**< battery status subscription */
	int	_sensor_gyro_sub[MAX_GYRO_COUNT];	/**< gyro data subscription */
	int	_sensor_correction_sub;	/**< sensor thermal correction subscription */

	unsigned _gyro_count;
	int _selected_gyro;

	orb_advert_t	_v_rates_sp_pub;		/**< rate setpoint publication */
	orb_advert_t	_actuators_0_pub;		/**< attitude actuator controls publication */
	orb_advert_t	_controller_status_pub;	/**< controller status publication */

	orb_id_t _rates_sp_id;	/**< pointer to correct rates setpoint uORB metadata structure */
	orb_id_t _actuators_id;	/**< pointer to correct actuator controls0 uORB metadata structure */

	bool		_actuators_0_circuit_breaker_enabled;	/**< circuit breaker to suppress output */

	struct control_state_s				_ctrl_state;		/**< control state  反馈值 机身 姿态角度  姿态角速度 风速 */
	struct vehicle_attitude_setpoint_s	_v_att_sp;			/**< vehicle attitude setpoint */
	struct vehicle_rates_setpoint_s		_v_rates_sp;		/**< vehicle rates setpoint */
	struct manual_control_setpoint_s	_manual_control_sp;	/**< manual control setpoint */
	struct vehicle_control_mode_s		_v_control_mode;	/**< vehicle control mode */
	struct actuator_controls_s			_actuators;			/**< actuator controls */
	struct actuator_armed_s				_armed;				/**< actuator arming status */
	struct vehicle_status_s				_vehicle_status;	/**< vehicle status */
	struct multirotor_motor_limits_s	_motor_limits;		/**< motor limits */
	struct mc_att_ctrl_status_s 		_controller_status; /**< controller status */
	struct battery_status_s				_battery_status;	/**< battery status */
	struct sensor_gyro_s			_sensor_gyro;		/**< gyro data before thermal correctons and ekf bias estimates are applied */
	struct sensor_correction_s		_sensor_correction;		/**< sensor thermal corrections */

	union {
		struct {
			uint16_t motor_pos	: 1; // 0 - true when any motor has saturated in the positive direction
			uint16_t motor_neg	: 1; // 1 - true when any motor has saturated in the negative direction
			uint16_t roll_pos	: 1; // 2 - true when a positive roll demand change will increase saturation
			uint16_t roll_neg	: 1; // 3 - true when a negative roll demand change will increase saturation
			uint16_t pitch_pos	: 1; // 4 - true when a positive pitch demand change will increase saturation
			uint16_t pitch_neg	: 1; // 5 - true when a negative pitch demand change will increase saturation
			uint16_t yaw_pos	: 1; // 6 - true when a positive yaw demand change will increase saturation
			uint16_t yaw_neg	: 1; // 7 - true when a negative yaw demand change will increase saturation
			uint16_t thrust_pos	: 1; // 8 - true when a positive thrust demand change will increase saturation
			uint16_t thrust_neg	: 1; // 9 - true when a negative thrust demand change will increase saturation
		} flags;
		uint16_t value;
	} _saturation_status;

	perf_counter_t	_loop_perf;			/**< loop performance counter */
	perf_counter_t	_controller_latency_perf;

	math::Vector<3>		_rates_prev;	/**< angular rates on previous step */
	math::Vector<3>		_rates_sp_prev; /**< previous rates setpoint */
	math::Vector<3>		_rates_sp;		/**< angular rates setpoint */
	math::Vector<3>		_rates_int;		/**< angular rates integral error */
	float				_thrust_sp;		/**< thrust setpoint */
	math::Vector<3>		_att_control;	/**< attitude control vector */

	math::Matrix<3, 3>  _I;				/**< identity matrix */

	math::Matrix<3, 3>	_board_rotation = {};	/**< rotation matrix for the orientation that the board is mounted */

	struct {
		// roll 滚转 控制器参数
		param_t roll_p;         //角度   比例控制增益
		param_t roll_rate_p;    //角速度 比例控制器增益 
		param_t roll_rate_i;    //角速度 积分控制器增益
		param_t roll_rate_integ_lim;//角速度 积分限制   抗积分饱和！！！！！！！！！！！！！
		param_t roll_rate_d;        //角速度 微分增益
		param_t roll_rate_ff;       //前馈控制
		// pitch 俯仰控制器参数
		param_t pitch_p;
		param_t pitch_rate_p;
		param_t pitch_rate_i;
		param_t pitch_rate_integ_lim;
		param_t pitch_rate_d;
		param_t pitch_rate_ff;
		//油门
		param_t tpa_breakpoint_p;
		param_t tpa_breakpoint_i;
		param_t tpa_breakpoint_d;
		param_t tpa_rate_p;
		param_t tpa_rate_i;
		param_t tpa_rate_d;
		// yaw 偏航控制器参数
		param_t yaw_p;
		param_t yaw_rate_p;
		param_t yaw_rate_i;
		param_t yaw_rate_integ_lim;
		param_t yaw_rate_d;
		param_t yaw_rate_ff;
		
		param_t yaw_ff;
		param_t roll_rate_max;
		param_t pitch_rate_max;
		param_t yaw_rate_max;
		param_t yaw_auto_max;

		param_t acro_roll_max;
		param_t acro_pitch_max;
		param_t acro_yaw_max;
		param_t rattitude_thres;

		param_t vtol_type;
		param_t roll_tc;
		param_t pitch_tc;
		param_t vtol_opt_recovery_enabled;
		param_t vtol_wv_yaw_rate_scale;

		param_t bat_scale_en;

		param_t board_rotation;

		param_t board_offset[3];

	}		_params_handles;		/**< handles for interesting parameters */

	struct {
		math::Vector<3> att_p;				/**< P gain for angular error 角度误差 增益*/
		math::Vector<3> rate_p;				/**< P gain for angular rate error 角速度误差比例增益*/
		math::Vector<3> rate_i;				/**< I gain for angular rate error 角速度误差积分增益*/
		math::Vector<3> rate_int_lim;			/**< integrator state limit for rate loop 角速度环 积分限制*/
		math::Vector<3> rate_d;				/**< D gain for angular rate error 角速度误差微分增益*/
		math::Vector<3>	rate_ff;			/**< Feedforward gain for desired rates 期望角速度的 前馈增益 */
		float yaw_ff;						/**< yaw control feed-forward 偏航控制的 前馈控制参数增益*/

		float tpa_breakpoint_p;				/**< 油门 Throttle PID Attenuation breakpoint */
		float tpa_breakpoint_i;				/**< Throttle PID Attenuation breakpoint */
		float tpa_breakpoint_d;				/**< Throttle PID Attenuation breakpoint */
		float tpa_rate_p;					/**< Throttle PID Attenuation slope */
		float tpa_rate_i;					/**< Throttle PID Attenuation slope */
		float tpa_rate_d;					/**< Throttle PID Attenuation slope */

		float roll_rate_max;
		float pitch_rate_max;
		float yaw_rate_max;
		float yaw_auto_max;
		math::Vector<3> mc_rate_max;		/**< attitude rate limits in stabilized modes */
		math::Vector<3> auto_rate_max;		/**< attitude rate limits in auto modes */
		math::Vector<3> acro_rate_max;		/**< max attitude rates in acro mode */
		float rattitude_thres;
		int vtol_type;						/**< 0 = Tailsitter, 1 = Tiltrotor, 2 = Standard airframe */
		bool vtol_opt_recovery_enabled;
		float vtol_wv_yaw_rate_scale;			/**< Scale value [0, 1] for yaw rate setpoint  */

		int bat_scale_en;

		int board_rotation;

		float board_offset[3];

	}		_params;

	TailsitterRecovery *_ts_opt_recovery;	/**< Computes optimal rates for tailsitter recovery */

	/**
	 * Update our local parameter cache.
	 */
	int			parameters_update();

	/**
	 * Check for parameter update and handle it.
	 */
	void		parameter_update_poll();

	/**
	 * Check for changes in vehicle control mode.  控制模式
	 */
	void		vehicle_control_mode_poll(); 

	/**
	 * Check for changes in manual inputs. 人为输入
	 */
	void		vehicle_manual_poll();

	/**
	 * Check for attitude setpoint updates. 角度设定值 
	 */
	void		vehicle_attitude_setpoint_poll();

	/**
	 * Check for rates setpoint updates. 角速度设定值
	 */
	void		vehicle_rates_setpoint_poll();

	/**
	 * Check for arming status updates. 解锁状态
	 */
	void		arming_status_poll();

	/**
	 * Attitude controller.        角度控制器
	 */
	void		control_attitude(float dt);

	/**
	 * Attitude rates controller.  角速度控制器
	 */
	void		control_attitude_rates(float dt);

	/**
	 * Throttle PID attenuation.
	 */
	math::Vector<3> pid_attenuations(float tpa_breakpoint, float tpa_rate);

	/**
	 * Check for vehicle status updates.
	 */
	void		vehicle_status_poll();

	/**
	 * Check for vehicle motor limits status.
	 */
	void		vehicle_motor_limits_poll();//电机限制

	/**
	 * Check for battery status updates.
	 */
	void		battery_status_poll();//电池状态

	/**
	 * Check for control state updates.
	 */
	void		control_state_poll(); // 反馈值  当前机身 姿态角度  姿态角速度  风速 

	/**
	 * Check for sensor thermal correction updates.
	 */
	void		sensor_correction_poll();// 传感器校准数据

	/**
	 * Shim for calling task_main from task_create.
	 */
	static void	task_main_trampoline(int argc, char *argv[]);

	/**
	 * Main attitude control task.
	 */
	void		task_main();
};

namespace mc_att_control
{

MulticopterAttitudeControl	*g_control;
}

MulticopterAttitudeControl::MulticopterAttitudeControl() :

	_task_should_exit(false),
	_control_task(-1),

	/* subscriptions */
	_ctrl_state_sub(-1),
	_v_att_sp_sub(-1),
	_v_control_mode_sub(-1),
	_params_sub(-1),
	_manual_control_sp_sub(-1),
	_armed_sub(-1),
	_vehicle_status_sub(-1),
	_motor_limits_sub(-1),
	_battery_status_sub(-1),
	_sensor_correction_sub(-1),

	/* gyro selection */
	_gyro_count(1),
	_selected_gyro(0),

	/* publications */
	_v_rates_sp_pub(nullptr),
	_actuators_0_pub(nullptr),
	_controller_status_pub(nullptr),
	_rates_sp_id(nullptr),
	_actuators_id(nullptr),

	_actuators_0_circuit_breaker_enabled(false),

	_ctrl_state{},
	_v_att_sp{},
	_v_rates_sp{},
	_manual_control_sp{},
	_v_control_mode{},
	_actuators{},
	_armed{},
	_vehicle_status{},
	_motor_limits{},
	_controller_status{},
	_battery_status{},
	_sensor_gyro{},
	_sensor_correction{},

	_saturation_status{},
	/* performance counters */
	_loop_perf(perf_alloc(PC_ELAPSED, "mc_att_control")),
	_controller_latency_perf(perf_alloc_once(PC_ELAPSED, "ctrl_latency")),
	_ts_opt_recovery(nullptr)

{
	for (uint8_t i = 0; i < MAX_GYRO_COUNT; i++) {
		_sensor_gyro_sub[i] = -1;
	}

	_vehicle_status.is_rotary_wing = true;

	_params.att_p.zero();
	_params.rate_p.zero();
	_params.rate_i.zero();
	_params.rate_int_lim.zero();
	_params.rate_d.zero();
	_params.rate_ff.zero();
	_params.yaw_ff = 0.0f;
	_params.roll_rate_max = 0.0f;
	_params.pitch_rate_max = 0.0f;
	_params.yaw_rate_max = 0.0f;
	_params.mc_rate_max.zero();
	_params.auto_rate_max.zero();
	_params.acro_rate_max.zero();
	_params.rattitude_thres = 1.0f;
	_params.vtol_opt_recovery_enabled = false;
	_params.vtol_wv_yaw_rate_scale = 1.0f;
	_params.bat_scale_en = 0;

	_params.board_rotation = 0;

	_params.board_offset[0] = 0.0f;
	_params.board_offset[1] = 0.0f;
	_params.board_offset[2] = 0.0f;

	_rates_prev.zero();
	_rates_sp.zero();
	_rates_sp_prev.zero();
	_rates_int.zero();
	_thrust_sp = 0.0f;
	_att_control.zero();

	_I.identity();
	_board_rotation.identity();

	_params_handles.roll_p			= 	param_find("MC_ROLL_P");
	_params_handles.roll_rate_p		= 	param_find("MC_ROLLRATE_P");
	_params_handles.roll_rate_i		= 	param_find("MC_ROLLRATE_I");
	_params_handles.roll_rate_integ_lim	= 	param_find("MC_RR_INT_LIM");
	_params_handles.roll_rate_d		= 	param_find("MC_ROLLRATE_D");
	_params_handles.roll_rate_ff	= 	param_find("MC_ROLLRATE_FF");
	_params_handles.pitch_p			= 	param_find("MC_PITCH_P");
	_params_handles.pitch_rate_p	= 	param_find("MC_PITCHRATE_P");
	_params_handles.pitch_rate_i	= 	param_find("MC_PITCHRATE_I");
	_params_handles.pitch_rate_integ_lim	= 	param_find("MC_PR_INT_LIM");
	_params_handles.pitch_rate_d	= 	param_find("MC_PITCHRATE_D");
	_params_handles.pitch_rate_ff 	= 	param_find("MC_PITCHRATE_FF");
	_params_handles.tpa_breakpoint_p 	= 	param_find("MC_TPA_BREAK_P");
	_params_handles.tpa_breakpoint_i 	= 	param_find("MC_TPA_BREAK_I");
	_params_handles.tpa_breakpoint_d 	= 	param_find("MC_TPA_BREAK_D");
	_params_handles.tpa_rate_p	 	= 	param_find("MC_TPA_RATE_P");
	_params_handles.tpa_rate_i	 	= 	param_find("MC_TPA_RATE_I");
	_params_handles.tpa_rate_d	 	= 	param_find("MC_TPA_RATE_D");
	_params_handles.yaw_p			=	param_find("MC_YAW_P");
	_params_handles.yaw_rate_p		= 	param_find("MC_YAWRATE_P");
	_params_handles.yaw_rate_i		= 	param_find("MC_YAWRATE_I");
	_params_handles.yaw_rate_integ_lim	= 	param_find("MC_YR_INT_LIM");
	_params_handles.yaw_rate_d		= 	param_find("MC_YAWRATE_D");
	_params_handles.yaw_rate_ff	 	= 	param_find("MC_YAWRATE_FF");
	_params_handles.yaw_ff			= 	param_find("MC_YAW_FF");
	_params_handles.roll_rate_max	= 	param_find("MC_ROLLRATE_MAX");
	_params_handles.pitch_rate_max	= 	param_find("MC_PITCHRATE_MAX");
	_params_handles.yaw_rate_max	= 	param_find("MC_YAWRATE_MAX");
	_params_handles.yaw_auto_max	= 	param_find("MC_YAWRAUTO_MAX");
	_params_handles.acro_roll_max	= 	param_find("MC_ACRO_R_MAX");
	_params_handles.acro_pitch_max	= 	param_find("MC_ACRO_P_MAX");
	_params_handles.acro_yaw_max	= 	param_find("MC_ACRO_Y_MAX");
	_params_handles.rattitude_thres = 	param_find("MC_RATT_TH");
	_params_handles.vtol_type 		= 	param_find("VT_TYPE");
	_params_handles.roll_tc			= 	param_find("MC_ROLL_TC");
	_params_handles.pitch_tc		= 	param_find("MC_PITCH_TC");
	_params_handles.vtol_opt_recovery_enabled	= param_find("VT_OPT_RECOV_EN");
	_params_handles.vtol_wv_yaw_rate_scale		= param_find("VT_WV_YAWR_SCL");
	_params_handles.bat_scale_en		= param_find("MC_BAT_SCALE_EN");

	/* rotations */
	_params_handles.board_rotation = param_find("SENS_BOARD_ROT");

	/* rotation offsets */
	_params_handles.board_offset[0] = param_find("SENS_BOARD_X_OFF");
	_params_handles.board_offset[1] = param_find("SENS_BOARD_Y_OFF");
	_params_handles.board_offset[2] = param_find("SENS_BOARD_Z_OFF");



	/* fetch initial parameter values */
	parameters_update();

	if (_params.vtol_type == 0 && _params.vtol_opt_recovery_enabled) {
		// the vehicle is a tailsitter, use optimal recovery control strategy
		_ts_opt_recovery = new TailsitterRecovery();
	}

	/* initialize thermal corrections as we might not immediately get a topic update (only non-zero values) */
	for (unsigned i = 0; i < 3; i++) {
		// used scale factors to unity
		_sensor_correction.gyro_scale_0[i] = 1.0f;
		_sensor_correction.gyro_scale_1[i] = 1.0f;
		_sensor_correction.gyro_scale_2[i] = 1.0f;
	}

}

MulticopterAttitudeControl::~MulticopterAttitudeControl()
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

	if (_ts_opt_recovery != nullptr) {
		delete _ts_opt_recovery;
	}

	mc_att_control::g_control = nullptr;
}

// 私有数据（private） 参数初始化 
// _params_handles和_params这两种数据结构（struct类型）
/*
其中param_get()函数比较重要，特别是内部使用的lock和unlock的使用（主要就是通过sem信号量控制对某一数据的互斥访问）
Int param_get(param_t param, void *val)  
{  
    int result = -1;  
    param_lock();  
    const void *v = param_get_value_ptr(param);  
    if (val != NULL) {  
        memcpy(val, v, param_size(param));  
        result = 0;  
    }  
    param_unlock();  
    return result;  
} 

上述使用的*lock和*unlock通过sem实现互斥访问（进临界区），源码如下。
// lock the parameter store  
static void param_lock(void)  
{  
    //do {} while (px4_sem_wait(¶m_sem) != 0);  
}  
// unlock the parameter store
static void param_unlock(void)  
{  
    //px4_sem_post(¶m_sem);  
}  

上面是开源代码中的，代码里面把lock和unlock函数都写成空函数了，那还有屁用啊。
应该是由于程序开发和版本控制不是一个人，有的程序开发到一半人走了，搞版本控制的，
又找不到新的人来进行开发，搁置了忘记修改回来了吧；再或者别的什么意图。

    经过上述分析，该parameters_update()
	函数主要就是获取roll、pitch、yaw的PID参数的。
	并对三种飞行模式（stablize、auto、acro）下的最大姿态速度做了限制。
*/

int
MulticopterAttitudeControl::parameters_update()
{
	float v;

	float roll_tc, pitch_tc;

	param_get(_params_handles.roll_tc, &roll_tc);
	param_get(_params_handles.pitch_tc, &pitch_tc);

	/* roll gains 滚转增益参数*/
	param_get(_params_handles.roll_p, &v);//角度 比例增益
	_params.att_p(0) = v * (ATTITUDE_TC_DEFAULT / roll_tc);
	param_get(_params_handles.roll_rate_p, &v);//角速度 比例增益
	_params.rate_p(0) = v * (ATTITUDE_TC_DEFAULT / roll_tc);
	param_get(_params_handles.roll_rate_i, &v);//角速度 积分增益
	_params.rate_i(0) = v;
	param_get(_params_handles.roll_rate_integ_lim, &v);//角速度 积分限制 抗积分饱和
	_params.rate_int_lim(0) = v;
	param_get(_params_handles.roll_rate_d, &v);.
	_params.rate_d(0) = v * (ATTITUDE_TC_DEFAULT / roll_tc);//角速度 微分增益
	param_get(_params_handles.roll_rate_ff, &v); //角速度 前馈控制参数
	_params.rate_ff(0) = v;

	/* pitch gains 俯仰增益参数*/
	param_get(_params_handles.pitch_p, &v);
	_params.att_p(1) = v * (ATTITUDE_TC_DEFAULT / pitch_tc);
	param_get(_params_handles.pitch_rate_p, &v);
	_params.rate_p(1) = v * (ATTITUDE_TC_DEFAULT / pitch_tc);
	param_get(_params_handles.pitch_rate_i, &v);
	_params.rate_i(1) = v;
	param_get(_params_handles.pitch_rate_integ_lim, &v);
	_params.rate_int_lim(1) = v;
	param_get(_params_handles.pitch_rate_d, &v);
	_params.rate_d(1) = v * (ATTITUDE_TC_DEFAULT / pitch_tc);
	param_get(_params_handles.pitch_rate_ff, &v);
	_params.rate_ff(1) = v;
	
// 油门 增益参数
	param_get(_params_handles.tpa_breakpoint_p, &_params.tpa_breakpoint_p);
	param_get(_params_handles.tpa_breakpoint_i, &_params.tpa_breakpoint_i);
	param_get(_params_handles.tpa_breakpoint_d, &_params.tpa_breakpoint_d);
	param_get(_params_handles.tpa_rate_p, &_params.tpa_rate_p);
	param_get(_params_handles.tpa_rate_i, &_params.tpa_rate_i);
	param_get(_params_handles.tpa_rate_d, &_params.tpa_rate_d);

	/* yaw gains 偏航增益参数*/
	param_get(_params_handles.yaw_p, &v);
	_params.att_p(2) = v;
	param_get(_params_handles.yaw_rate_p, &v);
	_params.rate_p(2) = v;
	param_get(_params_handles.yaw_rate_i, &v);
	_params.rate_i(2) = v;
	param_get(_params_handles.yaw_rate_integ_lim, &v);
	_params.rate_int_lim(2) = v;
	param_get(_params_handles.yaw_rate_d, &v);
	_params.rate_d(2) = v;
	param_get(_params_handles.yaw_rate_ff, &v);
	_params.rate_ff(2) = v;

//偏航角前馈参数
	param_get(_params_handles.yaw_ff, &_params.yaw_ff);

	/* angular rate limits 角速度限制 */
	param_get(_params_handles.roll_rate_max, &_params.roll_rate_max);
	_params.mc_rate_max(0) = math::radians(_params.roll_rate_max);
	param_get(_params_handles.pitch_rate_max, &_params.pitch_rate_max);
	_params.mc_rate_max(1) = math::radians(_params.pitch_rate_max);
	param_get(_params_handles.yaw_rate_max, &_params.yaw_rate_max);
	_params.mc_rate_max(2) = math::radians(_params.yaw_rate_max);

	/* auto angular rate limits 自动角速度限制*/
	param_get(_params_handles.roll_rate_max, &_params.roll_rate_max);
	_params.auto_rate_max(0) = math::radians(_params.roll_rate_max);
	param_get(_params_handles.pitch_rate_max, &_params.pitch_rate_max);
	_params.auto_rate_max(1) = math::radians(_params.pitch_rate_max);
	param_get(_params_handles.yaw_auto_max, &_params.yaw_auto_max);
	_params.auto_rate_max(2) = math::radians(_params.yaw_auto_max);

	/* manual rate control scale and auto mode roll/pitch rate limits */
	param_get(_params_handles.acro_roll_max, &v);
	_params.acro_rate_max(0) = math::radians(v);
	param_get(_params_handles.acro_pitch_max, &v);
	_params.acro_rate_max(1) = math::radians(v);
	param_get(_params_handles.acro_yaw_max, &v);
	_params.acro_rate_max(2) = math::radians(v);

	/* stick deflection needed in rattitude mode to control rates not angles */
	param_get(_params_handles.rattitude_thres, &_params.rattitude_thres);

	param_get(_params_handles.vtol_type, &_params.vtol_type);

	int tmp;
	param_get(_params_handles.vtol_opt_recovery_enabled, &tmp);
	_params.vtol_opt_recovery_enabled = (bool)tmp;

	param_get(_params_handles.vtol_wv_yaw_rate_scale, &_params.vtol_wv_yaw_rate_scale);

	param_get(_params_handles.bat_scale_en, &_params.bat_scale_en);

	_actuators_0_circuit_breaker_enabled = circuit_breaker_enabled("CBRK_RATE_CTRL", CBRK_RATE_CTRL_KEY);

	/* rotation of the autopilot relative to the body */
	param_get(_params_handles.board_rotation, &(_params.board_rotation));

	/* fine adjustment of the rotation */
	param_get(_params_handles.board_offset[0], &(_params.board_offset[0]));
	param_get(_params_handles.board_offset[1], &(_params.board_offset[1]));
	param_get(_params_handles.board_offset[2], &(_params.board_offset[2]));

	return OK;
}


//////////////////////////////////////////////////////////////
/*以下为检查各个话题上是否更新数据*/

//参数话题是否更新 如果有更新则更新相应数据或
void
MulticopterAttitudeControl::parameter_update_poll()
{
	bool updated;

	/* Check if parameters have changed */
	orb_check(_params_sub, &updated);//获取参数话题更新标志

	if (updated) {
		struct parameter_update_s param_update;
		orb_copy(ORB_ID(parameter_update), _params_sub, &param_update);
		parameters_update();//更新参数
	}
}

// 控制模式 话题数据 更新
void
MulticopterAttitudeControl::vehicle_control_mode_poll()
{
	bool updated;

	/* Check if vehicle control mode has changed */
	orb_check(_v_control_mode_sub, &updated);

	if (updated) {
		// 得到 当前控制模式
		orb_copy(ORB_ID(vehicle_control_mode), _v_control_mode_sub, &_v_control_mode);
	}
}

// 人工控制设定值
void
MulticopterAttitudeControl::vehicle_manual_poll()
{
	bool updated;

	/* get pilots inputs */
	orb_check(_manual_control_sp_sub, &updated);

	if (updated) {
		//得到 人工设定值
		orb_copy(ORB_ID(manual_control_setpoint), _manual_control_sp_sub, &_manual_control_sp);
	}
}

// 姿态角度设定值更新
void
MulticopterAttitudeControl::vehicle_attitude_setpoint_poll()
{
	/* check if there is a new setpoint */
	bool updated;
	orb_check(_v_att_sp_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(vehicle_attitude_setpoint), _v_att_sp_sub, &_v_att_sp);
	}
}

// 姿态角速度设定值更新
void
MulticopterAttitudeControl::vehicle_rates_setpoint_poll()
{
	/* check if there is a new setpoint */
	bool updated;
	orb_check(_v_rates_sp_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(vehicle_rates_setpoint), _v_rates_sp_sub, &_v_rates_sp);
	}
}

//执行器解锁状态 更新
void
MulticopterAttitudeControl::arming_status_poll()
{
	/* check if there is a new setpoint */
	bool updated;
	orb_check(_armed_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(actuator_armed), _armed_sub, &_armed);
	}
}

// 飞行器状态 更新  角速度设定值 以及 相应模式下执行器编号 
void
MulticopterAttitudeControl::vehicle_status_poll()
{
	/* check if there is new status information */
	bool vehicle_status_updated;
	orb_check(_vehicle_status_sub, &vehicle_status_updated);

	if (vehicle_status_updated) {
		orb_copy(ORB_ID(vehicle_status), _vehicle_status_sub, &_vehicle_status);

		/* set correct uORB ID, depending on if vehicle is VTOL or not */
		if (!_rates_sp_id) {
			if (_vehicle_status.is_vtol) {// 垂直起降模式
				_rates_sp_id = ORB_ID(mc_virtual_rates_setpoint);
				_actuators_id = ORB_ID(actuator_controls_virtual_mc);

			} else {
				_rates_sp_id = ORB_ID(vehicle_rates_setpoint);// 姿态角速度设定值
				_actuators_id = ORB_ID(actuator_controls_0);  // 对应 执行器ID编号
			}
		}
	}
}

// 多旋翼 电机 限制
void
MulticopterAttitudeControl::vehicle_motor_limits_poll()
{
	/* check if there is a new message */
	bool updated;
	orb_check(_motor_limits_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(multirotor_motor_limits), _motor_limits_sub, &_motor_limits);
		_saturation_status.value = _motor_limits.saturation_status;
	}
}

// 电池状态  更新
void
MulticopterAttitudeControl::battery_status_poll()
{
	/* check if there is a new message */
	bool updated;
	orb_check(_battery_status_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(battery_status), _battery_status_sub, &_battery_status);
	}
}

//机体当前控制状态  机体系下 姿态角度设定值  角速度设定值  以及空速 
void
MulticopterAttitudeControl::control_state_poll()
{
	/* check if there is a new message */
	bool updated;
	orb_check(_ctrl_state_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(control_state), _ctrl_state_sub, &_ctrl_state);
	}
}

/*
//control_state.msg 文件
uint8 AIRSPD_MODE_MEAS = 0	    # 传感器测量的 风速    airspeed is measured airspeed from sensor
uint8 AIRSPD_MODE_EST = 1	    # 机身速度估算 的风速  airspeed is estimated by body velocity
uint8 AIRSPD_MODE_DISABLED = 2	# 风速数据未被使能     airspeed is disabled

# 机身坐标系 
float32 x_acc			# X轴加速度  X acceleration in body frame
float32 y_acc			# Y轴加速度  Y acceleration in body frame
float32 z_acc			# Z轴加速度  Z acceleration in body frame
float32 x_vel			# X轴速度    X velocity in body frame
float32 y_vel			# Y轴速度    Y velocity in body frame
float32 z_vel			# Z轴速度    Z velocity in body frame
# 本地坐标系
float32 x_pos			# x轴坐标值  X position in local earth frame
float32 y_pos			# y轴坐标值  Y position in local earth frame
float32 z_pos			# z轴坐标值  z position in local earth frame

float32 airspeed		# 估计的空速    Airspeed, estimated
bool airspeed_valid		# 空速值有效性  False: Non-finite values or non-updating sensor

float32[3] vel_variance	# 速度估计方差   Variance in body velocity estimate
float32[3] pos_variance	# 位置估计方差   Variance in local position estimate

float32[4] q 			# 当前姿态四元素   Attitude Quaternion  可以 转化成旋转矩阵 q_att.to_dcm()

float32[4] delta_q_reset 	# 四元素变化量 Amount by which quaternion has changed during last reset
uint8 quat_reset_counter	# 四元素更新（复位？？）计数 Quaternion reset counter

float32 roll_rate		    # 当前姿态 横滚角速度 Roll body angular rate (rad/s, x forward/y right/z down) - corrected for bias
float32 pitch_rate		    # 当前姿态 俯仰角速度 Pitch body angular rate (rad/s, x forward/y right/z down) - corrected for bias
float32 yaw_rate		    # 当前姿态 偏航角速度 Yaw body angular rate (rad/s, x forward/y right/z down) - corrected for bias
float32 horz_acc_mag		# 低通滤波后的 水平加速度幅值 low pass filtered magnitude of the horizontal acceleration

# 用于 陀螺仪数据的校正
float32 roll_rate_bias		# 横滚角速度偏差 需要从未校正的陀螺仪数据中减去 Roll body angular rate bias (rad/s, x forward) - 
float32 pitch_rate_bias		# 俯仰角速度偏差 Pitch body angular rate bias (rad/s, y right) - subtract from uncorrected gyro data
float32 yaw_rate_bias		# 偏航角速度偏差 Yaw body angular rate bias (rad/s, z down) - subtract from uncorrected gyro data

*/

/*
struct log_CTS_s control_state;  
struct log_CTS_s {  
         float vx_body;  
         float vy_body;  
         float vz_body;  
         float airspeed;  
         float roll_rate;  
         float pitch_rate;  
         float yaw_rate;  
};
*/



// 传感器校正数据 更新
void
MulticopterAttitudeControl::sensor_correction_poll()
{
	/* check if there is a new message */
	bool updated;
	orb_check(_sensor_correction_sub, &updated);

	if (updated) {
		orb_copy(ORB_ID(sensor_correction), _sensor_correction_sub, &_sensor_correction);
	}

	/* update the latest gyro selection */
	if (_sensor_correction.selected_gyro_instance < sizeof(_sensor_gyro_sub) / sizeof(_sensor_gyro_sub[0])) {
		_selected_gyro = _sensor_correction.selected_gyro_instance;
	}
}

/**
 * 姿态角度控制器         Attitude controller.
 * 输入：姿态角度设置值   Input: 'vehicle_attitude_setpoint' topics (depending on mode)
 * 输出；角速度设定值 Output: '_rates_sp' vector, '_thrust_sp'
 由于偏航轴的响应比横滚俯仰慢，这里采用的是倾转分离的解耦合进行姿态角控制：

先进行一个倾斜（对齐Z轴（ROLL 和 pitch ）），然后旋转（对齐X轴），这样以保证能够将当前机体坐标系迅速地旋转到目标坐标系，
通过这种方式求解旋转向量（详见《视觉SLAM十四讲》）仅适用于小角度情形；

 */
 // 姿态角度控制器   外环 比例控制器
void
MulticopterAttitudeControl::control_attitude(float dt)
{
	vehicle_attitude_setpoint_poll();//更新 姿态角度设置

	_thrust_sp = _v_att_sp.thrust;   //油门设定值

	/* construct attitude setpoint rotation matrix  设置的 四元素设定值*/
	math::Quaternion q_sp(_v_att_sp.q_d[0], _v_att_sp.q_d[1], _v_att_sp.q_d[2], _v_att_sp.q_d[3]);
	math::Matrix<3, 3> R_sp = q_sp.to_dcm();//四元数转换到 旋转矩阵 得到目标设定值 旋转矩阵

	/* get current rotation matrix from control state quaternions */
	math::Quaternion q_att(_ctrl_state.q[0], _ctrl_state.q[1], _ctrl_state.q[2], _ctrl_state.q[3]);
	math::Matrix<3, 3> R = q_att.to_dcm();//四元数转换到 旋转矩阵 得到 当前 姿态 旋转矩阵

    /*  四元数转换到 DCM旋转矩阵 函数  
	Firmware/src/lib/mathlib/math/Quaternion.hpp 
	to_dcm(
	 Matrix<3, 3> to_dcm(void) const {  
        Matrix<3, 3> R;  
        float aSq = data[0] * data[0];   // q1^2
        float bSq = data[1] * data[1];   // q2^2  
        float cSq = data[2] * data[2];   // q3^2
        float dSq = data[3] * data[3];   // q4^2
		
        R.data[0][0] = aSq + bSq - cSq - dSq;                                // q1^2 + q2^2 - q3^2 - q4^2
        R.data[0][1] = 2.0f * (data[1] * data[2] - data[0] * data[3]);       // 2*q2*q3 - 2*q1*q4
        R.data[0][2] = 2.0f * (data[1] * data[3] + data[0] * data[2]);       // 2*q2*q4 + 2*q1*q3 
        R.data[1][0] = 2.0f * (data[1] * data[2] + data[0] * data[3]);       // 2*q2*q3 + 2*q1*q4
        R.data[1][1] = aSq - bSq + cSq - dSq;                                // q1^2 - q2^2 + q3^2 - q4^2
        R.data[1][2] = 2.0f * (data[2] * data[3] - data[0] * data[1]);       // 2*q3*q4 - 2*q1*q2
        R.data[2][0] = 2.0f * (data[1] * data[3] - data[0] * data[2]);       // 2*q2*q4 - 2*q1*q3 
        R.data[2][1] = 2.0f * (data[2] * data[3] + data[0] * data[1]);       // 2*q3*q4 + 2*q1*q2
        R.data[2][2] = aSq - bSq - cSq + dSq;                                // q1^2 - q2^2 - q3^2 +zx q4^2
        return R;   
    }  
	*/
	
	
	
	/* all input data is ready, run controller itself */

	//  对齐Z轴  就等于  roll 横滚  pitch 俯仰 角度已 匹配
	//  取两个矩阵中的Z轴向量，即YAW-axis。
	/* try to move thrust vector shortest way, because yaw response is slower than roll/pitch 约2倍 */
	// 应该知道旋转矩阵是按照XYZ来进行的
	//   Xb           Xa     R(0, 0) * Xa + R(0, 1) * Ya + R(0, 2) * Za
	//   Yb   =  R *  Ya  =  R(1, 0) * Xa + R(1, 1) * Ya + R(1, 2) * Za
	//   Zb           Za     R(2, 0) * Xa + R(2, 1) * Ya + R(2, 2) * Za
	
    // 所以 当前姿态 和 设置姿态 的Z轴 姿态矩阵(由标准状态 旋转到此状态 的 旋转矩阵) 为 旋转矩阵的 最后一列
	math::Vector<3> R_z(R(0, 2), R(1, 2), R(2, 2));              // 当前 Z轴 姿态矩阵
	math::Vector<3> R_sp_z(R_sp(0, 2), R_sp(1, 2), R_sp(2, 2));  // 目标 Z轴 姿态矩阵

	/* axis and sin(angle) of desired rotation */
	
	// 当前姿态的z轴和目标姿态的z轴的误差大小（即需要旋转的角度） 并旋转到b系（即先对齐Z轴）。
	//  R_z%R_sp_z叉积，还记得这个么?在mahony算法中已经出现过一次了，就是求取误差的，
	// 本来应该z轴相互重合的，如果不是0就作为误差项。然后再左乘旋转矩阵旋转到b系。
	
	
	// 得到旋转轴  ?????????????????????????????????????????????????????????????
	math::Vector<3> e_R = R.transposed() * (R_z % R_sp_z);//得到对齐Z轴的旋转轴
	//                     当前姿态转换到 机体系  右乘 Z轴 反馈值与期望值 误差项 得到 从当前Z轴姿态转换到期望Z轴姿态 的 旋转轴
    /*
	Matrix3<T> Matrix3<T>::transposed(void) const  
     {  
    return Matrix3<T>(Vector3<T>(a.x, b.x, c.x),  
                      Vector3<T>(a.y, b.y, c.y),  
                      Vector3<T>(a.z, b.z, c.z));  
     }  
	*/
	/*
	  然后捏：计算姿态角度误差（姿态误差），一个数学知识背景：
	  由公式|a×b|=︱a︱︱b︱sinθ， 
	  点积 a•b=︱a︱︱b︱cosθ，这里的R_z和R_sp_z都是单位向量，模值为1，
	  因此误差向量
	  e_R（a×b叉积就是误差）的模就是sinθ，
	  点积就是cosθ。
	*/
	
	/* calculate angle error */
	float e_R_z_sin = e_R.length();  //叉积结果的模 即为sinθ 计算 旋转轴角度 正弦值
	float e_R_z_cos = R_z * R_sp_z;  //点积结果即为cosθ 计算 旋转轴角度 余弦值
 
	/* calculate weight for yaw control  根据机身平稳程度(横滚角和俯仰角大小) 设定偏航控制 权重*/
	float yaw_w = R_sp(2, 2) * R_sp(2, 2);	
	// R(2, 2) = cos(CetX)*cos(CetY) 如果Z轴对齐的话 即 roll pitch 无偏差角度即飞机机身比较平稳，偏航控制权重可以稍大R(2, 2)=1,
	// 如果 Z轴未对齐 即 roll pitch  有偏差 ，飞机机身不平稳 ， 偏航控制权重则不可过大，否者可能翻车
	//  这个权重纯粹是因为如果不转动roll-pitch的话那应该是1，而如果转动的话，那个权重会平方倍衰减 （来自MR的解释）。

	

	/* calculate rotation matrix after roll/pitch only rotation */
	math::Matrix<3, 3> R_rp;// 计算 从原状态转换到目标状态 的 旋转矩阵 
// 因为多轴的yaw响应一般比roll/pitch慢了接近一倍，因此将两者解耦(需要理解解耦的目的)，先补偿roll-pitch的变化，计算R_rp。


////////小角度偏转/////////////////////////////////
	if (e_R_z_sin > 0.0f) { //  存在误差 且 旋转角度小于 90度
		/* get axis-angle representation */
		float e_R_z_angle = atan2f(e_R_z_sin, e_R_z_cos);// 通过反正切求出该误差角度值  cet

		// 旋转轴单位向量
		math::Vector<3> e_R_z_axis = e_R / e_R_z_sin; 
		// 然后归一化e_R_z_axis(e_R /e_R_z_sin该步计算主要就是利用e_R_z_sin=e_R.length()
		// a = A / |A|  向量除以向量的模 即为 归一化  成 单位 误差 向量
		
        //很大的用途，下面的R_rp求取公式就是利用欧拉角计算的。  
		e_R = e_R_z_axis * e_R_z_angle; //  单位误差向量乘以角度值  用角度量表示 
	    // 旋转向量: 长度为 theta角大小

		//罗德里格旋转公式   http://www.amovauto.com/forum.php?mod=viewthread&tid=136
		
		/* cross product matrix for e_R_axis  叉乘算子  将叉乘通过反对称阵转化成点乘  */
		math::Matrix<3, 3> e_R_cp;
		e_R_cp.zero();
		e_R_cp(0, 1) = -e_R_z_axis(2);
		e_R_cp(0, 2) = e_R_z_axis(1);
		e_R_cp(1, 0) = e_R_z_axis(2);
		e_R_cp(1, 2) = -e_R_z_axis(0);
		e_R_cp(2, 0) = -e_R_z_axis(1);
		e_R_cp(2, 1) = e_R_z_axis(0);

		/* e_R_cp 反对称阵   姿态误差矩阵  叉乘算子  将叉乘通过反对称阵转化成点乘 
		  向量 k =[kx , ky, kz]  = [W1, W2, W3]
		  叉乘 v =[vx , vy, vz]
		    k % v = [ ky*vz - kz*vy, kz*vx - kx*vz, kx*vy - ky*vx]
			
			      = |0  -kz  ky|     |vx|    
				    |kz  0  -kx|  *  |vy|   变成了点乘
				   |-ky  kx  0 |     |vz|
				   
			      = 
		             0   -W3   W2    vx
		             W3   0   -W1  * vy
		            -W2   W1   0     vz
		*/	
       /////由旋转轴 和 旋转角度 的到 状态变换的 旋转矩阵  /////////////////////////////////////////////////////		
		/* rotation matrix for roll/pitch only rotation  罗德里格旋转公式 */
		//  V' = V + (1-cos )* K 叉乘 （K 叉乘 V） + sin * K 叉乘 V 
		//     = V + (1-cos )* K_cp	*（K_cp * V） + sin * K_cp *  V 
		//     = (单位阵 + sin * K_cp  + K_cp	* K_cp * (1-cos ) )
		//     = R_rp * V
		R_rp = R * (_I + e_R_cp * e_R_z_sin + e_R_cp * e_R_cp * (1.0f - e_R_z_cos));

	} else {
		/* zero roll/pitch rotation */
		R_rp = R;  // 如果 没有误差 ，即和之前测量的  的 状态 旋转矩阵一样
	}

	
// 对齐X轴 ，这样以保证能够将当前机体坐标系迅速地旋转到目标坐标系，
/////////////Z轴对齐后只需要再对齐一个轴即可完成姿态转换/////////////////////////////
	/* R_rp and R_sp has the same Z axis, calculate yaw error */
	math::Vector<3> R_sp_x(R_sp(0, 0), R_sp(1, 0), R_sp(2, 0));// x轴 姿态矩阵 设定值
	math::Vector<3> R_rp_x(R_rp(0, 0), R_rp(1, 0), R_rp(2, 0));// 对齐z轴后 的 当前X轴姿态矩阵
	// 叉积和点积求出误差角度的正弦和余弦，再反正切求出角度
	e_R(2) = atan2f((R_rp_x % R_sp_x) * R_sp_z, R_rp_x * R_sp_x) * yaw_w;
	
//////大角度旋转 //////////////////////////////////////////////////////////
	if (e_R_z_cos < 0.0f) {
		/* for large thrust vector rotations use another rotation method:
		
	// 大的推力向量:直接计算 R->R_sp 的旋转向量(轴、角)
		 * calculate angle and axis for R -> R_sp rotation directly */
		math::Quaternion q_error;
		q_error.from_dcm(R.transposed() * R_sp); // 四元数组成为旋转轴 转过的角度  
		
		// 四元数实部为 cos(theta)  虚部为 u_x*sin(theta) u_y*sin  u_z*sin
		math::Vector<3> e_R_d = q_error(0) >= 0.0f ? q_error.imag()  * 2.0f : -q_error.imag() * 2.0f;
       
	   // 然后对其进行归一化处理；最后2行是先求出互补系数，再通过互补方式求取e_R。
		/* use fusion of Z axis based rotation and direct rotation */
		// 使用基于Z轴的旋转和直接旋转的融合
		float direct_w = e_R_z_cos * e_R_z_cos * yaw_w;
		e_R = e_R * (1.0f - direct_w) + e_R_d * direct_w;
	}
	
////整体 P控制 ////////////////////////////////////////////////////
	/* calculate angular rates setpoint */
	_rates_sp = _params.att_p.emult(e_R);    // P控制   att_p * e_R 比例增益*误差 结果为角速度 赋值给_rates_sp角速度变量
  /*
   Matrix<Type, M, N> emult(const Matrix<Type, M,N> &other) const   
    {   
       Matrix<Type, M, N> res;   
        constMatrix<Type, M, N> &self = *this;   
        for (size_ti = 0; i < M; i++) {   
            for(size_t j = 0; j < N; j++) {   
               res(i , j) = self(i, j)*other(i, j);  //每个元素乘以  比例增益
            }   
        }   
        return res;  //  赋值给_rates_sp角速度变量
  }
  */
	
		
	/* limit rates 对角速度进制限制 */
	for (int i = 0; i < 3; i++) {
		// 自稳模式下限制
		if ((_v_control_mode.flag_control_velocity_enabled || _v_control_mode.flag_control_auto_enabled) &&
		    !_v_control_mode.flag_control_manual_enabled) {				
			_rates_sp(i) = math::constrain(_rates_sp(i), -_params.auto_rate_max(i), _params.auto_rate_max(i));
        // 多旋翼控制模式下 限制
		} else {
			_rates_sp(i) = math::constrain(_rates_sp(i), -_params.mc_rate_max(i), _params.mc_rate_max(i));
		}
	}

////YAW 偏航角度加入前馈////////////////
	/* feed forward yaw setpoint rate 由于偏航响应较慢，加入前馈控制 */
	_rates_sp(2) += _v_att_sp.yaw_sp_move_rate * yaw_w * _params.yaw_ff; // 偏航前馈

	/* weather-vane mode, dampen yaw rate 风向标模式 减轻偏航控制率*/
	if ((_v_control_mode.flag_control_velocity_enabled || _v_control_mode.flag_control_auto_enabled) &&
	    _v_att_sp.disable_mc_yaw_control == true && !_v_control_mode.flag_control_manual_enabled) {
			
		float wv_yaw_rate_max = _params.auto_rate_max(2) * _params.vtol_wv_yaw_rate_scale;
		_rates_sp(2) = math::constrain(_rates_sp(2), -wv_yaw_rate_max, wv_yaw_rate_max);
		// prevent integrator winding up in weathervane mode
		_rates_int(2) = 0.0f;
	}
}

/*
 * Throttle PID attenuation
 * Function visualization available here https://www.desmos.com/calculator/gn4mfoddje
 * Input: 'tpa_breakpoint', 'tpa_rate', '_thrust_sp'
 * Output: 'pidAttenuationPerAxis' vector
 */
math::Vector<3>
MulticopterAttitudeControl::pid_attenuations(float tpa_breakpoint, float tpa_rate)
{
	/* throttle pid attenuation factor */
	float tpa = 1.0f - tpa_rate * (fabsf(_v_rates_sp.thrust) - tpa_breakpoint) / (1.0f - tpa_breakpoint);
	tpa = fmaxf(TPA_RATE_LOWER_LIMIT, fminf(1.0f, tpa));

	math::Vector<3> pidAttenuationPerAxis;
	pidAttenuationPerAxis(AXIS_INDEX_ROLL) = tpa;
	pidAttenuationPerAxis(AXIS_INDEX_PITCH) = tpa;
	pidAttenuationPerAxis(AXIS_INDEX_YAW) = 1.0;

	return pidAttenuationPerAxis;
}

/*
 * 姿态角速度控制器                         Attitude rates controller.  
 * 输入：角速度设定值 向量 和 油门设置 向量 Input: '_rates_sp' vector, '_thrust_sp'
 * 输出：姿态控制向量                       Output: '_att_control' vector
 */
void
MulticopterAttitudeControl::control_attitude_rates(float dt)
{
	/* reset integral if disarmed  上锁了 */
	if (!_armed.armed || !_vehicle_status.is_rotary_wing) {
		_rates_int.zero(); //角速度设定值 赋值为0
	}

	/* get transformation matrix from sensor/board to body frame */
	get_rot_matrix((enum Rotation)_params.board_rotation, &_board_rotation);

	/* fine tune the rotation */
	math::Matrix<3, 3> board_rotation_offset;
	board_rotation_offset.from_euler(M_DEG_TO_RAD_F * _params.board_offset[0],
					 M_DEG_TO_RAD_F * _params.board_offset[1],
					 M_DEG_TO_RAD_F * _params.board_offset[2]);
	_board_rotation = board_rotation_offset * _board_rotation;

	// get the raw gyro data and correct for thermal errors
	math::Vector<3> rates;

// 获取当前 机身 姿态角速度 
	if (_selected_gyro == 0) {
		rates(0) = (_sensor_gyro.x - _sensor_correction.gyro_offset_0[0]) * _sensor_correction.gyro_scale_0[0];
		rates(1) = (_sensor_gyro.y - _sensor_correction.gyro_offset_0[1]) * _sensor_correction.gyro_scale_0[1];
		rates(2) = (_sensor_gyro.z - _sensor_correction.gyro_offset_0[2]) * _sensor_correction.gyro_scale_0[2];

	} else if (_selected_gyro == 1) {
		rates(0) = (_sensor_gyro.x - _sensor_correction.gyro_offset_1[0]) * _sensor_correction.gyro_scale_1[0];
		rates(1) = (_sensor_gyro.y - _sensor_correction.gyro_offset_1[1]) * _sensor_correction.gyro_scale_1[1];
		rates(2) = (_sensor_gyro.z - _sensor_correction.gyro_offset_1[2]) * _sensor_correction.gyro_scale_1[2];

	} else if (_selected_gyro == 2) {
		rates(0) = (_sensor_gyro.x - _sensor_correction.gyro_offset_2[0]) * _sensor_correction.gyro_scale_2[0];
		rates(1) = (_sensor_gyro.y - _sensor_correction.gyro_offset_2[1]) * _sensor_correction.gyro_scale_2[1];
		rates(2) = (_sensor_gyro.z - _sensor_correction.gyro_offset_2[2]) * _sensor_correction.gyro_scale_2[2];

	} else {
		rates(0) = _sensor_gyro.x;
		rates(1) = _sensor_gyro.y;
		rates(2) = _sensor_gyro.z;
	}

	// rotate corrected measurements from sensor to body frame
	rates = _board_rotation * rates;

	// correct for in-run bias errors
	rates(0) -= _ctrl_state.roll_rate_bias;
	rates(1) -= _ctrl_state.pitch_rate_bias;
	rates(2) -= _ctrl_state.yaw_rate_bias;

 // P I D 增益 放大到 油门设定值
	math::Vector<3> rates_p_scaled = _params.rate_p.emult(pid_attenuations(_params.tpa_breakpoint_p, _params.tpa_rate_p));
	math::Vector<3> rates_i_scaled = _params.rate_i.emult(pid_attenuations(_params.tpa_breakpoint_i, _params.tpa_rate_i));
	math::Vector<3> rates_d_scaled = _params.rate_d.emult(pid_attenuations(_params.tpa_breakpoint_d, _params.tpa_rate_d));

	/* angular rates error */
	math::Vector<3> rates_err = _rates_sp - rates; // 姿态角速度误差 //目标姿态-当前姿态 

///////////// P I(分离的I) D 控制器 + 前馈
	_att_control =    rates_p_scaled.emult(rates_err) // 比例增益 * 误差项  得到比例项
                	+ _rates_int                      //积分项 使用积分分离 当误差累计过大 则不进行积分控制
					+ rates_d_scaled.emult(_rates_prev - rates) / dt  // 直接使用 陀螺仪角速度 前后两次差值对时间导数 得到微分项
					+ _params.rate_ff.emult(_rates_sp);               // 前馈项   前馈增益直接乘以 设定值
/*标准的PID微分项D=Kd×(e(k)−e(k−1))，但是这里因为角度的微分就是角速度，而陀螺仪可以直接测出角速度。此处没有将微分项作为偏差的差可能是有此考虑，直接使用D=kd×ΔGyro/t可以增强稳定性。
*/


	_rates_sp_prev = _rates_sp;// 上次设定值
	_rates_prev = rates;       // 上次反馈值

///////////////// 积分抗饱和设置  积分控制分离 //////////////////////
/*
TPA的作用

TPA在此处使用是考虑到了抗积分饱和。

当系统存在一个方向的误差时，由于积分的累加作用会使控制量一直增大，
可能会使控制量达到执行器的执行阈值，如果此时误差的方向发生改变，控制量会逐渐减小，
控制量也会退出饱和区，执行器也会在阈值内执行；如果此时误差方向还是没有改变，
控制量会继续增大但是执行器会一直保持在阈值，此时控制量就进入了饱和区。
进入饱和区越深，退出饱和区时间就会越长，在饱和区时执行器会一直在阈值位置，
如果误差发生反向，执行器不会立刻有反应，控制量会慢慢减小，
等执行器推出饱和区才会有反应。这样就会使控制的动态响应变差，控制性能变差。 
抗积分饱和算法 
若 u(k-1)>Umax，只累加负的误差；若u(k-1)
*/
	/* update integral only if motors are providing enough thrust to be effective */
	if (_thrust_sp > MIN_TAKEOFF_THRUST) {//油门设定值 大于起飞 油门值
	
		for (int i = AXIS_INDEX_ROLL; i < AXIS_COUNT; i++) {
			// Check for positive control saturation
			bool positive_saturation = //正向饱和   过大
				((i == AXIS_INDEX_ROLL) && _saturation_status.flags.roll_pos) ||
				((i == AXIS_INDEX_PITCH) && _saturation_status.flags.pitch_pos) ||
				((i == AXIS_INDEX_YAW) && _saturation_status.flags.yaw_pos);

			// Check for negative control saturation
			bool negative_saturation =//反向饱和   过小
				((i == AXIS_INDEX_ROLL) && _saturation_status.flags.roll_neg) ||
				((i == AXIS_INDEX_PITCH) && _saturation_status.flags.pitch_neg) ||
				((i == AXIS_INDEX_YAW) && _saturation_status.flags.yaw_neg);

			// prevent further positive control saturation
			if (positive_saturation) {
				rates_err(i) = math::min(rates_err(i), 0.0f);//过大 设置为0 

			}

			// prevent further negative control saturation
			if (negative_saturation) {
				rates_err(i) = math::max(rates_err(i), 0.0f);//过小设置为0

			}
// 积分器
			// Perform the integration using a first order method and do not propaate the result if out of range or invalid
			float rate_i = _rates_int(i) + _params.rate_i(i) * rates_err(i) * dt;// 积分误差 累计和
			//            _rates_int(i) 前过程 的 积分项 + 原始积分增益系数 * 误差 * 时间间隔        (连续 变 离散 )
			// 积分项在有效范围内 才 更新积分项
			if (PX4_ISFINITE(rate_i) && rate_i > -_params.rate_int_lim(i) && rate_i < _params.rate_int_lim(i)) {
				_rates_int(i) = rate_i;

			}
		}
	}

	/* explicitly limit the integrator state  积分项 限制*/
	for (int i = AXIS_INDEX_ROLL; i < AXIS_COUNT; i++) {
		_rates_int(i) = math::constrain(_rates_int(i), -_params.rate_int_lim(i), _params.rate_int_lim(i));

	}
}

void
MulticopterAttitudeControl::task_main_trampoline(int argc, char *argv[])
{
	mc_att_control::g_control->task_main();//跳转到主任务
}

void
MulticopterAttitudeControl::task_main()
{

	/*
	 * do subscriptions  订阅话题 
	 
	 首先还是需要通过IPC模型uORB进行订阅所需要的数据。
	 需要注意的一个细节就是在该算法处理过程中的有效数据的用途问题，
	 最后处理过的数据最后又被改进程自己订阅了，然后再处理，
	 再订阅，一直处于循环状态，这就是所谓的PID反馈控制器吧，
	 最终达到所需求的控制效果，达到控制效果以后就把一系列的控制量置0（类似于idle），
	 该任务一直在运行，随启动脚本启动的。
	 
	 */
	_v_att_sp_sub = orb_subscribe(ORB_ID(vehicle_attitude_setpoint));  //姿态角度设定值话题
	_v_rates_sp_sub = orb_subscribe(ORB_ID(vehicle_rates_setpoint));   //姿态角速度设定值话题
	_ctrl_state_sub = orb_subscribe(ORB_ID(control_state));            //反馈值  机体控制状态  当前姿态角度四元数  姿态角速度 风速
	_v_control_mode_sub = orb_subscribe(ORB_ID(vehicle_control_mode)); //控制模式  垂直起降等
	_params_sub = orb_subscribe(ORB_ID(parameter_update));             //控制器参数等参数更新了吗
	_manual_control_sp_sub = orb_subscribe(ORB_ID(manual_control_setpoint));//手动控制设定值
	_armed_sub = orb_subscribe(ORB_ID(actuator_armed));                     //执行器解锁
	_vehicle_status_sub = orb_subscribe(ORB_ID(vehicle_status));            //飞行器状态
	_motor_limits_sub = orb_subscribe(ORB_ID(multirotor_motor_limits));     //多旋翼电机限制
	_battery_status_sub = orb_subscribe(ORB_ID(battery_status));            //电池状态
    //订阅 多个陀螺仪数据
	_gyro_count = math::min(orb_group_count(ORB_ID(sensor_gyro)), MAX_GYRO_COUNT);//得到 发布陀螺仪传感器数据的话题个数
	for (unsigned s = 0; s < _gyro_count; s++) {
		_sensor_gyro_sub[s] = orb_subscribe_multi(ORB_ID(sensor_gyro), s);//订阅多个 陀螺仪传感器  角速度数据的话题
	}
    //传感器校正后的数据   
	//corrected_rate = raw_rate * gyro_scale + gyro_offset
    //corrected_accel = raw_accel * accel_scale + accel_offset
	_sensor_correction_sub = orb_subscribe(ORB_ID(sensor_correction));

	/* initialize parameters cache */
	parameters_update();
	// 私有数据（private）参数初始化，
	//比如roll、pitch、yaw以及与它们对应的PID参数。注意区分_params_handles和_params这两种数据结构（struct类型），比如roll、pitch、yaw以及与它们对应的PID参数。注意区分_params_handles和_params这两种数据结构（struct类型）

	/* wakeup source: gyro data from sensor selected by the sensor app */
	px4_pollfd_struct_t poll_fds = {};
	poll_fds.fd = _sensor_gyro_sub[_selected_gyro];// 角速度数据 话题 
	poll_fds.events = POLLIN;

	while (!_task_should_exit) {

	//阻塞等待数据
		/* wait for up to 100ms for data */
		int pret = px4_poll(&poll_fds, 1, 100);//配置阻塞时间100ms（uORB模型的函数API）。

		/* timed out - periodic check for _task_should_exit */
		if (pret == 0) {
			continue; //超时 poll失败，直接使用关键词continue从头开始运行（注意while和continue的组合使用）
		}

		/* this is undesirable but not much we can do - might want to flag unhappy status */
		if (pret < 0) {//发生错误
			warn("mc att ctrl: poll error %d, %d", pret, errno);
			/* sleep a bit before next try */
			usleep(100000);//线程级睡眠函数，使当前线程挂起
			continue;
		}

		perf_begin(_loop_perf);//是一个空函数，带perf开头的都是空函数，它的作用主要是“Empty function calls for ros compatibility”。

		/* run controller on gyro changes 
		终于到了姿态控制器了，兴奋不？别只顾着兴奋了，好好理解一下。
		尤其是下面的几个*poll函数，特别重要，后期算法中的很多数据都是通过这个几个*poll()函数获取的，
		也是uORB模型，不理解这个后去会很晕的，别说没提醒啊；代码中没有一点冗余的部分，
		每一个函数、每一行都是其意义所在。
		*/
		
		//注意  pollevent_t events;  /* The input event flags */ 
		//      pollevent_t revents; /* The output event flags */
		if (poll_fds.revents & POLLIN) {//
			static uint64_t last_run = 0;
			float dt = (hrt_absolute_time() - last_run) / 1000000.0f;
			last_run = hrt_absolute_time();//记录表本次时间

			// 时间间隔 限幅
			/* guard against too small (< 2ms) and too large (> 20ms) dt's */
			if (dt < 0.002f) {
				dt = 0.002f;

			} else if (dt > 0.02f) {
				dt = 0.02f;
			}

			/* copy gyro data 从话题上复制数据 话题id   订阅话题返回的句柄  存储数据的变量*/
			orb_copy(ORB_ID(sensor_gyro), _sensor_gyro_sub[_selected_gyro], &_sensor_gyro);

			/* check for updates in other topics 检查其他话题数据是否更新 并且更新数据 */
			parameter_update_poll();       // 参数是否更新
			vehicle_control_mode_poll();   // 控制模式
			arming_status_poll();          // 执行器解锁 否
			vehicle_manual_poll();         // 手动控制设定值
			vehicle_status_poll();         // 飞行器状态
			vehicle_motor_limits_poll();   // 飞行器电机输出限制
			battery_status_poll();         // 电池状态
			control_state_poll();          // 机体控制状态      当前姿态角度 空速 姿态角速度   
			sensor_correction_poll();      // 传感器校正后的数据

			/* Check if we are in rattitude mode and the pilot is above the threshold on pitch
			 * or roll (yaw can rotate 360 in normal att control).  If both are true don't
			 * even bother running the attitude controllers 
			 （新的飞行模式，角速度模式，没有角度控制）
			 */
			// 确定是否进行角度控制
			/*然后：飞行模式判断是否是MAIN_STATE_RATTITUD模式，该模式是一种新的飞行模式，只控制角速度，不控制角度，俗称半自稳模式（小舵量自稳大舵量手动），主要用在setpoint中，航点飞行。根据介绍，这个模式只有在pitch和roll都设置为Rattitude模式时才有意义，如果yaw也设置了该模式，那么就会自动被手动模式替代了。所以代码中只做了x、y阈值的检测。
			*/
			if (_v_control_mode.flag_control_rattitude_enabled) {// rattitude 模式下
			     // 并且 横滚和俯仰超过限制 
				if (fabsf(_manual_control_sp.y) > _params.rattitude_thres || // 左右运动(roll 横滚角设置值)  超过限制
				    fabsf(_manual_control_sp.x) > _params.rattitude_thres) { // 前进后退(pitch 俯仰角设定值) 超过限制
					_v_control_mode.flag_control_attitude_enabled = false;   // 则不进行角度控制
				}
			}
////////////////////////////// 角度控制/////////////////////////////////////////////////////
			if (_v_control_mode.flag_control_attitude_enabled) {//角度控制被使能

				if (_ts_opt_recovery == nullptr) {//姿态控制器 已开启
					// the  tailsitter recovery instance has not been created, thus, the vehicle
					// is not a tailsitter, do normal attitude control
					control_attitude(dt);//进行角度控制

				} else {//姿态控制器未开启
					
					vehicle_attitude_setpoint_poll();//更新 姿态角度设置
					_thrust_sp = _v_att_sp.thrust;   //油门设定值
					math::Quaternion q(_ctrl_state.q[0], _ctrl_state.q[1], _ctrl_state.q[2], _ctrl_state.q[3]);//当前 姿态
					math::Quaternion q_sp(&_v_att_sp.q_d[0]);// 期望 的 姿态
					_ts_opt_recovery->setAttGains(_params.att_p, _params.yaw_ff);//比例控制增益
					
				//////// 比例控制器输出//////
					_ts_opt_recovery->calcOptimalRates(q, q_sp, _v_att_sp.yaw_sp_move_rate, _rates_sp);//比例控制器输出 最优速率

					/* limit rates 角速度 限制*/
					for (int i = 0; i < 3; i++) {
						_rates_sp(i) = math::constrain(_rates_sp(i), -_params.mc_rate_max(i), _params.mc_rate_max(i));
					}
				}

				/* publish attitude rates setpoint */
				_v_rates_sp.roll = _rates_sp(0);  // 滚转角速度
				_v_rates_sp.pitch = _rates_sp(1); // 俯仰角速度
				_v_rates_sp.yaw = _rates_sp(2);   // 偏航角速度
				_v_rates_sp.thrust = _thrust_sp;  // 油门设定值
				_v_rates_sp.timestamp = hrt_absolute_time();//当前 时间

				if (_v_rates_sp_pub != nullptr) {//_rates_sp_id, 话题已广播 
					orb_publish(_rates_sp_id, _v_rates_sp_pub, &_v_rates_sp);//发布角速度 设定值

				} else if (_rates_sp_id) {// _rates_sp_id 话题还没有被广播告知
					_v_rates_sp_pub = orb_advertise(_rates_sp_id, &_v_rates_sp);//发布 话题数据前 需要先进行广播话题 
				}

				//}

			} else {//角度控制没有被使能
				/* attitude controller disabled, poll rates setpoint topic */
				if (_v_control_mode.flag_control_manual_enabled) {//人工控制使能
					/* manual rates control - ACRO mode */
					// 角度控制没有被使能 直接得到 手动设定值  的 角速度设定值 以及 油门设定值
					
					// 
					_rates_sp = math::Vector<3>(_manual_control_sp.y,    // roll  绕x轴 角度, 
					                            -_manual_control_sp.x,   // pitch 绕y轴 角度 与前进后退 相反 ,
								                _manual_control_sp.r	 // yaw   绕Z轴 角度 
												).emult(_params.acro_rate_max);
									
					_thrust_sp = math::min(_manual_control_sp.z, MANUAL_THROTTLE_MAX_MULTICOPTER);

					/* publish attitude rates setpoint  发布 角速度 油门 时间 数据*/
					_v_rates_sp.roll = _rates_sp(0);  // 滚转角速度
					_v_rates_sp.pitch = _rates_sp(1); // 俯仰角速度
					_v_rates_sp.yaw = _rates_sp(2);   // 偏航角速度
					_v_rates_sp.thrust = _thrust_sp;  // 油门设定值
					_v_rates_sp.timestamp = hrt_absolute_time();//当前 时间

					if (_v_rates_sp_pub != nullptr) {//发布角速度 设定值
						orb_publish(_rates_sp_id, _v_rates_sp_pub, &_v_rates_sp);

					} else if (_rates_sp_id) {// _rates_sp_id 话题还没有被广播告知
						_v_rates_sp_pub = orb_advertise(_rates_sp_id, &_v_rates_sp);//发布 话题数据前 需要先进行广播话题 
					}

				} else {//角度控制没有被使能 且人工控制也未使能  直接订阅 角速度 话题 获取角速度设定值
					/* attitude controller disabled, poll rates setpoint topic */
					vehicle_rates_setpoint_poll();   //获取角速度设定值话题上的数据
					_rates_sp(0) = _v_rates_sp.roll;    // 横滚角速度设定值
					_rates_sp(1) = _v_rates_sp.pitch;   // 俯仰角速度设定值
					_rates_sp(2) = _v_rates_sp.yaw;     // 偏航据速度设定值
					_thrust_sp = _v_rates_sp.thrust;    // 油门设置值
				}
			}
			
///////////////////////////////// 角速度 控制///////////////////////////////////////////////////
			if (_v_control_mode.flag_control_rates_enabled) {
				
				control_attitude_rates(dt);//进行角速度 PID 控制

				/* publish actuator controls 发布执行器电机控制量*/
				_actuators.control[0] = (PX4_ISFINITE(_att_control(0))) ? _att_control(0) : 0.0f; // 滚转 力矩 控制量
				_actuators.control[1] = (PX4_ISFINITE(_att_control(1))) ? _att_control(1) : 0.0f; // 俯仰 力矩 控制量
 				_actuators.control[2] = (PX4_ISFINITE(_att_control(2))) ? _att_control(2) : 0.0f; // 偏航 力矩 控制量
				_actuators.control[3] = (PX4_ISFINITE(_thrust_sp)) ? _thrust_sp : 0.0f;           // 油门 控制量
				_actuators.control[7] = _v_att_sp.landing_gear;// 着陆 
				_actuators.timestamp = hrt_absolute_time();//当前时间
				_actuators.timestamp_sample = _ctrl_state.timestamp;//控制状态时间

				/* scale effort by battery status 油门控制量 考虑电池状态 */
				if (_params.bat_scale_en && _battery_status.scale > 0.0f) {
					for (int i = 0; i < 4; i++) {
						_actuators.control[i] *= _battery_status.scale;
					}
				}

				// 控制器 状态  主要是积分项 的和 
				_controller_status.roll_rate_integ = _rates_int(0);    // 横滚 PID 控制积分项和
				_controller_status.pitch_rate_integ = _rates_int(1);   // 俯仰 PID 控制积分项和
				_controller_status.yaw_rate_integ = _rates_int(2);     // 偏航 PID 控制积分项和
				_controller_status.timestamp = hrt_absolute_time();

				if (!_actuators_0_circuit_breaker_enabled) { // 0组执行器 使能
					if (_actuators_0_pub != nullptr) { // 0组执行器 话题已被广播 
						orb_publish(_actuators_id, _actuators_0_pub, &_actuators);// 发布 0组执行器 控制量
						perf_end(_controller_latency_perf);

					} else if (_actuators_id) {// 0组执行器 话题未被广播 
						_actuators_0_pub = orb_advertise(_actuators_id, &_actuators);//广播 0组执行器 话题
					}

				}

				/* publish controller status 发布控制器状态信息  主要是积分项的值 */
				if (_controller_status_pub != nullptr) { // 控制器状态信息 话题已被广播
					orb_publish(ORB_ID(mc_att_ctrl_status), _controller_status_pub, &_controller_status); // 发布控制器状态信息

				} else {// 控制器状态信息 话题未被广播
					_controller_status_pub = orb_advertise(ORB_ID(mc_att_ctrl_status), &_controller_status);// 广播 控制器状态信息 话题
				}
			}

			if (_v_control_mode.flag_control_termination_enabled) { // 远程遥控使能
			
				if (!_vehicle_status.is_vtol) {  //垂直起降

					_rates_sp.zero();    //角速度设定值 归零
					_rates_int.zero();   //控制器积分项和 归零
					_thrust_sp = 0.0f;   //油门设定值 归零
					_att_control.zero(); //执行器控制量 归零
 

					/* publish actuator controls  发布执行器 控制量 */
					_actuators.control[0] = 0.0f;
					_actuators.control[1] = 0.0f;
					_actuators.control[2] = 0.0f;
					_actuators.control[3] = 0.0f;
					_actuators.timestamp = hrt_absolute_time();
					_actuators.timestamp_sample = _ctrl_state.timestamp;

					if (!_actuators_0_circuit_breaker_enabled) {
						if (_actuators_0_pub != nullptr) {

							orb_publish(_actuators_id, _actuators_0_pub, &_actuators);
							perf_end(_controller_latency_perf);

						} else if (_actuators_id) {
							_actuators_0_pub = orb_advertise(_actuators_id, &_actuators);
						}
					}

					
					/* publish controller status 发布控制器状态 主要是积分项 的和 */
					_controller_status.roll_rate_integ = _rates_int(0);
					_controller_status.pitch_rate_integ = _rates_int(1);
					_controller_status.yaw_rate_integ = _rates_int(2);
					_controller_status.timestamp = hrt_absolute_time();

					if (_controller_status_pub != nullptr) {
						orb_publish(ORB_ID(mc_att_ctrl_status), _controller_status_pub, &_controller_status);

					} else {
						_controller_status_pub = orb_advertise(ORB_ID(mc_att_ctrl_status), &_controller_status);
					}

					/* publish attitude rates setpoint  发布姿态角速度设定值 */
					_v_rates_sp.roll = _rates_sp(0);
					_v_rates_sp.pitch = _rates_sp(1);
					_v_rates_sp.yaw = _rates_sp(2);
					_v_rates_sp.thrust = _thrust_sp;
					_v_rates_sp.timestamp = hrt_absolute_time();

					if (_v_rates_sp_pub != nullptr) {
						orb_publish(_rates_sp_id, _v_rates_sp_pub, &_v_rates_sp);

					} else if (_rates_sp_id) {
						_v_rates_sp_pub = orb_advertise(_rates_sp_id, &_v_rates_sp);
					}
				}
			}
		}

		perf_end(_loop_perf);
	}

	_control_task = -1; //任务结束
}


int
MulticopterAttitudeControl::start()
{
	ASSERT(_control_task == -1);

	/* start the task */
//函数原型 
//px4_task_t px4_task_spawn_cmd(const char *name, int scheduler, int priority, int stack_size, px4_main_t entry,  char *const argv[]) 
	_control_task = px4_task_spawn_cmd("mc_att_control",//任务名
					   SCHED_DEFAULT,                   //默认调度
					   SCHED_PRIORITY_MAX - 5,          //优先级
					   1700,                            //堆栈大小
					   (px4_main_t)&MulticopterAttitudeControl::task_main_trampoline,//主任务入口函数  跳转
					   nullptr);//命令行参数  无

	if (_control_task < 0) {
		warn("task start failed");//启动任务失败
		return -errno;
	}

	return OK;
}



// 函数
int mc_att_control_main(int argc, char *argv[])
{
//命令行参数 数量 核对
	if (argc < 2) {
		warnx("usage: mc_att_control {start|stop|status}");
		return 1;
	}
//任务启动
	if (!strcmp(argv[1], "start")) {

		if (mc_att_control::g_control != nullptr) {//不为空 已启动
			warnx("already running");
			return 1;
		}

		mc_att_control::g_control = new MulticopterAttitudeControl;
		// new关键词应该不陌生吧，类似于C语言中的malloc，对变量进行内存分配的，即对姿态控制过程中使用到的变量赋初值。

		if (mc_att_control::g_control == nullptr) {//启动失败
			warnx("alloc failed");
			return 1;
		}

		if (OK != mc_att_control::g_control->start()) {//启动跳转到 start()  启动是否是成功
			delete mc_att_control::g_control;
			mc_att_control::g_control = nullptr;
			warnx("start failed");
			return 1;
		}

		return 0;
	}
//任务结束
	if (!strcmp(argv[1], "stop")) {
		if (mc_att_control::g_control == nullptr) {
			warnx("not running");
			return 1;
		}

		delete mc_att_control::g_control;
		mc_att_control::g_control = nullptr;
		return 0;
	}
//任务状态信息
	if (!strcmp(argv[1], "status")) {
		if (mc_att_control::g_control) {
			warnx("running");
			return 0;

		} else {
			warnx("not running");
			return 1;
		}
	}
//其他 指令
	warnx("unrecognized command");
	return 1;
}
