/* 

 *  Code commit statistics can be found here: https://github.com/ArduPilot/ardupilot/graphs/contributors
 *  Wiki: http://copter.ardupilot.org/
 *  Requires modified version of Arduino, which can be found here: http://ardupilot.com/downloads/?category=6
    在主循环执行过程中（比如Pixhawk的任务调度周期2.5ms，400Hz；APM2.x为10ms，100Hz），每一个周期，程序会按下述步骤执行：
首先，高层次文件flight_mode.cpp中的update_flight_mode()函数被调用。通过检查control_mode变量，前飞行器的飞行模式（使用变量），
然后执行相应飞行模式下的_run()函数（如自稳模式的stabilize_run，返航模式（RTL）的rtl_run等）。
执行_run的结果是，系统将会找到与飞行模式相对应的命名为control_.cpp飞行控制文件
（比如：control_stabilize.cpp，control_rtl.cpp等）。
_run函数负责将用户的输入（从g.rc_1.control_in，g.rc_2.control_in等读入）
转换为此时飞行模式下的倾斜角（lean angle）、滚转速率（rotation rate）、爬升率（climb rate）等（也就是设置目标值roll\pitch\yaw\throttle）。举个例子：AltHold(定高，altitude hold)模式中将用户的滚转和俯仰输入转换为倾斜角（单位：角度/°），将偏航输入转换为滚转速率（单位：°/s），
将油门输入转换为爬升率（单位：cm/s）。

_run函数最后还必须要完成的就是将预期角度、速率等参数传送给姿态控制和/或方位控制库（它们都放在AC_AttitiudeControl文件夹内）。

AC_AttitiudeControl库提供了5种可能的方法来调整飞行器的姿态，下面来说明最通用的三种方法：
     1)angle_ef_roll_pitch_rate_ef_yaw():该函数需要一个地轴系坐标下滚转和偏航角度，一个地轴系坐标下的偏航速率。
	 例如：传递给该函数三个参数分别为，roll = -1000， pitch = -1500， yaw = 500代表飞行器此时向左倾斜10°，低头15°，向右偏航速率为5°/s。
     2) angle_ef_roll_pitch_yaw():该函数接受地轴系下的滚转、俯仰和偏航角。
	 和上面的函数类似，不过参数yaw = 500代表飞行器北偏东5°
     3) rate_bf_roll_pitch_yaw():该函数接受一个体轴系下的滚转、俯仰和偏航角速率（°/s）。
	 例如：传递给该函数三个参数：roll = -1000， pitch = -1500， yaw = 500代表飞行器此时左倾速率10°/s，低头速率15°/s，绕Z轴速率为5°/s。
当上述这些函数调用之后，就会接着调用AC_AttitudeControl::rate_controller_run()函数，将上面所列举的函数的输出转化为滚转、偏航和俯仰输入，并使用set_roll,set_pitch,set_yaw 和 set_throttle方法将这些输入发送给AP_Motors库。
 
另外，
AC_PosControl库用来控制飞行器的3D方位。不过通常只用来调整比较简单的Z轴方向（如姿态控制），这是因为许多需要复杂3D方位调整的飞行模式(例如悬停Loiter)使用的是“AC_WPNav库”。总之，AC_PosControl库中常用的方法有：
                1) set_alt_target_from_climb_rate():将爬升率（cm/s）作为参数，用来更新一个需要调整的相对高度目标。
                2) set_pos_target():接受一个以系统中的home位置作为参考点的3D位置矢量（单位：cm）。
如果调用了AC_PosControl中的任何一个方法，那么在该飞行模式下就必须调用函数AC_PosControl::update_z_controller()。这样的话，就可以启用Z轴的方位控制PID循环，并向AP_Motors库发送低级别的油门信息。同样，如果调用了xy轴的函数，那就就必须调用AC_PosControl::update_xy_controller()函数。
AP_Motors库含有“电机混合模式”代码。这些代码负责将从AC_AttitudeControl和AC_PosControl库发送过来的滚转、俯仰、偏航角度和油门值信息转换为电机的相对输出值（例如：PWM值）。因此，这样高级别的库就必须要使用如下函数：
                1) set_roll(),set_pitch(),set_yaw()：接受在[-4500,4500]角度范围内的滚转、俯仰和偏航角。这些参数不是期望角度或者速率，更准确的讲，它仅仅是一个数值。例如，set_roll(-4500)将代表飞行器尽可能快的向左滚转。
                2) set_throttle():接受一个范围在[0,1000]的相对油门值。0代表电机关闭，1000代表满油门状态。
虽然对于不同飞行器构型（如四旋翼，Y6，传统直升机等）的控制代码中有许多不同的类，但这些类中都有一个相同的函数output_armed，
负责将这些滚转、俯仰、偏航和油门值转换为PWM类型输入值。这转换的过程中，会应用到stability patch，
用来控制由于飞行器构型限制所引起的轴系的优先级问题（例如四旋翼的四个电机不可能在做最大速度滚转时四个电机的油门同时达到最大，
因为必须一部分电机输出小于另一部分才能引起滚转）。在执行函数output_armed的最后，还将调用hal.rcout->write()，
把期望PWM值传递给AP_HAL层。
AP_HAL库（硬件抽象层）提供了针对所有飞控板统一的接口。实际控制中，hal.rc_out->write()函数
将接受到的来自于AP_Motors类中指定的PWM值，输出至飞控板对应的PWM端口（pin端）
。

 */

#include "Copter.h"

#define SCHED_TASK(func, rate_hz, max_time_micros) SCHED_TASK_CLASS(Copter, &copter, func, rate_hz, max_time_micros)

/*
  scheduler table for fast CPUs - all regular tasks apart from the fast_loop()
  should be listed here, along with how often they should be called (in hz)
  and the maximum time they are expected to take (in microseconds)
 */
const AP_Scheduler::Task Copter::scheduler_tasks[] = {
    SCHED_TASK(rc_loop,              100,    130),// pwm 信号输入与输出
    SCHED_TASK(throttle_loop,         50,     75),
    SCHED_TASK(update_GPS,            50,    200),// GPS 位置信息
#if OPTFLOW == ENABLED
    SCHED_TASK(update_optical_flow,  200,    160),// 光流信息
#endif
    SCHED_TASK(update_batt_compass,   10,    120),// 罗盘
    SCHED_TASK(read_aux_switches,     10,     50),// 开关信息
    SCHED_TASK(arm_motors_check,      10,     50),
    SCHED_TASK(auto_disarm_check,     10,     50),
    SCHED_TASK(auto_trim,             10,     75),
    SCHED_TASK(read_rangefinder,      20,    100),
    SCHED_TASK(update_proximity,     100,     50),
    SCHED_TASK(update_beacon,        400,     50),
    SCHED_TASK(update_visual_odom,   400,     50),
    SCHED_TASK(update_altitude,       10,    100),
    SCHED_TASK(run_nav_updates,       50,    100),
    SCHED_TASK(update_throttle_hover,100,     90),
    SCHED_TASK(three_hz_loop,          3,     75),
    SCHED_TASK(compass_accumulate,   100,    100),
    SCHED_TASK(barometer_accumulate,  50,     90),
#if PRECISION_LANDING == ENABLED
    SCHED_TASK(update_precland,      400,     50),
#endif
#if FRAME_CONFIG == HELI_FRAME
    SCHED_TASK(check_dynamic_flight,  50,     75),
#endif
    SCHED_TASK(fourhundred_hz_logging,400,    50),
    SCHED_TASK(update_notify,         50,     90),
    SCHED_TASK(one_hz_loop,            1,    100),
    SCHED_TASK(ekf_check,             10,     75),
    SCHED_TASK(gpsglitch_check,       10,     50),
    SCHED_TASK(landinggear_update,    10,     75),
    SCHED_TASK(lost_vehicle_check,    10,     50),
    SCHED_TASK(gcs_check_input,      400,    180),
    SCHED_TASK(gcs_send_heartbeat,     1,    110),
    SCHED_TASK(gcs_send_deferred,     50,    550),
    SCHED_TASK(gcs_data_stream_send,  50,    550),
    SCHED_TASK(update_mount,          50,     75),
    SCHED_TASK(update_trigger,        50,     75),
    SCHED_TASK(ten_hz_logging_loop,   10,    350),
    SCHED_TASK(twentyfive_hz_logging, 25,    110),
    SCHED_TASK(dataflash_periodic,    400,    300),
    SCHED_TASK(perf_update,           0.1,    75),
    SCHED_TASK(read_receiver_rssi,    10,     75),
    SCHED_TASK(rpm_update,            10,    200),
    SCHED_TASK(compass_cal_update,   100,    100),
    SCHED_TASK(accel_cal_update,      10,    100),
#if ADSB_ENABLED == ENABLED
    SCHED_TASK(avoidance_adsb_update, 10,    100),
#endif
#if ADVANCED_FAILSAFE == ENABLED
    SCHED_TASK(afs_fs_check,          10,    100),
#endif
    SCHED_TASK(terrain_update,        10,    100),
#if GRIPPER_ENABLED == ENABLED
    SCHED_TASK(gripper_update,        10,     75),
#endif
#ifdef USERHOOK_FASTLOOP
    SCHED_TASK(userhook_FastLoop,    100,     75),
#endif
#ifdef USERHOOK_50HZLOOP
    SCHED_TASK(userhook_50Hz,         50,     75),
#endif
#ifdef USERHOOK_MEDIUMLOOP
    SCHED_TASK(userhook_MediumLoop,   10,     75),
#endif
#ifdef USERHOOK_SLOWLOOP
    SCHED_TASK(userhook_SlowLoop,     3.3,    75),
#endif
#ifdef USERHOOK_SUPERSLOWLOOP
    SCHED_TASK(userhook_SuperSlowLoop, 1,   75),
#endif
    SCHED_TASK(button_update,          5,    100),
    SCHED_TASK(stats_update,           1,    100),
};


void Copter::setup() 
{
    cliSerial = hal.console;

    // Load the default values of variables listed in var_info[]s
    AP_Param::setup_sketch_defaults();

    // setup storage layout for copter
    StorageManager::set_layout_copter();

    init_ardupilot();

    // initialise the main loop scheduler
    scheduler.init(&scheduler_tasks[0], ARRAY_SIZE(scheduler_tasks));

    // setup initial performance counters
    perf_info_reset();
    fast_loopTimer = AP_HAL::micros();
}

/*
  try to accumulate a baro reading
 */
void Copter::barometer_accumulate(void)
{
    barometer.accumulate();
}

void Copter::perf_update(void)
{
    if (should_log(MASK_LOG_PM))
        Log_Write_Performance();
    if (scheduler.debug()) {
        gcs().send_text(MAV_SEVERITY_WARNING, "PERF: %u/%u %lu %lu",
                          (unsigned)perf_info_get_num_long_running(),
                          (unsigned)perf_info_get_num_loops(),
                          (unsigned long)perf_info_get_max_time(),
                          (unsigned long)perf_info_get_min_time());
    }
    perf_info_reset();
    pmTest1 = 0;
}

/*
  update AP_Stats
 */
void Copter::stats_update(void)
{
    g2.stats.update();
}


void Copter::loop()
{
    // wait for an INS sample
    ins.wait_for_sample();

    uint32_t timer = micros();// 当前时间 微妙

    // check loop time
    perf_info_check_loop_time(timer - fast_loopTimer);

    // used by PI Loops
    G_Dt                    = (float)(timer - fast_loopTimer) / 1000000.0f;
    fast_loopTimer          = timer;

    // for mainloop failure monitoring
    mainLoop_count++;//主循环计数  监控

    // Execute the fast loop
    // ---------------------
    fast_loop();//主循环

    // tell the scheduler one tick has passed
    scheduler.tick();

    // run all the tasks that are due to run. Note that we only
    // have to call this once per loop, as the tasks are scheduled
    // in multiples of the main loop tick. So if they don't run on
    // the first call to the scheduler they won't run on a later
    // call until scheduler.tick() is called again
    uint32_t time_available = (timer + MAIN_LOOP_MICROS) - micros();
    scheduler.run(time_available > MAIN_LOOP_MICROS ? 0u : time_available);
}


// Main loop - 400hz
void Copter::fast_loop()
{
    // update INS immediately to get current gyro data populated
    ins.update();
    
    // run low level rate controllers that only require IMU data
    attitude_control->rate_controller_run();

    // send outputs to the motors library immediately
    motors_output();

    // run EKF state estimator (expensive)
    // --------------------
    read_AHRS();// 姿态估计 方向余弦矩阵(DCM) 或者 扩展卡尔曼滤波(EKF)

#if FRAME_CONFIG == HELI_FRAME
    update_heli_control_dynamics();
#endif //HELI_FRAME

    // Inertial Nav  惯性导航
    // --------------------
    read_inertia();

    // check if ekf has reset target heading or position
    check_ekf_reset();

    // run the attitude controllers
    update_flight_mode();   //飞行器模式更新 flightmode.cpp

    // update home from EKF if necessary
    update_home_from_EKF();

    // check if we've landed or crashed   着陆 碰撞检测
    update_land_and_crash_detectors();

#if MOUNT == ENABLED
    // camera mount's fast update
    camera_mount.update_fast();
#endif

    // log sensor health
    if (should_log(MASK_LOG_ANY)) {
        Log_Sensor_Health();//日志记录
    }
}

// rc_loops - reads user input from transmitter/receiver
// called at 100hz
void Copter::rc_loop()
{
    // Read radio and 3-position switch on radio
    // -----------------------------------------
    read_radio();         // 无线接收器 pwm 各通道输入
    read_control_switch();//控制开关输入
}

// throttle_loop - should be run at 50 hz
// ---------------------------
void Copter::throttle_loop()
{
    // update throttle_low_comp value (controls priority of throttle vs attitude control)
    update_throttle_thr_mix();

    // check auto_armed status
    update_auto_armed();

#if FRAME_CONFIG == HELI_FRAME
    // update rotor speed
    heli_update_rotor_speed_targets();

    // update trad heli swash plate movement
    heli_update_landing_swash();
#endif

    // compensate for ground effect (if enabled)
    update_ground_effect_detector();
}

// update_mount - update camera mount position
// should be run at 50hz  相机云台 摄影机座架 位置
void Copter::update_mount()
{
#if MOUNT == ENABLED
    // update camera mount's position
    camera_mount.update();
#endif
}

// update camera trigger    快门开关
void Copter::update_trigger(void)
{
#if CAMERA == ENABLED
    camera.trigger_pic_cleanup();
    if (camera.check_trigger_pin()) {
        gcs().send_message(MSG_CAMERA_FEEDBACK);
        if (should_log(MASK_LOG_CAMERA)) {
            DataFlash.Log_Write_Camera(ahrs, gps, current_loc);
        }
    }    
#endif
}

// update_batt_compass - read battery and compass 电池电压  和  罗盘信息  
// should be called at 10hz
void Copter::update_batt_compass(void)
{
    // read battery before compass because it may be used for motor interference compensation
    read_battery(); // 电池电压

    if(g.compass_enabled) {
        // update compass with throttle value - used for compassmot
        compass.set_throttle(motors->get_throttle());
        compass.read();// 罗盘信息
        // log compass information
        if (should_log(MASK_LOG_COMPASS) && !ahrs.have_ekf_logging()) {
            DataFlash.Log_Write_Compass(compass);
        }
    }
}

// Full rate logging of attitude, rate and pid loops
// should be run at 400hz  全速率  记录角度 角速度  Pid 控制
void Copter::fourhundred_hz_logging()
{
    if (should_log(MASK_LOG_ATTITUDE_FAST)) {
        Log_Write_Attitude();
    }
}

// ten_hz_logging_loop
// should be run at 10hz
void Copter::ten_hz_logging_loop()// 10 Hz  日志记录
{
    // log attitude data if we're not already logging at the higher rate
    if (should_log(MASK_LOG_ATTITUDE_MED) && !should_log(MASK_LOG_ATTITUDE_FAST)) {
        Log_Write_Attitude();//姿态信息
        Log_Write_EKF_POS(); //位置信息
    }
    if (should_log(MASK_LOG_MOTBATT)) {
        Log_Write_MotBatt();//电池电压
    }
    if (should_log(MASK_LOG_RCIN)) {
        DataFlash.Log_Write_RCIN(); // 遥控 控制信号输入PWM
        if (rssi.enabled()) {
            DataFlash.Log_Write_RSSI(rssi);//信号强度
        }
    }
    if (should_log(MASK_LOG_RCOUT)) {
        DataFlash.Log_Write_RCOUT();// 执行信号输出 PWM
    }
    if (should_log(MASK_LOG_NTUN) && (mode_requires_GPS(control_mode) || landing_with_GPS())) {
        Log_Write_Nav_Tuning();     // GPS导航信息
    }
    if (should_log(MASK_LOG_IMU) || should_log(MASK_LOG_IMU_FAST) || should_log(MASK_LOG_IMU_RAW)) {
        DataFlash.Log_Write_Vibration(ins);//惯性传感器信息
    }
    if (should_log(MASK_LOG_CTUN)) {
        attitude_control->control_monitor_log();//
        Log_Write_Proximity();                  //
        Log_Write_Beacon();                     //
    }
#if FRAME_CONFIG == HELI_FRAME
    Log_Write_Heli();
#endif
}

// twentyfive_hz_logging - should be run at 25hz
void Copter::twentyfive_hz_logging()// 25 Hz  日志记录
{
#if HIL_MODE != HIL_MODE_DISABLED
    // HIL for a copter needs very fast update of the servo values
    gcs().send_message(MSG_SERVO_OUTPUT_RAW);
#endif

#if HIL_MODE == HIL_MODE_DISABLED
    if (should_log(MASK_LOG_ATTITUDE_FAST)) {
        Log_Write_EKF_POS();
    }

    // log IMU data if we're not already logging at the higher rate
    if (should_log(MASK_LOG_IMU) && !should_log(MASK_LOG_IMU_RAW)) {
        DataFlash.Log_Write_IMU(ins);
    }
#endif

#if PRECISION_LANDING == ENABLED
    // log output
    Log_Write_Precland();
#endif
}

void Copter::dataflash_periodic(void)
{
    DataFlash.periodic_tasks();
}

// three_hz_loop - 3.3hz loop
void Copter::three_hz_loop()
{
    // check if we've lost contact with the ground station
    failsafe_gcs_check();//是否与地面站失去通信

    // check if we've lost terrain data   失去地形数据？
    failsafe_terrain_check();

#if AC_FENCE == ENABLED
    // check if we have breached a fence
    fence_check();
#endif // AC_FENCE_ENABLED

#if SPRAYER == ENABLED
    sprayer.update();
#endif

    update_events();

    // update ch6 in flight tuning
    tuning();
}

// one_hz_loop - runs at 1Hz
void Copter::one_hz_loop()
{
    if (should_log(MASK_LOG_ANY)) {
        Log_Write_Data(DATA_AP_STATE, ap.value);
    }

    arming.update();//解锁标志更新

    if (!motors->armed()) {
        // make it possible to change ahrs orientation at runtime during initial config
        ahrs.set_orientation();

        update_using_interlock();

        // check the user hasn't updated the frame class or type
        motors->set_frame_class_and_type((AP_Motors::motor_frame_class)g2.frame_class.get(), (AP_Motors::motor_frame_type)g.frame_type.get());

#if FRAME_CONFIG != HELI_FRAME
        // set all throttle channel settings
        motors->set_throttle_range(channel_throttle->get_radio_min(), channel_throttle->get_radio_max());
#endif
    }

    // update assigned functions and enable auxiliary servos 辅助伺服系统
    SRV_Channels::enable_aux_servos();

    check_usb_mux();

    // log terrain data  记录地形数据
    terrain_logging();

    adsb.set_is_flying(!ap.land_complete);
    
    // update error mask of sensors and subsystems. The mask uses the
    // MAV_SYS_STATUS_* values from mavlink. If a bit is set then it
    // indicates that the sensor or subsystem is present but not
    // functioning correctly
    update_sensor_status_flags();
}

// called at 50hz
void Copter::update_GPS(void)
{
    static uint32_t last_gps_reading[GPS_MAX_INSTANCES];   // time of last gps message
    bool gps_updated = false;

    gps.update();

    // log after every gps message
    for (uint8_t i=0; i<gps.num_sensors(); i++) {
        if (gps.last_message_time_ms(i) != last_gps_reading[i]) {
            last_gps_reading[i] = gps.last_message_time_ms(i);

            // log GPS message
            if (should_log(MASK_LOG_GPS) && !ahrs.have_ekf_logging()) {
                DataFlash.Log_Write_GPS(gps, i);
            }

            gps_updated = true;
        }
    }

    if (gps_updated) {
        // set system time if necessary
        set_system_time_from_GPS();

        // checks to initialise home and take location based pictures
        if (gps.status() >= AP_GPS::GPS_OK_FIX_3D) {

#if CAMERA == ENABLED
            if (camera.update_location(current_loc, copter.ahrs) == true) {
                do_take_picture();
            }
#endif
        }
    }
}

void Copter::init_simple_bearing()  //航向
{
    // capture current cos_yaw and sin_yaw values
    simple_cos_yaw = ahrs.cos_yaw();
    simple_sin_yaw = ahrs.sin_yaw();

    // initialise super simple heading (i.e. heading towards home) to be 180 deg from simple mode heading
    super_simple_last_bearing = wrap_360_cd(ahrs.yaw_sensor+18000);
    super_simple_cos_yaw = simple_cos_yaw;
    super_simple_sin_yaw = simple_sin_yaw;

    // log the simple bearing to dataflash
    if (should_log(MASK_LOG_ANY)) {
        Log_Write_Data(DATA_INIT_SIMPLE_BEARING, ahrs.yaw_sensor);
    }
}

// update_simple_mode - rotates pilot input if we are in simple mode
void Copter::update_simple_mode(void)
{
    float rollx, pitchx;

    // exit immediately if no new radio frame or not in simple mode
    if (ap.simple_mode == 0 || !ap.new_radio_frame) {
        return;
    }

    // mark radio frame as consumed
    ap.new_radio_frame = false;

    if (ap.simple_mode == 1) {
        // rotate roll, pitch input by -initial simple heading (i.e. north facing)
        rollx = channel_roll->get_control_in()*simple_cos_yaw - channel_pitch->get_control_in()*simple_sin_yaw;
        pitchx = channel_roll->get_control_in()*simple_sin_yaw + channel_pitch->get_control_in()*simple_cos_yaw;
    }else{
        // rotate roll, pitch input by -super simple heading (reverse of heading to home)
        rollx = channel_roll->get_control_in()*super_simple_cos_yaw - channel_pitch->get_control_in()*super_simple_sin_yaw;
        pitchx = channel_roll->get_control_in()*super_simple_sin_yaw + channel_pitch->get_control_in()*super_simple_cos_yaw;
    }

    // rotate roll, pitch input from north facing to vehicle's perspective
    channel_roll->set_control_in(rollx*ahrs.cos_yaw() + pitchx*ahrs.sin_yaw());
    channel_pitch->set_control_in(-rollx*ahrs.sin_yaw() + pitchx*ahrs.cos_yaw());
}

// update_super_simple_bearing - adjusts simple bearing based on location
// should be called after home_bearing has been updated
void Copter::update_super_simple_bearing(bool force_update)
{
    // check if we are in super simple mode and at least 10m from home
    if(force_update || (ap.simple_mode == 2 && home_distance > SUPER_SIMPLE_RADIUS)) {
        // check the bearing to home has changed by at least 5 degrees
        if (labs(super_simple_last_bearing - home_bearing) > 500) {
            super_simple_last_bearing = home_bearing;
            float angle_rad = radians((super_simple_last_bearing+18000)/100);
            super_simple_cos_yaw = cosf(angle_rad);
            super_simple_sin_yaw = sinf(angle_rad);
        }
    }
}

void Copter::read_AHRS(void)
{
    // Perform IMU calculations and get attitude info
    //-----------------------------------------------
#if HIL_MODE != HIL_MODE_DISABLED
    // update hil before ahrs update
    gcs_check_input();
#endif

    // we tell AHRS to skip INS update as we have already done it in fast_loop()
    ahrs.update(true);
}

//海拔高度
// read baro and rangefinder altitude at 10hz
void Copter::update_altitude()
{
    // read in baro altitude
    read_barometer();//气压计

    // write altitude info to dataflash logs
    if (should_log(MASK_LOG_CTUN)) {
        Log_Write_Control_Tuning();
    }
}

AP_HAL_MAIN_CALLBACKS(&copter);// 线程注册有效
