# 学习 C 语言

## 鱼C工作室 >带你学C带你飞
[鱼C工作室 >带你学C带你飞](http://blog.fishc.com/category/c/page/7)

[C语言知识点整理](http://jinjaysnow.github.io/blog/2014-09/C%E8%AF%AD%E8%A8%80%E7%9F%A5%E8%AF%86%E7%82%B9%E6%95%B4%E7%90%86.html)

# 1. 关键字 
* 1. auto 自动变量声明 
* 2. 整形 short int long  浮点型float double  字符型char signed unsigned     
     类型-模子---模具---（内存空间大小不一样）   
* 3. struct结构体 union联合 enum枚举 static静态变量(内存静态区-唯一性-静态全局-静态局部-静态函数(内部函数))     
     const只读变量(常量) ，常量表达式，常不分配内存空间，而是将它们保存在符号表中   
     它是全局的只读变量，存放在静态区， 在程序运行过程中只有一份拷贝。   
     
     enum 枚举 可以自增1，这样不用每一个值都定义，而 宏 #define 必须每个值都定义。   
     而且枚举是一个集合，代表一类值，像你代码中的颜色归为一类，方便使用，而宏不能形成集合。   
     #define不只是可以为类型取别名，还可以定义常量、变量、编译开关等.   
     
* 4. case break default if else do while for continue 
* 5. register寄存器变量-最快-CPU皇帝同志的太监总管-中转站    
* 6. volatile易失型变量(确保本条指令不会因编译器的优化而省略，且要求每次直接从内存中读值)      
* 7. typedef 类型别名, 伟大的缝纫师，擅长做马甲, 面具，化妆师， 整容师， 挂羊头卖狗肉，鬼门关，孟婆汤 ...      
                      华美绝伦的芍药，就有个别名---“将离”, 表达了那种依依不舍的惜别之情…    
                      常用来给结构体取别名，方便使用。   
                      
* 8. extern声明-未分配内存(喝稀粥)，外来的，非国产的，不是本文件中定义的；  定义才分配了内存(抱伊人)  
* 9. void色即是空空即是色，世间万物  return返回 
* 10. goto无条件跳转语句，尽量少用，或不用 
* 11. sizeof 计算对象所占内存空间大小 
      sizeof 在计算变量所占空间大小时，括号可以省略，而计算类型(模子)大小时不能省略 
      int i; sizeof i; sizeof(i); sizeof(int)都可以； sizeof int;编译出错 
      
## 技巧
>**bool 变量与“零值”进行比较**

          if(bTestFlag); if(!bTestFlag);
>**float 变量与“零值”进行比较**
          
          if((fTestVal >= -EPSINON) && (fTestVal <= EPSINON)); //EPSINON 为定义好的精度
>**指针变量与“零值”进行比较**   
          
          int * p = NULL;//定义指针一定要同时初始化
          if(NULL == p); if(NULL != p);
>**代码的缩进一般为 4 个字符，但不要使用 Tab 键，因为不同的编辑器 Tab 键定义
的空格数量不一样，别的编辑器打开 Tab 键缩进的代码可能会一片混乱。**

>**if 语句使用建议**
          
          把正常情况的处理放在 if 后面，而不要放在 else 后面，else执行的会少一些。

>**case 语句建议**
          
          case 后面只能是整型或字符型的常量或常量表达式（想想字符型数据在内存里是怎么存的）
          把正常情况放在前面，而把异常情况放在后面。
          把最常执行的情况放在前面，而把最不常执行的情况放在后面。
>**do、while、for循环 语句建议**
          
          do{} while()先执行后判断
          while(){} 先判断后执行
          for(初始化;条件;递归增量){循环体} 初始化-->判断条件-->循环体--->递归增量--->判断条件--->循环体--->递归增量-->
          break 表示本层循环(最近的一个循环)结束；
          continue 表示跳过本层循环的某一次循环；
          
          continue语句的作用是跳过循环本中剩余的语句，并到循环末尾。
          continue语句只用在for、while、do-while等循环体中, 常与if条件语句一起使用, 用来加速循环。
          
          多层循环，应当将最长的循环放在最内层，最短的循环放在最外层，以减少 CPU 跨切循环层的次数
```c
for (col=0; col<5; col++ )// 外层循环 次数少   常采用左闭右开区间=======
{
     for (row=0; row<100; row++)// 内存循环次数多
     {
          sum = sum + a[row][col];
     }
}
```

>**struct定义的柔性数组 flexible array**
```c
// C99 中，结构中的最后一个元素允许是未知大小的数组，这就叫做柔性数组成员,
// 但结构中的柔性数组成员前面必须至少一个其他成员。
// 柔性数组成员允许结构中包含一个大小可变的数组。
// sizeof 返回的这种结构大小不包括柔性数组的内存。
typedef struct st_type
{
int i;
int a[]; // int a[0];
}type_a; // type_a 为 typede f定义的 结构体 st_type 的别名，方便使用
// 定义一个可变长的结构体，用 sizeof(type_a)得到的只有 4，就是 sizeof(i)=sizeof(int)。
// 给结构体分配内存
type_a *p = (type_a*)malloc(sizeof(type_a)+100*sizeof(int));
// 为结构体指针 p 分配了一块内存。用 p->a[n]就能简单地访问可变长元素。
// 我们再用 sizeof（*p）测试结构体的大小，发现仍然为 4。
// 在定义这个结构体的时候，模子的大小就已经确定不包含柔性数组的内存大小。
// 柔性数组只是编外人员，不占结构体的编制。
// 用 free 函数来释放内存：
// free(p);

```
>**struct 的成员默认情况下属性是 public 的，而 class 成员却是 private 的**  

>**大小端模式对 union 类型数据的影响,高高低低是小端模式；高低低高是大端模式**

>**序测试系统的存储模式**
```c
int checkSystem( )
{
     union check
     {
          int i;   // 4字节
          char ch; // 存放在 低地址
     } c;
     c.i = 1; // 大端模式：高字节低地址 0x0 0x0 0x0 0x1 低字节高地址，c.ch 为 0
              // 小端模式：高字节高地址 0x0 0x0 0x0 0x1 低字节低地址，c.ch 为 1
     
     return (c.ch ==1);// 高高低低小端模式（1），高低低高大端模式（0）
}
```
>**数组类型的指针以及数组元素类型的指针**
```c
int a[5]={1,2,3,4,5};// 数组a的类型为 存有5个整形元素的数组 sizeof(a)=20, 此时a的类型为int[5]数组
                     // sizeof(*a)=4，因为有取值符*，表示把a当成一个指针(int*)，而a指向数组的首地址，
                     // a=&(a[0])，即sizeof(*a)=sizeof(*&(a[0]))=sizeof(a[0])=sizeof(int)=4
int *ptr1=(int *)(&a+1);// (&a+1)先取变量a的地址，并根据a的地址获得下一个与a同类型的相邻地址。
                        // 根据前面所说的a的类型为int[5]数组
                        // &a+1=&a+sizeof(5*int)，因此&a+1指向的地址为&a[5](数组a[5]的下一个地址)。
int *ptr2=(int *)((int)a+1);
printf("%x,%x",ptr1[-1],*ptr2);// 5,2
                        // ptr-1=ptr-sizeof(int)，故ptr-1指向&a[4]。因此，*(ptr-1)的值即为a[4]=5。

```
>**typedef常见用法**
```c
1.常规变量类型定义：typedef unsigned char uchar描述：uchar等价于unsigned char类型定义
                       uchar c 声明 等于unsigned char c声明
2.数组类型定义:     typedef int array[2];（注：可理解为typedef int[] array）
                       array 等价于 int[2]定义;      array a 声明等价于int a[2]声明
                   typedef int array[M][N]; 描述： array等价于 int[M][N]定义;
                       array a 声明 等价于int a[M][N]声明
3.指针类型定义:     typedef  int *pointer;   描述： pointer等价于 int*定义; 
                       pointer p 声明等价于int* a声明
                   typedef int *pointer[M]; 描述： pointer等价于 int*[M]定义;
                       pointer p声明等价于int*a[M]声明明
4.函数地址说明描述:  int func(void);       
                    unsigned long funcAddr=(unsigned long)func；
                    funcAddr 的值是 func 函数的首地址
5.函数声明例如：     typedef int func(void); 
                       func 等价于 int(void)类型函数
6.函数指针例如：     typedef int (*func)(void) 
                    func等价于int (*)(void)类型  
                       func pf 等价于 int(*pf)(void)声明，pf是一个函数指针变量
```
# 2. 符号






