#include <stdio.h>
#include <stdlib.h> //exit()
#include <unistd.h>// execXXX()函数 
#include <errno.h> // errno 变量 
#include <string.h>// strerror() 函数 
//#include <sys/wait.h>// waitptd() 等待进程 函数  window 也没有 
// 错误处理函数
void error(char *msg){
	fprintf(stderr, "%s %s\n", msg, strerror(errno));
	exit(1);//会立刻终止程序， 并把退出状态置 1   exit() 是唯一没有返回值 并且不会失败的函数 
} 

// 打开url连接
void open_url(char * url){
	char launch[255];
	sprintf(launch, "cmd /c start %s", url);//window 下打开网页
	system(launch);
	sprintf(launch, "x-www-browser '%s' &", url);//linux下打开网页
	system(launch);
	sprintf(launch, "open '%s'", url);//mac下打开网页
	system(launch);	
	
} 
 int main(int argc, char* argv[])
 {
   char* phrase = argv[1];//需要搜索的 文本 
   char* vars[] = {"RSS_FEED=http://www.cnn.com/rss/celebs.xml", NULL};
    
    int fd[2];//管道 输出端 写入端fd[1]  输入端fd[0] 描述符 
	if(pipe(fd) == -1){
		error("创建管道失败");
	}// pipe创建的管道不是文件 仅能使用在 父子进程中
	// 可以使用 mkfifo() 系统调用创建  实名 命名管道 FIFO  
	
	
     pid_t pid = fork();// 会返回一个整形值 子进程返回0  父进程返回一个正数 复制出错返回 -1 
     if(pid == -1) {
		error("不能复制进程");
		} 
		
     if(!pid){// pid=0为 子进程 由新的进程接力 
         close(fd[0]);// 子进程 不会读取 管道的输出 
		 dup2(fd[1],1);//子进程 的标志输出 定向到 管道的写入端 
         // 运行子程序  需要一定的时间  
	     if (execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", "-u", phrase, NULL, vars) == -1) {
			error("不能运行脚本"); //子进程搜索在 RSS_FEED中搜索  phrase 返回 url 
	     }
     }
     //父进程 本进程
	 close(fd[1]);//关闭 管道写fd[1] 
	 dup2(fd[0],0);//父进程 标准输入stdin 描述符0 重定向到 管道输入 fd[0] 
     char line[255];
	 while(fgets(line, 255, stdin)) {
	 	if(line[0] == '\t')
	 	open_url(line + 1);
	 }
     
   return 0;
 }
