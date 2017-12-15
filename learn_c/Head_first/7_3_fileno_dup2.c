#include <stdio.h>
#include <stdlib.h> //exit()
#include <unistd.h>// execXXX()函数 
#include <errno.h> // errno 变量 
#include <string.h>// strerror() 函数 
#include <sys/wait.h>// waitptd() 等待进程 函数  window 也没有 
// 错误处理函数
void error(char *msg){
	fprintf(stderr, "%s %s\n", msg, strerror(errno));
	exit(1);//会立刻终止程序， 并把退出状态置 1   exit() 是唯一没有返回值 并且不会失败的函数 
} 
 int main(int argc, char* argv[])
 {
   char* phrase = argv[1];//需要搜索的 文本 
     char* vars[] = {"RSS_FEED=http://www.cnn.com/rss/celebs.xml", NULL};
     FILE  *f = fopen("stories.txt","w");// 写形式打开文件
	 if(!f){//打不开文件 f=0 
	  error("打开文件失败"); 	
	 }
	 
     pid_t pid = fork();// 会返回一个整形值 子进程返回0  父进程返回一个正数 复制出错返回 -1 
     if(pid == -1) {
		error("不能复制进程");
		} 
		
     if(!pid){// pid=0为 子进程 由新的进程接力 
         if(dup2(fileno(f),1) == -1){//fileno(f)获取文件  数据流 描述符表中对应的 描述符   0~255  0 1 2固定 为标志输入 标志输出 标志错误 
         	error("不能重定向标志输出数据流");
         }
         // 运行子程序  需要一定的时间  
	     if (execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", NULL, vars) == -1) {
			error("不能运行脚本"); 
	     }
     }
     // 等待子进程结束
	int pid_status;
	if(waitpid(pid, &pid_status, 0)==-1){//子进程ID 子进程退出信息 int 指针  选项0 等待结束 
		error("等待子进程出错");
	}
	if(WEXITSTATUS(pid_status))//使用宏查看 退出状态不是 0  pid_status 整数 不同的位表示不同的信息 
	  puts("退出状态不是0"); // 自然死亡 为0  他杀就为非0 真 
	
   return 0;
 }
