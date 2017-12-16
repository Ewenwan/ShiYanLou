#include <stdio.h>
#include <stdlib.h>
#include <string.h>//  strerror()
#include <errno.h>//  errno
#include <sys/socket.h> 
#include <arpa/inet.h> 

 void error(char* msg)
 {
   fprintf(stderr, "%s: %s\n", msg, strerror(errno));
   exit(1);
 }
 
int main(int argc, char* argv[])
 {
   char* advice[] = {
     "Take smaller bites\r\n",
      "Go for the tight jeans. No they do NOT make you look fat.\r\n",
      "One word: inappropriate\r\n",
      "Just for today, be honest. Tell your boss what you *really* think\r\n",
      "You might want to rethink that haircut\r\n"
   }; 
	// 创建  互联网流 套接字 
	int listener_d = socket(PF_INET, SOCK_STREAM, 0);// 协议  socket数据流   0为协议号 
	if(listener_d == -1) 
		error("无法打开套接字");
	
	// 绑定后 30秒内不允许再次绑定
	int reuse = 1;//重新 使用端口 
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
    	error("Can't set the reuse option on the socket");
	//绑定 端口 
	struct sockaddr_in name; 
	name.sin_family = PF_INET; 
	name.sin_port = (int_port_t)htons(30000);//端口 范围  0~65535  通常 选择 1024以上 
	name.sin_addr.s_addr = htonl(INADDR_ANY); 
	int c = bind(listener_d, (struct sockaddr *) &name, sizeof(name)); 
	if(c == -1) 
		error("无法绑定端口");
		
	//监听 端口 
	if(listen(listener_d, 10) == -1) 
	   // 监听队列最大为10  排队 列队最长 10   后面的客户端会被通知 服务器忙	
		error("无法监听");
	puts("Waiting for connection");
	
	struct sockaddr_storage client_addr;//保存客户端的详细信息 
	unsigned int address_size = sizeof(client_addr); 
	for (;;) {		
		// 接收链接 
		int connect_d = accept(listener_d, (struct sockaddr *) &client_addr, &address_size); 
		if(connect_d == -1) 
			error("无法打开副套接字);	
		
		// 开始通信  发送数据	
		char *msg = advice[rand() % 5]; 
		if(send(connect_d, msg, strlen(msg), 0) == -1) //最后一个参数 是高级选项 0 就可以了
			error("send error");
		close(connect_d);
	} 
	return 0;
}
