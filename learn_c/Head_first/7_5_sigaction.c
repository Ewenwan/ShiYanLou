#include <stdio.h>
#include <stdlib.h> //exit()
#include <signal.h>

void diediedie(int sig){      // 自定义信号处理器函数
  puts("再见了...");
  exit(1);
}
int catch_signal(int sig, void (*handler)(int)){//信号编号 处理器指针
struct sigaction action;  // 结构体
action.sa_handler = handler;// 信号处理器函数
sigemptyset(&action.sa_mask); // 用掩码过滤 sigaction要处理的信号
action.sa_flags = 0;  // 附加标志位
return sigaction(sig, &action, NULL);
}

int main(){
	/*
	SIGINT   中断信号  Ctrl-C等
    SIGQUIT  停止程序 
    SIGFPE   浮点错误 
    SIGTRAP  询问程序执行到哪里了 
    SIGSEGV  访问非法存储器地址 
    SIGWINCH 终端窗口 大小改变 
    SIGTERM  终止程序 
    SIGPIPE  向一个没有人读的管道写数据 
    SIGKILL  不能忽略的 终止信号   就是说在 程序不响应了 也可以使用  SIGKILL 终结它 
    SIGSTOP  不能忽略的 暂停信号  
    SIGALRM  闹钟信号  alrm 
    可以用 kill发送信号  命令行中 
	例如：
	ps -a 列出进行信息 含有进程的进程号
	kill  78222        //默认发送  SIGTERM 信号
	kill -INT   78222  // 发送 SIGINT信号 
	kill -SEGV  78222  // 发送 SIGSEGV信号 
	kill -KILL  78222  // 发送 SIGKILL信号   送程序上西天
	
    用 raise()  发送信号 程序中 
	raise(SIGTERM);//程序给自己发信号  信号升级  收到低级信号后 再 触发高级信号   蝴蝶效应 
	 
	 alarm(120); 定时120秒后 发出  SIGALRM 信号 期间可以做其他事情   也sleep()函数不能同时使用 
	 // setitimer()  可以设定 几分之一秒 定时 闹钟 
	 catch_signal(SIGALRM, do_other)//定时做其他事情 
	
	 catch_signal(SIGALRM, SIG_DFL)//还原默认方式 处理信号 
	 catch_signal(SIGINT, SIG_IGN) // 忽略某个信号    SIGKILL SIGSTOP不能被忽略 
	*/
	if(catch_signal(SIGITN, diediedie) == -1){// Ctrl-C之后触发的函数
	 fprintf(stderr, "不能指定中断信号处理函数"); 
	 exit(2); 
	}
	char name[30];
	printf("Please enter your name: \r\n");
	fgets(name, 30, stdin);
	printf("Hello %s \r\n",name);
	return 0;	
	
}
