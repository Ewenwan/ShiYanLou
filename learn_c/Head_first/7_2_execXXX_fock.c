#include <stdio.h>
#include <stdlib.h>
 #include <unistd.h>// execXXX()函数 
 #include <errno.h> // errno 变量 
 #include <string.h>// strerror() 函数 
// 进程 存储器中运行的程序 
// taskmgr window下  
//  ps -ef  linux下 
// exec + l / v + p/e/pe 命令 用一个进程替换一个进程 PID不变 接力赛
// l list 参数列表 ； v vector 参数数组/向量 ；p 路径 path ; e 环境变量 environment 
//  C程序中使用 getenv("变量名") 获取环境变量的值
// char *my_env[] = {"变量名=值",NULL}; //变量数组 最后一个必须为 NULL
// execle("diner_info", "diner_info", "4", NULL, my_env);//使用 diner_info替换当前进程 前连个参数需要相同
// 执行的状态 保存在变量 errno 中  puts(strderror(errno));查询错误信息
/* 执行出错返回 -1 同时 修改  变量 errno
   EPERM=1    | 不允许操作       Operation not permitted   |
 | ENOENT=2   | 没有该文件或目录 No such file or directory |
 | ESRCH=3    | 没有该进程       No such process           |
 | EMULLET=81 | 发型很难看(不会) Bad haircut  
*/ 
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

// 环境变量 
// coffee.c
// gcc coffee.c -o coffee
/*
 #include <stdio.h>
 #include <stdlib.h>
 int main(int argc, char* argv[])
 {
   char* w = getenv("EXTRA");
   if (!w)
     w = getenv("FOOD");
   if (!w)
     w = argv[argc - 1];
   char* c = getenv("EXTRA");
   if (!c)
     c = argv[argc - 1];
   printf("%s with %s\n", c, w);
   return 0;
 }
*/ 
//输出  donuts with coffee 
/*
 #include <stdio.h>
 #include <stdlib.h>
  int main(int argc, char* argv[]){
      char* my_env[] = {"FOOD=coffee", NULL};
      if(execle("./coffee", "./coffee", "donuts", NULL, my_env) == -1){
        fprintf(stderr,"Can't run process 0: %s\n", strerror(errno));
        return 1;
       } 
      fprintf(stderr,"Can't create order: %s\n", strerror(errno));
      return 1;
    }
    return 0;
  }
*/

 int main(int argc, char* argv[])
 {
   // 不同的新闻来源 网站 
   char* feeds[] = {"http://www.cnn.com/rss/celebs.xml",
                    "http://www.rollingstone.com/rock.xml",
                    "http://eonline.com/gossip.xml"};
   int times = 3;
   char* phrase = argv[1];//需要搜索的 文本 
   int i;
   for (i = 0; i < times; i++) {
     char var[255];//环境变量设置 
     sprintf(var, "RSS_FEED=%s", feeds[i]);
     char* vars[] = {var, NULL};
     // 只要execle()执行成功一遍本程序 就停止了！！！！
     // 需要先复制一份 子程序 子进程 父进程继续执行 本程序 
     // 注意 window下无 fork()程序  如需使用 需要安装  Cygwin 
     pid_t pid = fork();// 会返回一个整形值 子进程返回0  父进程返回一个正数 复制出错返回 -1 
     if(pid == -1) {
		fprintf(stderr, "不能复制进程：%s", strerror(errno));
		return 1; 
		} 
     if(!pid){// pid=0为 子进程 由新的进程接力 
	     if (execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", NULL, vars) == -1) {
	       fprintf(stderr, "Can't run script: %s\n", strerror(errno));
	       return 1;
	     }
        }
   }
   return 0;
 }
