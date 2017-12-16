#include <stdio.h>
#include <stdlib.h>
#include <string.h>//  strerror()
#include <errno.h>//  errno
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <signal.h>//处理信号  本机进程间通信
#include <unistd.h>// execXXX()函数   进程接力函数 
// gcc socket_server.c -o server
// ./server 启动服务端
// telnet 127.0.0.1 30000  客户端 发起链接 

// 打印错误 并结束程序   
  void error(char* msg)
 {
   fprintf(stderr, "%s: %s\n", msg, strerror(errno));
   exit(1);//会终结程序 
 }
 
 int listener_d;//主监听套接字 
 // 处理器函数  处理 Ctrl-C 
 void handle_shutdown(int sig){
 	if(listener_d) 
 		close(listener_d);
 	fprintf(stderr, "再见了...\n");
    exit(0);	
 } 
 
//捕获 消息  并执行处理器函数 
int catch_signal(int sig, void (*handler)(int)){//信号编号 处理器指针
	struct sigaction action;  // 结构体
	action.sa_handler = handler;// 信号处理器函数
	sigemptyset(&action.sa_mask); // 用掩码过滤 sigaction要处理的信号
	action.sa_flags = 0;  // 附加标志位
	return sigaction(sig, &action, NULL);
}
 
 
 // 打开 互联网流 套接字 
 int open_listener_socket()
 {
   int s = socket(PF_INET, SOCK_STREAM, 0);
   if (s == -1)
     error("Can't open socket");
   return s;
 }
 
 // 绑定端口 
 void bind_to_port(int socket, int port)
 {
   struct sockaddr_in name;
   name.sin_family = PF_INET;
   name.sin_port = (in_port_t)htons(30000);//端口  
   name.sin_addr.s_addr = htonl(INADDR_ANY);
   int reuse = 1;//端口重用 
   if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
     error("Can't set the reuse option on the socket");
   int c = bind (socket, (struct sockaddr *) &name, sizeof (name));
   if (c == -1)
     error("Can't bind to socket");
 }
 //监听端口 
 void listen_to_port(int socket, int queue_size)
 {
 	if(listen(socket, queue_size) == -1) 
		error("无法监听");
	puts("Waiting for connection");
 } 
 // 接收连接
  
 //发送消息 给客户端 
 int say(int socket, char* s)
 {
   int result = send(socket, s, strlen(s), 0);
   if (result == -1)
     fprintf(stderr, "%s: %s\n", "Error talking to the client", strerror(errno));
   return result;
 }
 // 接从客户端收消息 
    /* Handle the error */
 int read_in(int socket, char* buf, int len){
 // 客户端过来的数据以\r\n结尾 
   char* s = buf;
   int slen = len;
   int c = recv(socket, s, slen, 0);//接收 一部分数据 返回实际接收的数据长度  
   while ((c > 0) && (s[c-1] != '\n')) {// 直到没有可读的字符 或者读到了 \n 
     s++; slen -= c;
     c = recv(socket, s, slen, 0);
   }
   if (c < 0)//  recv 发送错误 返回-1 
     return c;
   else if (c == 0)//什么都没有读到 
     buf[0] = '\0';//返回空字符 
   else
     s[c-1]='\0';//用'\0' 代替'\r' 
   return slen - len;
 }
 
 //主函数 
  int main(int argc, char* argv[])
 {
   if (catch_signal(SIGINT, handle_shutdown) == SIG_ERR)
     error("不能设置中断处理器函数");
   //1 创建套接字  主套接字  listener_d  监听功能 
   listener_d = open_listener_socket();
   //2 绑定端口 
   bind_to_port(listener_d, 30000);
   //3 监听端口 
   listen_to_port(listener_d, 10); 
   //4  接收客户端连接 
   struct sockaddr_storage client_addr;//客户端地址 
   unsigned int address_size = sizeof client_addr;
   char buf[255];//接收消息缓冲区 
   for(;;) {
   	// 副套接字  connect_d  链接 客户端功能 
     int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
     if (connect_d == -1)
       error("Can't open secondary socket");
     //5 发送 消息 
     if(!fock()){// 子进程 fock() 返回0
	 	 close(listener_d); //子进程关闭 监听套接字 保留 链接套接字 
	     if (say(connect_d, "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! Knock!\r\n> ")!= -1) {
	       //6 接收消息 
	       read_in(connect_d, buf, sizeof(buf));
	       if (strncasecmp("Who's there?", buf, 12))//比较字符串  没有找到 返回1 
	          // 发送消息 
	         say(connect_d, "You should say 'Who's there?'!");
	       else {//正确回答 
	         if (say(connect_d, "Oscar\r\n> ") != -1) {
	           read_in(connect_d, buf, sizeof(buf));
	           if (strncasecmp("Oscar who?", buf, 10))
	             say(connect_d, "You should say 'Oscar who?'!\r\n");
	           else
	             say(connect_d, "Oscar silly question, you get a silly answer\r\n");
	         }
           }
         }
         close(connect_d);//子进程 与客户端通信结束后  关闭 链接套接字
		 exit(0); //子进程退出 程序 
     }    
     close(connect_d);//主进程 关闭 链接套接字  保留 监听套接字 
   }
   return 0;
 }
 
 
