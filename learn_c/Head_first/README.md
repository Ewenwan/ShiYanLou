# 类型
> 指针 类型* 符号;
> char *temp_s; 指向字符类型的指针(地址)

> int *num;     指向整形类型的指针

> void *all;    可以指向任意类型

> int a = *(int*)all // 函数参数类型使用 void*  函数内使用需要先转化为具体类型的指针 再 取值

> 字符串字面值保存在 只读存储器中 字符串数组是 数组的数组 即 char** 

> strlen() 返回字符串长度

> char 是字符 为数值， 小整数short 大整数long 普通整数int 一般浮点数 float  高精度浮点数 double

# 多个源文件
> 函数声明与定义分离  声明在.h头文件 定义在.c文件

> #include<stdlib.h>包含标志头文件   #include "my_func.h" 包含本地文件

> make管理代码构建过程

# 枚举 联合 结构体
> 枚举 enum{COUNT, POUNDS, PINTS};  符号变量

> 联合 union{short count; float weight; float volume;};  可以在同一个存储单元中保存不同的数据类型

> 结构体 typedef struct{}my_struce,*my_st; 组合多种组数据类型  typedef  为结构体创建别名

> my_struce.temp  点表示法读取结构中的字段 my_st->temp = 123; -> 表示法读取/更新结构中的字段

> 位字段  unsigned int low_pass:1; 1位 0~1   unsigned int low_pass:3; 三位  0~7

# 数据结构 链表 动态存储
> 动态 数据结构使用 递归结构，包含一个或多个链向相同数据结构的数据的 链接（指针）

> typedef struct island{ 

> char *name; 

> char *opens; 

> char *closes;

> struct island *nest;

> }island;

# 动态存储
> 计算机存储单元：

> 只读存储器：保存代码 和 常量 包含 代码段 和 常量段，不可修改

> 全局量段，保存全局变量

> 栈 用来 保存局部变量 ，随用随取，自动管理内存，不用即清除

> 堆 用来存储 需要手动管理的变量 可长时间存储，内存需要自己管理  malloc()申请内存  free()释放内存

> strdup()会把字符串复制到堆上   valgrind 是一款用于内存调试、内存泄漏检测以及性能分析的软件开发工具

# 函数指针   
> 函数指针，可使得函数 像 数据一样传递

> int tee(char *tmp){}; 函数

> int (*fun_p)(char *tmp); 函数指针

> 每个函数名都是一个指向函数的 函数指针

> qsort()排序函数 接收 排序规则函数（比较器函数）指针

> 函数指针是唯一不需要 加 * 和 &运算符的指针，当然加上也不会错
 

# 函数指针数组
> 多个函数可以像数组一样被调用

> int (*fun_p_a[])(char *tmp); 函数指针数组

# 可变参数函数

> variadic function

> 参数数量可变的函数 叫可变参数函数  包含 #include<stdarg.h>可以创建可变参数函数

> va_list val;//参数列表 va_start(val, args);//从args 开始  va_arg(val,int);//列表 参数类型 va_end(val); //结束 销毁列表

# 静态库 与动态库
> #include<> 会查找 /usr/include 下的标准目录

> #include "头文件路径" 会按照路径查找头文件

> gcc -I/my_header_files test_code.c -o test_coden  -I告诉编译器还可以去哪里找头文件

> gcc -I/my_header_files test_code.c /my_header_files/encrypt.o  -o test_coden 还可以指定 目标文件的路径

> 为目标文件创建 存档 ar指令  存档包含多个目标文件的 文件   nm指令查看存档文件 包含了哪些目标文件 以及 函数

> ar -rcs libname.a  object_file1.o object_file2.o   存档文件 格式 libXXX.a 固定格式  静态库 可以放在 /usr/local/lib标准库下

> 编译时使用 自编译的静态库  gcc test_code.c  -L/my_lib  -lname -o test_code   -L 指定静态库的位置 

> **动态链接库 加强版的目标文件  在运行时链接 的

> 创建动态链接库目标文件 **gcc -I/my_header_files -fPIC -c test_code_lib.c -o libtest.o   //-fPIC位置无关代码 -c 不链接代码

> 动态链接库 共享      **gcc -shared libtest.o -o libtest_.so //linux上以  .so结尾   -shared 转成动态库

> 使用 **gcc test_code.c  -L/my_lib  -ltest -o test_code   -L 指定动态库的位置

# 数据流 重定向 文件  管道|连接进程  一台主机上 进程间通信
> printf() fprintf() scanf()  fscanf() 格式化 输入输出数据流 标准输入stdin  标准输出stdout 标准错误stderr 

> fopen("文件名", mode) 定义自己的数据流 mode: w 写  r读  a 追加写入  getopt() 获取命令行参数

> < 输入重定向 ， 输出重定向 >；每一种数据流都对应描述符； 0号为标志输入 stdin 默认 来源于键盘 ； 1号为标志输出 stdout  2号为标志错

> 误 ； 1号和2号 默认输出到 屏幕；

> 2>  代表重定向标志错误，可以重定向到文件 2>error.txt;  2>&1 把标识错误和标准输出重定向到同一个地方。

### fileno 定义自己的数据流 其他任意指定，使用fileno() 查看文件指针 的 描述符

> FILE *my_file = fopen("guitar.mp3","r");

> int descriptor = fileno(my_file); // 得到文件指针 的描述符  该函数不会失败 即不可能返回 -1

### dup2(数据流源 描述符，目标 描述符) 复制 文件描述符 重定向文件

> dup2(4,3);//将4号文件描述符 对应的 数据流 也对应到3号描述符，3号描述符原来对应的数据流失效

> FILE *f = fopen("stories.txt","w");

> dup2(fileno(f),1)；//将标准输出 重定向到 stories.txt文件

### 管道 ls | grep txt C函数 pipe()函数建立管道

> int fd[2];//文件描述符 file description

> if(pipe(fd) == -1) error("不能创建管道");//fd[1]保存了管道写入端 文件描述符  fd[0]保存了管道读取端 fd

> close(fd[0]);//子进程 //关闭管道的读取端，子进程不会从管道读取数据

> dup2(fd[1], 1);// 管道写入端连接到 标志输出 stdout  子进程输出到 stdout的数据都会写入到管道写入端

> dup2(fd[0], 0);//父进程 //父进程从管道输出端 获取数据到 标志输入 stdin

> closed(fd[1]);//父进程不会向管道 写数据 关闭管道 写入端  // 子进程写入到管道的数据，酱油 父进程的标准输入读取

> 管道只能单向通信。

> 可以创建两个管道 一个从子进程的标准输出定向到 管道输入，再从管道输出定向到 父进程的标准输入。

> 另一个可以 从父进程的标准输出定向到 管道输入，再从管道输出 定向到 子进程的标志输入，实现双向通信。

### 信号控制进程   Ctrl-C中断信号 SIGINT值为2 会调用 exit()函数结束程序  sigaction设置信号处理器 raise()向自己发送信号 kill 命令行 命令发送信号   alarm()函数定时向进程发送 SIGALRM 信号   

> int catch_signal(int sig, void(*handler)(int)){//信号编号 处理器指针
> struct sigaction action;      // 结构体
> action.sa_handler = diediedie;// 信号处理器函数
> sigemptyset(&action.sa_mask); // 用掩码过滤 sigaction要处理的信号
> action.sa_flags = 0;          // 附加标志位
> if(sigaction(sig, &action, NULL))//编号 新动作 旧动作(可以为NULL)
> {
> fprintf("发送错误，错误：%s",strerror(errno));
>  return -1;
>  }
>  else return 0; //直接 return sigaction(sig, &action, NULL)
> }
> void diediedie(int sig){      // 自定义信号处理器函数
>   puts("再见了...");
>   exit(1);
> }
> catch_signal(SIGINT, diediedie);//函数指针 按下 Ctrl-C之后触发 SIGINT 信号，会进入指定的信号处理函数

# 不同电脑 上 程序通信 网络套接字 socket()  新的数据流     文件、标准输入、标准输出 
### BLAB四部曲  Bind 绑定端口  Listen 监听端口 Accept 接收连接   Begin  开始通信   端口就好比 电视不同的频道
## 服务器Serve
### 创建socket

>  #include <sys/socket.h>

>  int listener_d = socket(PF_INET, SOCKET_STREAM, 0);// 协议  socket数据流   0为协议号

>  if(listener_d == -1) error("无法打开套接字");

### Bind 绑定端口

> #include <arpa/inet.h>

> struct sockaddr_in name;

> name.sin_family = PF_INET;

> name.sin_port = (int_port_t)htons(30000);//端口 范围  0~65535  通常 选择 1024以上  本地端口

> name.sin_addr.s_addr = htonl(INADDR_ANY);//本地IP

> int c = bind(listener_d, (struct sockaddr *) &name, sizeof(name));

> if(c == -1) error("无法绑定端口");

### Listen 监听端口

> if(listen(listener_d, 10) == -1) error("无法监听");// 监听队列最大为10  排队 列队最长 10   后面的客户端会被通知 服务器忙

### Accept 接收连接

> struct sockaddr_storage client_addr;//保存客户端的详细信息

> unsigned int address_size = sizeof(client_addr);

> int connect_d = accept(listener_d,  (struct sockaddr *) &client_addr, &address_size);

> if(connect_d == -1) error("无法打开副套接字);

### Begin  开始通信 
> * 使用send()通信  不是 fprintf()、fscanf()、fgets()、fputs()   recv()接收数据

> * char *msg = "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! knock!\r\n> ";//发送的消息

> * if(send(connect_d, msg, strlen(msg), 0) == -1) error("send error");//最后一个参数 是高级选项 0 就可以了

## 客户端Client

> 需要 远程服务器的 ip地址(可以使用 域名 ) 和端口号 

### 连接远程端口

> *int s = socket(PF_INET, SOCKET_STREAM, 0);// 协议  socket数据流   0为协议号
> if( s == -1) error("无法打开套接字");
> struct sockaddr_in name;
> name.sin_family = PF_INET;
> name.sin_addr.s_addr = inet_addr("208.201.239.100");//远程 服务器 ip地址   getaddrinfo()获取域名的ip  #include<netdb.h>
> name.sin_port = htons(80);// 绝大多数 网络服务器 运行在 80 端口
> int c = connect(s, (struct sockaddr *) &name, sizeof(name));
> if(c == -1) error("无法连接服务器");

> getaddrinfo()获取域名方法 创建 socket
`
  #include <netdb.h>
  struct addrinfo *res;//解析结果 结构体指针
  struct addrinfo hints;//结构体变量
  memset(&hints, 0, sizeof(hints));//初始化为0
  hints.ai_family = PF_UNSPEC;     //协议类型 AF_UNSPEC  0 协议无关  AF_INET 2 IPv4协议   AF_INET6 23 IPv6协议
  hints.ai_socktype = SOCK_STREAM; //数据类型 SOCK_STREAM 1 流  SOCK_DGRAM  2  数据报
  getaddrinfo("www.oreilly.com", "80", &hints, &res);// 端口80 域名 "www.oreilly.com" 的解析信息
  int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);//创建端口
  connect(s, res->ai_addr, res->ai_addrlen);//连接
  freeaddrinfo(res);//连接后删除 地址信息  该信息存储在 堆中 需要手动清除
`
### 开始通信  read()  send()
> 发送  接收消息

# 线程
> 进程（店面）的缺点： 创建时间较长；共享数据不方便，需要管道；代码较复杂，先fock()一个进程，在执行execXXX();普通进程，单线程，一个人干活。

> 线程（员工）启动快，又可以共享数据，可以并行执行多个线程，一个进程可以多开几个线程（多雇几名员工），并行工作。

> 使用POSIX 线程库，也叫 pthread  #include<pthread.h> pthread_create()创建线程 pthread_t结构体 变量保存 线程信息 pthread_join()等待线程结束

> 使用线程库 编译时需要链接库 -lpthread 

> 多线程会同时运行，因此两个线程可能会同时访问共享的数据资源，会出现意想不到的后果，需要增设红绿灯来防止撞车，老司机开车小心！

> pthread_mutex_t a_lock = PTHREAD_MUTEX_INITIALIZER;//初始化锁 
> pthread_mutex_lock(&a_lock);//启动锁
> 含有共享数据的代码 (同一时间段 只能有一个线程通过)
> pthread_mutex_unlock(&a_lock)//开锁

`
pthread_t pt1;
pthread_t pt2;
if(pthread_create(&pt1, NULL, 函数1, NULL) == -1) error("创建线程pt1失败");
if(pthread_create(&pt2, NULL, 函数2, NULL) == -1) error("创建线程pt2失败");
void *result;
if(pthread_join(pt1, &result) == -1) error("无法回收线程 pt1");
if(pthread_join(pt2, &result) == -1) error("无法回收线程 pt2");
`

