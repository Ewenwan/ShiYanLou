/****************************************************************************
 *
 
 *
 ****************************************************************************/

/**
 * @file local_position_estimator.cpp
 * @author James Goppert <james.goppert@gmail.com>
 * @author Mohammed Kabir
 * @author Nuno Marques <n.marques21@hotmail.com>
 *
 * Local position estimator
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <systemlib/param/param.h>
#include <systemlib/err.h>
#include <drivers/drv_hrt.h>
#include <math.h>
#include <fcntl.h>
#include <px4_posix.h>
#include <px4_tasks.h>

#include "BlockLocalPositionEstimator.hpp"

static volatile bool thread_should_exit = false;     /**< Deamon exit flag */
static volatile bool thread_running = false;     /**< Deamon status flag */
static int deamon_task;             /**< Handle of deamon task / thread */

/**
 * Deamon management function.
 */
extern "C" __EXPORT int local_position_estimator_main(int argc, char *argv[]);

/**
 * Mainloop of deamon.
 */
int local_position_estimator_thread_main(int argc, char *argv[]);

/**
 * Print the correct usage.
 */
static int usage(const char *reason);

static int
usage(const char *reason)
{
	if (reason) {
		fprintf(stderr, "%s\n", reason);
	}

	fprintf(stderr, "usage: local_position_estimator {start|stop|status} [-p <additional params>]\n\n");
	return 1;
}

/**
 * The deamon app only briefly exists to start
 * the background job. The stack size assigned in the
 * Makefile does only apply to this management task.
 *
 * The actual stack size should be set in the call
 * to task_create().
 */
int local_position_estimator_main(int argc, char *argv[])
{

	if (argc < 2) {
		usage("missing command");
		return 1;
	}
	// 启动
	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			PX4_INFO("already running");
			/* this is not an error */
			return 0;
		}

		thread_should_exit = false;

		deamon_task = px4_task_spawn_cmd("lp_estimator",//任务名
						 SCHED_DEFAULT,         //默认调度
						 SCHED_PRIORITY_MAX - 5,//优先级
						 13500,                 //协议栈大小
						 local_position_estimator_thread_main,//主程序入口函数
						 (argv && argc > 2) ? (char *const *) &argv[2] : (char *const *) nullptr);//命令行参数
		return 0;
	}
	//结束
	if (!strcmp(argv[1], "stop")) {
		if (thread_running) {
			PX4_DEBUG("stop");
			thread_should_exit = true;

		} else {
			PX4_WARN("not started");
		}

		return 0;
	}
	//状态
	if (!strcmp(argv[1], "status")) {
		if (thread_running) {
			PX4_INFO("is running");

		} else {
			PX4_INFO("not started");
		}

		return 0;
	}

	usage("unrecognized command");
	return 1;
}

int local_position_estimator_thread_main(int argc, char *argv[])
{

	PX4_DEBUG("starting");

	using namespace control;

	BlockLocalPositionEstimator est;

	thread_running = true;

	while (!thread_should_exit) {
		est.update();
	}

	PX4_DEBUG("exiting.");

	thread_running = false;

	return 0;
}
