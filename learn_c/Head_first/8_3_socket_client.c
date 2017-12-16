 #include <stdio.h>
 #include <string.h>
 #include <errno.h>
 #include <stdlib.h>
 #include <sys/socket.h>//socket
 #include <arpa/inet.h>//ip 
 #include <unistd.h>
 #include <netdb.h>// getaddrinfo 域名获取  ip 
 
 void error(char* msg)
 {
   fprintf(stderr, "%s: %s\n", msg, strerror(errno));
   exit(1);
 }
 /*
//数字IP 方式打开 socket  dns 域名服务器中没有对应的域名时 而知道 ip地址 
int s = socket(PF_INET, SOCKET_STREAM, 0);// 协议 ?socket数据流 ? 0为协议号
if( s == -1) error("无法打开套接字");
struct sockaddr_in name;
name.sin_family = PF_INET;
name.sin_addr.s_addr = inet_addr("208.201.239.100");//远程 服务器 ip地址 ? getaddrinfo()获取域名的ip ?#include<netdb.h>
name.sin_port = htons(80);// 绝大多数 网络服务器 运行在 80 端口
int c = connect(s, (struct sockaddr *) &name, sizeof(name));
if(c == -1) error("无法连接服务器");
 */ 
 
 // 以域名方式打开 socket   先决条件 域名已在 域名服务器中注册 
 int open_socket(char* host, char* port)
 {
   struct addrinfo *res;//解析结果 结构体指针
   struct addrinfo hints;//结构体变量
   memset(&hints, 0, sizeof(hints));//初始化为0
   hints.ai_family = PF_UNSPEC; //协议类型 AF_UNSPEC  0 协议无关  AF_INET 2 IPv4协议 ? AF_INET6 23 IPv6协议
   hints.ai_socktype = SOCK_STREAM; //数据类型 SOCK_STREAM 1 流  SOCK_DGRAM  2  数据报
   if (getaddrinfo(host, port, &hints, &res) == -1)// 端口80 域名 "www.oreilly.com" 的解析信息
     error("Can't resolve the address");
   int d_sock = socket(res->ai_family, res->ai_socktype,res->ai_protocol);//创建socket 
   if (d_sock == -1)
     error("Can't open socket");
   int c = connect(d_sock, res->ai_addr, res->ai_addrlen);//连接
   freeaddrinfo(res);//连接后删除 地址信息 ?该信息存储在 堆中 需要手动清除
   if (c == -1)
     error("Can't connect to socket");
   return d_sock;
 }
 
  //发送消息 给服务器 
 int say(int socket, char* s)
 {
   int result = send(socket, s, strlen(s), 0);
   if (result == -1)
     fprintf(stderr, "%s: %s\n", "Error talking to the client", strerror(errno));
   return result;
 }
 
 
 int main(int argc, char* argv[])
 {
   int d_sock;
   d_sock = open_socket("en.wikipedia.org", "80");
   char buf[255];
   // 客户端连接上服务器后
   // 1需要先发送  GET 命令 
   sprintf(buf, "GET /wiki/%s http/1.1\r\n", argv[1]);//传入对应的网页 
   say(d_sock, buf);
   // 2 主机名  + 空行 \r\n 
   say(d_sock, "Host: en.wikipedia.org\r\n\r\n");
   // 接收缓冲区 
   char rec[256];
   int bytesRcvd = recv(d_sock, rec, 255, 0);
   while (bytesRcvd) {//还能接收到消息 
     if (bytesRcvd == -1)
       error("Can't read from server");
     rec[bytesRcvd] = '\0';// '\0' 替换  '\r' 
     printf("%s", rec);//打印收到的消息 
     bytesRcvd = recv(d_sock, rec, 255, 0);//再次接收 最多(255)个数据 
   }
   close(d_sock);//关闭socket连接 
   return 0;
 }
 
