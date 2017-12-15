# 指针
 类型* 符号;
 `
> char *temp_s; 指向字符类型的指针(地址)
> int *num;     指向整形类型的指针
> void *all;    可以指向任意类型
> int a = *(int*)all // 函数参数类型使用 void*  函数内使用需要先转化为具体类型的指针 再 取值
 `
# 数据流 重定向 文件  管道|连接进程
> < 输入重定向 ， 输出重定向 >；每一种数据流都对应描述符； 0号为标志输入 stdin 默认 来源于键盘 ； 1号为标志输出 stdout  2号为标志错> 误 ； 1号和2号 默认输出到 屏幕；
> 2>  代表重定向标志错误，可以重定向到文件 2>error.txt;  2>&1 把标识错误和标准输出重定向到同一个地方。
### 其他任意指定，使用fileno() 查看文件指针 的 描述符
`
FILE *my_file = fopen("guitar.mp3","r");
int descriptor = fileno(my_file); // 得到文件指针 的描述符  该函数不会失败 即不可能返回 -1
`
### dup2(数据流源 描述符，目标 描述符) 复制 文件描述符 重定向文件
`
dup2(4,3);//将4号文件描述符 对应的 数据流 也对应到3号描述符，3号描述符原来对应的数据流失效
FILE *f = fopen("stories.txt","w");
dup2(fileno(f),1)；//将标准输出 重定向到 stories.txt文件
`
### 管道 ls | grep txt C函数 pipe()函数建立管道
`
int fd[2];//文件描述符 file description
if(pipe(fd) == -1) error("不能创建管道");//fd[1]保存了管道写入端 文件描述符  fd[0]保存了管道读取端 fd
close(fd[0]);//子进程 //关闭管道的读取端，子进程不会从管道读取数据
dup2(fd[1], 1);// 管道写入端连接到 标志输出 stdout  子进程输出到 stdout的数据都会写入到管道写入端
dup2(fd[0], 0);//父进程 //父进程从管道输出端 获取数据到 标志输入 stdin
closed(fd[1]);//父进程不会向管道 写数据 关闭管道 写入端  // 子进程写入到管道的数据，酱油 父进程的标准输入读取
`
> 管道只能单向通信。
> 可以创建两个管道 一个从子进程的标准输出定向到 管道输入，再从管道输出定向到 父进程的标准输入。
> 另一个可以 从父进程的标准输出定向到 管道输入，再从管道输出 定向到 子进程的标志输入，实现双向通信。

### 信号控制进程   Ctrl-C中断信号 SIGINT值为2 会调用 exit()函数结束程序  sigaction设置信号处理器 raise()向自己发送信号 kill 命令行 命令发送信号   alarm()函数定时向进程发送 SIGALRM 信号   
`
int catch_signal(int sig, void(*handler)(int)){//信号编号 处理器指针
struct sigaction action;      // 结构体
action.sa_handler = diediedie;// 信号处理器函数
sigemptyset(&action.sa_mask); // 用掩码过滤 sigaction要处理的信号
action.sa_flags = 0;          // 附加标志位
if(sigaction(sig, &action, NULL))//编号 新动作 旧动作(可以为NULL)
 {
 fprintf("发送错误，错误：%s",strerror(errno));
 return -1;
 }
 else return 0; //直接 return sigaction(sig, &action, NULL)
}

void diediedie(int sig){      // 自定义信号处理器函数
  puts("再见了...");
  exit(1);
}

catch_signal(SIGINT, diediedie);//函数指针 按下 Ctrl-C之后触发 SIGINT 信号，会进入指定的信号处理函数

`
# 多个源文件


# 枚举 联合 结构体
> 枚举 enum{COUNT, POUNDS, PINTS};  联合 union{short count; float weight; float volume;};  结构体 struct{};

# 链表 动态存储
typedef struct island{ char *name; char *opens; char *closes; struct island *nest;}island;

# 函数指针   
> int tee(char *tmp){}; 函数
> int (*fun_p)(char *tmp); 函数指针

# 函数指针数组
> int (*fun_p_a[])(char *tmp); 函数指针数组

# 可变参数函数
> variadic function
> #include<stdarg.h>
> va_List va_start  va_arg  va_end


