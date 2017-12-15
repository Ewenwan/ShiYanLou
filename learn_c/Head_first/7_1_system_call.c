#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//system("strstr...") 把字符串当成命令运行
 
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
