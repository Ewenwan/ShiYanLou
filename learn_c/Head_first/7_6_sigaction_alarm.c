#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>//  strerror()
#include <errno.h>//  errno
#include <signal.h>
int score = 0;
void end_game(int sig)
 {
   printf("\n最终得分: %i\n", score);
   exit(0);
 }
 
 void times_up(int sig)
 {
   puts("\n时间到了!");
   raise(SIGINT);
 }
int catch_signal(int sig, void (*handler)(int)){//信号编号 处理器指针
	struct sigaction action;  // 结构体
	action.sa_handler = handler;// 信号处理器函数
	sigemptyset(&action.sa_mask); // 用掩码过滤 sigaction要处理的信号
	action.sa_flags = 0;  // 附加标志位
	return sigaction(sig, &action, NULL);
}
 
 void error(char* msg)
 {
   fprintf(stderr, "%s: %s\n", msg, strerror(errno));
   exit(1);
 }
 int main()
 {
   catch_signal(SIGALRM, times_up);//定时器触发的 SIGALRM 信号 再调用 times_up处理 触发  SIGTERM终止函数 
   catch_signal(SIGINT,  end_game);//Ctrl-C 信号 
   srandom (time (0));//每次都能得到不同的随机数 
   while(1) {
     int a = random() % 11;
     int b = random() % 11;
     char txt[4];
     alarm(5);//5s 内答题  5s内回到这个地方 闹钟会重置 
     printf("\nWhat is %i * %i = ? ", a, b);
     fgets(txt, 4, stdin);
     int answer = atoi(txt);//转成整形 
     if (answer == a * b)
       score++;
     else
       printf("\nWrong! Score: %i\n", score);
   }
   return 0;
 }
