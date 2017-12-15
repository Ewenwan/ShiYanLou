# 指针
 类型* 符号;
 `
> char *temp_s; 指向字符类型的指针(地址)
> int *num;     指向整形类型的指针
> void *all;    可以指向任意类型
// 函数参数类型使用 void*  函数内使用需要先转化为具体类型的指针 再 取值
> int a = *(int*)all
 `
# 数据流 重定向 文件
> < 输入重定向 ， 输出重定向 >；每一种数据流都对应描述符； 0号为标志输入 stdin 默认 来源于键盘 ； 1号为标志输出 stdout  2号为标志错> 误 ； 1号和2号 默认输出到 屏幕；
> 2>  代表重定向标志错误，可以重定向到文件 2>error.txt
## 其他任意指定，使用fileno() 查看文件指针 的 描述符
`
FILE *my_file = fopen("guitar.mp3","r");
int descriptor = fileno(my_file); // 得到文件指针 的描述符  该函数不会失败 即不可能返回 -1
`
## dup2(数据流源 描述符，目标 描述符) 复制 文件描述符 重定向文件
`
dup2(4,3);//将4号文件描述符 对应的 数据流 也对应到3号描述符，3号描述符原来对应的数据流失效
FILE *f = fopen("stories.txt","w");
dup2(fileno(f),1)；//将标准输出 重定向到 stories.txt文件
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


