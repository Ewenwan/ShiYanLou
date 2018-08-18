#include "Copter.h"

/*
 * Init and run calls for stabilize flight mode  自稳模式
 
 在主循环执行过程中（比如Pixhawk的任务调度周期2.5ms，400Hz；APM2.x为10ms，100Hz），每一个周期，程序会按下述步骤执行：
首先，高层次文件flight_mode.pde中的update_flight_mode()函数被调用。通过检查control_mode变量，前飞行器的飞行模式（使用变量），然后执行相应飞行模式下的_run()函数（如自稳模式的stabilize_run，返航模式（RTL）的rtl_run等）。执行_run的结果是，系统将会找到与飞行模式相对应的命名为control_.pde飞行控制文件（比如：control_stabilize.pde，control_rtl.pde等）。
_run函数负责将用户的输入（从g.rc_1.control_in，g.rc_2.control_in等读入）转换为此时飞行模式下的倾斜角（lean angle）、滚转速率（rotation rate）、爬升率（climb rate）等（也就是设置目标值roll\pitch\yaw\throttle）。举个例子：AltHold(定高，altitude hold)模式中将用户的滚转和俯仰输入转换为倾斜角（单位：角度/°），将偏航输入转换为滚转速率（单位：°/s），将油门输入转换为爬升率（单位：cm/s）。
_run函数最后还必须要完成的就是将预期角度、速率等参数传送给姿态控制和/或方位控制库（它们都放在AC_AttitiudeControl文件夹内）。
AC_AttitiudeControl库提供了5种可能的方法来调整飞行器的姿态，下面来说明最通用的三种方法：
                1) angle_ef_roll_pitch_rate_ef_yaw():该函数需要一个地轴系坐标下滚转和偏航角度，一个地轴系坐标下的偏航速率。例如：传递给该函数三个参数分别为，roll = -1000， pitch = -1500， yaw = 500代表飞行器此时向左倾斜10°，低头15°，向右偏航速率为5°/s。
                2) angle_ef_roll_pitch_yaw():该函数接受地轴系下的滚转、俯仰和偏航角。和上面的函数类似，不过参数yaw = 500代表飞行器北偏东5°
                3) rate_bf_roll_pitch_yaw():该函数接受一个体轴系下的滚转、俯仰和偏航角速率（°/s）。例如：传递给该函数三个参数：roll = -1000， pitch = -1500， yaw = 500代表飞行器此时左倾速率10°/s，低头速率15°/s，绕Z轴速率为5°/s。
当上述这些函数调用之后，就会接着调用AC_AttitudeControl::rate_controller_run()函数，将上面所列举的函数的输出转化为滚转、偏航和俯仰输入，并使用set_roll,set_pitch,set_yaw 和 set_throttle方法将这些输入发送给AP_Motors库。
 
另外，
AC_PosControl库用来控制飞行器的3D方位。不过通常只用来调整比较简单的Z轴方向（如姿态控制），这是因为许多需要复杂3D方位调整的飞行模式(例如悬停Loiter)使用的是“AC_WPNav库”。总之，AC_PosControl库中常用的方法有：
                1) set_alt_target_from_climb_rate():将爬升率（cm/s）作为参数，用来更新一个需要调整的相对高度目标。
                2) set_pos_target():接受一个以系统中的home位置作为参考点的3D位置矢量（单位：cm）。
如果调用了AC_PosControl中的任何一个方法，那么在该飞行模式下就必须调用函数AC_PosControl::update_z_controller()。这样的话，就可以启用Z轴的方位控制PID循环，并向AP_Motors库发送低级别的油门信息。同样，如果调用了xy轴的函数，那就就必须调用AC_PosControl::update_xy_controller()函数。
AP_Motors库含有“电机混合模式”代码。这些代码负责将从AC_AttitudeControl和AC_PosControl库发送过来的滚转、俯仰、偏航角度和油门值信息转换为电机的相对输出值（例如：PWM值）。因此，这样高级别的库就必须要使用如下函数：
                1) set_roll(),set_pitch(),set_yaw()：接受在[-4500,4500]角度范围内的滚转、俯仰和偏航角。这些参数不是期望角度或者速率，更准确的讲，它仅仅是一个数值。例如，set_roll(-4500)将代表飞行器尽可能快的向左滚转。
                2) set_throttle():接受一个范围在[0,1000]的相对油门值。0代表电机关闭，1000代表满油门状态。
虽然对于不同飞行器构型（如四旋翼，Y6，传统直升机等）的控制代码中有许多不同的类，但这些类中都有一个相同的函数output_armed，负责将这些滚转、俯仰、偏航和油门值转换为PWM类型输入值。这转换的过程中，会应用到stability patch，用来控制由于飞行器构型限制所引起的轴系的优先级问题（例如四旋翼的四个电机不可能在做最大速度滚转时四个电机的油门同时达到最大，因为必须一部分电机输出小于另一部分才能引起滚转）。在执行函数output_armed的最后，还将调用hal.rcout->write()，把期望PWM值传递给AP_HAL层。
AP_HAL库（硬件抽象层）提供了针对所有飞控板统一的接口。实际控制中，hal.rc_out->write()函数将接受到的来自于AP_Motors类中指定的PWM值，输出至飞控板对应的PWM端口（pin端）。
 
 
 */

// stabilize_init - initialise stabilize controller
bool Copter::stabilize_init(bool ignore_checks)
{
    // if landed and the mode we're switching from does not have manual throttle and the throttle stick is too high
    if (motors->armed() && ap.land_complete && !mode_has_manual_throttle(control_mode) &&
            (get_pilot_desired_throttle(channel_throttle->get_control_in()) > get_non_takeoff_throttle())) {
        return false;
    }
    // set target altitude to zero for reporting
    pos_control->set_alt_target(0);  // 设置 目标高度为零

    return true;
}

// stabilize_run - runs the main stabilize controller
// should be called at 100hz or more
void Copter::stabilize_run()
{
    float target_roll, target_pitch;   // 横滚 俯仰 角度
    float target_yaw_rate;             // 偏航角速度
    float pilot_throttle_scaled;       // 油门  范围

    // if not armed set throttle to zero and exit immediately 未解锁
    if (!motors->armed() || ap.throttle_zero || !motors->get_interlock()) {
        motors->set_desired_spool_state(AP_Motors::DESIRED_SPIN_WHEN_ARMED);
        attitude_control->set_throttle_out_unstabilized(0,true,g.throttle_filt);
        return;
    }

    // clear landing flag
    set_land_complete(false);// 清 着陆标志

    motors->set_desired_spool_state(AP_Motors::DESIRED_THROTTLE_UNLIMITED);

    // apply SIMPLE mode transform to pilot inputs
    update_simple_mode();

    // convert pilot input to lean angles  横滚 俯仰角度
    // To-Do: convert get_pilot_desired_lean_angles to return angles as floats
    get_pilot_desired_lean_angles(channel_roll->get_control_in(), channel_pitch->get_control_in(), target_roll, target_pitch, aparm.angle_max);

    // get pilot's desired yaw rate  偏航角速度
    target_yaw_rate = get_pilot_desired_yaw_rate(channel_yaw->get_control_in());

    // get pilot's desired throttle  油门
    pilot_throttle_scaled = get_pilot_desired_throttle(channel_throttle->get_control_in());

    // call attitude controller  姿态控制器  roll pitch 角度控制  yaw 角速度控制
    attitude_control->input_euler_angle_roll_pitch_euler_rate_yaw(target_roll, target_pitch, target_yaw_rate, get_smoothing_gain());

    // body-frame rate controller is run directly from 100hz loop

    // output pilot's throttle
    attitude_control->set_throttle_out(pilot_throttle_scaled, true, g.throttle_filt);
}
