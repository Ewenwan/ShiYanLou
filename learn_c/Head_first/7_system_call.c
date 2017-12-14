#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 进程 存储器中运行的程序 
// taskmgr window下  
//  ps -ef  linux下 
// exec + l / v / p /e 命令 用一个进程替换一个进程 PID不变 接力赛
// l list 参数列表 ； v vector 参数数组/向量 ；p 路径 path ; e 环境变量 environment 
//  C程序中使用 getenv("变量名") 获取环境变量的值
// char *my_env[] = {"变量名=值",NULL}; //变量数组 最后一个必须为 NULL
// execle("diner_info", "diner_info", "4", NULL, my_env);//使用 diner_info替换当前进程 
// 执行的状态 保存在变量 errno 中  puts(strderror(errno));查询错误信息
// #include <unistd.h>  // execXXX()函数 
// #include <errno.h>   // errno 变量 
// #include <string.h>  // strerron() 显示错误消息
// 例如查看 网络信息
/*
#include <unistd.h>  // execXXX()函数 
#include <errno.h>   // errno 变量 
#include <string.h>

int main(){// exec()函数运行其他进程成功 本函数进行就死亡了 
  if(execl("/sbin/ifconfig","/sbin/ifconfig", NULL)==-1){
  	if(execl("ipconfig","ipconfig", NULL)==-1)
  	  fprintf(stderr,"不能运行 ipconfig: %s", strerron(erron));
  	  return 1;
  }
  return 0;
} 
*/ 

 
/*
struct tm {
   int tm_sec; //秒，范围从 0 到 59 //
   int tm_min; //分，范围从 0 到 59 //
   int tm_hour;//小时，范围从 0 到 23/
   int tm_mday;//一月中的第几天，范围从 1 到 31//
   int tm_mon; //月份，范围从 0 到 11 //
   int tm_year; //自 1900 起的年数//
   int tm_wday; //一周中的第几天，范围从 0 到 6//
   int tm_yday; //一年中的第几天，范围从 0 到 365 //
   int tm_isdst;//夏令时// 
};
*/
char * now_time(){
	time_t t;
	time (&t);
	return asctime (localtime (&t));	//C++ 中 asctime不安全，建议使用asctime_s代替。与localtime_s结合使用
}

int main(){
	char comment[80];//记录的文本
	char cmd[120];//system 执行的命令
	fgets(comment,80,stdin);//获取记录的文本
	sprintf(cmd,"echo '%s %s' >> report.log",comment,now_time());
	system(cmd);//系统调用 
	
	return 0;
}
