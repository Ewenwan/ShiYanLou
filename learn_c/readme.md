# 解决任何问题的办法都一致，那就是把没见过的、不会的问题想法设法转换成你见过的、你会的问题。

# 学习 C 语言

[c语言 循环代码优化 python工具](https://github.com/inducer/loopy)

[小项目学习 扫雷小游戏 贪吃蛇小游戏 学生成绩管理系统 图书管理系统 小说分析软件](https://github.com/Ewenwan/Curriculum-Design)

## 鱼C工作室 >带你学C带你飞
[鱼C工作室 >带你学C带你飞](http://blog.fishc.com/category/c/page/7)

[C语言知识点整理](http://jinjaysnow.github.io/blog/2014-09/C%E8%AF%AD%E8%A8%80%E7%9F%A5%E8%AF%86%E7%82%B9%E6%95%B4%E7%90%86.html)

# 陷阱
```c
// 词法陷阱
// 1. 赋值号 = 和判等号 == ， 常用判等写法：if (常量 == 变量)
// 2. 位运算符， 按位与& 按位或| 按位非~; 逻辑运算符，逻辑与&& 逻辑或|| 逻辑非! 
// 语法陷阱
// 语义陷阱

```
# 工具

> **1. PClint 代码静态检查工具**

官网：http://www.gimpel.com/html/index.htm

它能有效地发现程序语法错误、潜在的错误隐患、不合理的编程习惯等。

PCLint就是不通过执行程序进行代码分析，发现代码中潜在的Bug。通过个人实践来看，PCLint可以发现非逻辑上的80%左右的Bug，绝对是C++开发以及测试人员一个强大的助手。

以C语言为例，其中的编号1-199指的是一般编译器也会产生的语法错误；编号200-299是PC-Lint程序内部的错误，这类错误不会出现在代码中的；编号300-399指的是由于内存限制等导致的系统致命错误。编号400-999中出现的提示信息，是根据隐藏代码问题的可能性进行分类的：其中编号400-699指的是被检查代码中很可能存在问题而产生的告警信息；编号700-899中出现的信息，产生错误的可能性相比告警信息来说级别要低，但仍然可能是因为代码问题导致的问题。编号900-999是可选信息，他们不会被默认检查，除非你在选项中指定检查他们。

> **2. Bullseye Coverage 代码覆盖率测试工具**

Bullseye Coverage 是Bullseye 公司提供的一款C/C++代码覆盖率测试工具。

代码覆盖率 =代码的覆盖程度，一种度量方式。

1.2. 语句覆盖(StatementCoverage)

      又称行覆盖(LineCoverage)，段覆盖(SegmentCoverage)，基本块覆盖(BasicBlockCoverage)，这是最常用也是最常见的一种覆盖方式，就是度量被测代码中每个可执行语句是否被执行到了。
      
      这里说的是“可执行语句”，因此就不会包括像C＋＋的头文件声明，代码注释，空行，等等。非常好理解，只统计能够执行的代码被执行了多少行。
      需要注意的是，单独一行的花括号｛｝ 也常常被统计进去。
      
      语句覆盖常常被人指责为“最弱的覆盖”，它只管覆盖代码中的执行语句，却不考虑各种分支的组合等等。
      
1.3. 判定覆盖(DecisionCoverage)

      又称分支覆盖(BranchCoverage)，所有边界覆盖(All-EdgesCoverage)，基本路径覆盖(BasicPathCoverage)，判定路径覆盖(Decision-Decision-Path)。它度量程序中每一个判定的分支是否都被测试到了。
      
1.4. 条件覆盖(ConditionCoverage)

      它度量判定中的每个子表达式结果true和false是否被测试到了。
      
1.5. 路径覆盖(PathCoverage)

      又称断言覆盖(PredicateCoverage)。它度量了是否函数的每一个分支都被执行了。 这句话也非常好理解，就是所有可能的分支都执行一遍，有多个分支嵌套时，需要对多个分支进行排列组合，可想而知，测试路径随着分支的数量指数级别增加。
      
      这是最强的覆盖，一般代码覆盖率工具都不支持，BullseyeCoverage也不支持。

> **3.GTest算法集成测试工具**

gtest是一个跨平台的(Liunx、Mac OS X、Windows、Cygwin、Windows CE and Symbian)C++单元测试框架，由google公司发布。gtest是为在不同平台上为编写C++测试而生成的。它提供了丰富的断言、致命和非致命判断、参数化、”死亡测试”等等。 

https://www.cnblogs.com/helloworldcode/p/9606838.html

https://www.cnblogs.com/coderzh/category/183698.html

# 1. 关键字 
```c
* 1. auto 自动变量声明 
* 2. 整形 short int long  浮点型float double  字符型char signed unsigned     
     类型-模子---模具---（内存空间大小不一样）   
* 3. struct结构体 union联合 enum枚举 static静态变量(内存静态区-唯一性-静态全局-静态局部-静态函数(内部函数))     
     const只读变量(常量) ，常量表达式，常不分配内存空间，而是将它们保存在符号表中   
     它是全局的只读变量，存放在静态区， 在程序运行过程中只有一份拷贝。   
     const修饰的不是常量而是 readonly 的变量，
     const 修饰的只读变量不能用来作为定义数组的维数，
     也不能放在 case 关键字后面。
     
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
``` 
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
>**反斜杠\用作接续符和转义符**
```c
cha\
r* s="这是一个合法的\\
n 字符串";
// 反斜杠除了可以被用作接续符，还能被用作转义字符的开始标识。
/*
     转义字符 转义字符的意义
     \n      回车换行
     \t      横向跳到下一制表位置
     \v      竖向跳格
     \b      退格
     \r      回车
     \f      走纸换页
     \\      反斜扛符"\"
     \'      单引号符
     \a      鸣铃
     \ddd    1～3 位八进制数所代表的字符
     \xhh    1～2 位十六进制数所代表的字符
*/
// 广义地讲， C 语言字符集中的任何一个字符均可用转义字符来表示。
// 表中的\ddd 和\xhh正是为此而提出的。
// ddd 和 hh 分别为八进制和十六进制的 ASCII 代码。如\102 表示字母"B"，
// \134 表示反斜线， \X0A 表示换行等
```
>**引号**
```c
1，‘1‘，“1”。
第一个是整形常数， 32 位系统下占 4 个 byte；
第二个是字符常量，占 1 个 byte；
第三个是字符串常量，占 2 个 byt，多一个\0结束符。

```
>**逻辑与位运算符**
```c
|| 逻辑或
&& 逻辑与
!  逻辑非

& 按位与
| 按位或
^ 按位异或 不同为1，相同为0
~ 取反    --- 位非
<< 左移  相当于 *2  , 高位丢弃，低位补 0。
>> 右移  相当于 /2
// 对于有符号数，在右移时，符号位将随同移动。
// 当为正数时， 最高位补 0；
// 而为负数时，符号位为 1，最高位是补 0 或是补 1 取决 于编译系统的规定。 

// 左移和右移的位数不能大于数据的长度，不能小于 0!!!!!
// 左移 32 位发生了什么事情？溢出！左移-1位呢？反过来移？


// 其中按位异或操作可以实现不用第三个临时变量交换两个变量的值：
// a ^= b; // a = a^b;
// b ^= a; // b = b ^ a = b ^ (a^b) = b^a^b = a;   
// a ^= b; // a = a^b = a^b^a = b; 
// 但并不推荐这么做，因为这样的代码读起来很费劲。
// 交换两个数的做法
// 1. 临时变量=========
  int tep = a;
  a = b;
  b = tep;
// 2. 求和============
  a = a + b;
  b = a - b; // b = a - b = a + b - b = a;
  a = a - b; // a = a - b = a + b - a = b;
// 3. 异或============
  a = a^b;
  b = b^a;
  a = a^b;

```
# 3. 预处理
```c
预处理名称    意 义  

#define      宏定义 ,  是个演技非常高超的替身演员, 经常耍大牌, 要搞定它其实很简单，别吝啬括号就行了.
             从本行宏定义开始，以后的代码就就都认识这个宏了.
             编译器会在预编译的时候用真身替换替身.
             
#undef       撤销已定义过的宏名  

#include    使编译程序将另一源文件嵌入到带有#include 的源文件中  

#if         #if 的一般含义是如果#if 后面的常量表达式为 true，    
            则编译它与#endif 之间的代码，否则跳过这些代码。  
            
#endif      命令 #endif 标识一个 #if 块的结束。   

#else       #else 命令的功能有点象 C 语言中的 else ，    
            #else 建立另一选择（在# if 失败的情况下）。   
            
#elif       #elif 命令意义与 else if 相同，   
            它形成一个 if else-if 阶梯状语句，可进行多种编译选择。   

#ifdef       用#ifdef 与 #ifndef 命令分别表示“如果有定义”及“如果无定义”，   
             是条件编译的另一种方法。    
#ifndef   

#line         改变当前行数和文件名称，它们是在编译程序中预先定义的标识符，   
              命令的基本形式如下：   
              #line number["filename"]      
#error        编译程序时，只要遇到 #error 就会生成一个编译错误提示消息，并停止编译     
#pragma       为实现时定义的命令，它允许向编译程序传送各种指令例如， 
              编译程序可  能有一种选择，它支持对程序执行的跟踪。   
              可用#pragma 语句指定一个跟踪选择。 
              
另外 ANSI 标准 C 还定义了如下几个宏：   
              _LINE_ 表示正在编译的文件的行号       
              _FILE_ 表示正在编译的文件的名字      
              _DATE_ 表示编译时刻的日期字符串，例如： "25 Dec 2007"    
              _TIME_ 表示编译时刻的时间字符串，例如： "12:30:55"    
              _STDC_ 判断该文件是不是定义成标准 C 程序    
```
## **3.1 #define 宏定义**
```c
#define PI 3.141592654   // 数值宏常量,  通常大写
#define ERROR_POWEROFF -1 // 这个-1，我们一般称为“魔鬼数”, 鬼知道-1 表示的是什么意思
// 字符串宏常量
#define ENG_PATH_1 E:\English\listen_to_this\listen_to_this_3
#define ENG_PATH_2 “E:\English\listen_to_this\listen_to_this_3”
// 请注意：有的系统里规定路径的要用双反斜杠“\\” ,比如：
#define ENG_PATH_4 E:\\English\\listen_to_this\\listen_to_this_3

// 如果路径太长,用反斜杠 接续符 啊：（最后一个 \ 为接续符）
#define ENG_PATH_3 E:\English\listen_to_this\listen\
_to_this_3

// 不能用define 宏定义注释符号，因为 注释 先于 预处理指令 被处理
#define BSC //
#define BMC /*
#define EMC */
BSC my single-line comment      // 错误, 预处理阶段不认识注释符
BMC my multi-line comment EMC   // 错误

// 定义表达式---注意多使用括号
#define SEC_A_YEAR (60*60*24*365) UL   // 定义一年有多少秒
#define SQR (x) ( (x) * (x) )          // 定义一个宏函数，求 x 的平方
                                       // 宏函数被调用时是以实参代换形参。而不是“值传送”。

// 撤销宏定义==========
#define PI 3.141592654
… // 可以使用 PI
// code                                       
#undef PI
// 下面的代码就不能用 PI 了，它已经被撤销了宏定义。
// 也就是说宏的生命周期从#define 开始到#undef 结束。

```
## 3.2 #ifdef #ifndef 条件编译
```C
第一种形式：

#ifdef 标识符
     程序段 1
#else          //可略 
     程序段 2
#endif


第二种形式：

#ifndef 标识符
     程序段 1
#else
     程序段 2
#endif


第三种形式：

#if 常量表达式
     程序段 1
#else       // #elif  阶梯状语句， 可进行多种编译选择。
     程序段 2
#endif


```
## 3.3 #pragma编译
```c
1. #pragma message("消息文本") 
     它能够在编译信息输出窗口中输出相应的信息，这对于源代码信息的控制是非常重要的。
     当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。

     假设我们希望判断自己有没有在源代码的什么地方定义了_X86 这个宏可以用下面的方法:
     #ifdef _X86
     #Pragma message(“_X86 macro activated!”)
     #endif

2. #pragma code_seg()
     另一个使用得比较多的 pragma 参数是 code_seg。
     格式如：

     #pragma code_seg( ["section-name"[,"section-class"] ] )

     它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。

3. #pragma once 编译一次
     比较常用只要在头文件的最开始加入这条指令就能够保证头文件被编译一次, 
     但是考虑到兼容性并没有太多的使用它。

4. #pragma hdrstop
     #pragma hdrstop 表示预编译头文件到此为止，后面的头文件不进行预编译。 
     你可以用#pragma startup 指定编译优先级，如果使用了#pragma package(smart_init) ，
     BCB就会根据优先级的大小先后编译.

5. #pragma resource 载入资源
     #pragma resource "*.dfm"  表示把*.dfm 文件中的资源加入工程。 
     *.dfm 中包括窗体外观的定义。

6. #pragma warning 错误信息
     #pragma warning(disable:4507 34) // 不显示 4507 和 34 号警告信息
     #pragma warning(once:4385)       // 4385 号警告信息仅报告一次
     #pragma warning(error:164)       // 把 164 号警告信息作为一个错误。

     #pragma warning( push )保存所有警告信息的现有的警告状态。
     #pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为 n。
     #pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。

7. #pragma pack(用于指定内存对齐的方式（按指定的字节数进行对齐）) 和 内存对齐问题(降低访存消耗)
struct TestStruct1
{
     char c1;  // 1字节   数据结构（尤其是栈）应该尽可能地在自然边界上对齐。(大小为4的倍数)
     short s;  // 2字节
     char c2;  // 1字节
     int i;    // 4字节
}

|c1| - |     s |   编译器在默认情况下按照4字节对齐，即#pragma pack(4)
|c2| - | - | - |
| i            |
3*4 =12 字节

CPU对内存的读取不是连续的，而是分块读取的，块的大小只能是2^i字节数(i=0,1,2,3…)。
从CPU的读取性能和效率来考虑，若读取的数据未对齐，则需要两次总线周期来访问内存，因而效率会大打折扣。
某些硬件平台只能从规定的相对地址处读取特定类型的数据，否则产生硬件异常。

struct TestStruct2
{
     char c1;  // 1字节   
     char c2;  // 1字节
     short s;  // 2字节
     int i;    // 4字节
}

|c1|c2|s    |
|i          |
2*4 = 8字节内存


使用指令#pragma pack (n)，编译器将按照 n 个字节对齐。
使用指令#pragma pack ()，编译器将取消自定义字节对齐方式。
在#pragma pack (n)和#pragma pack ()之间的代码按 n 个字节对齐。


#pragma pack(8)
struct TestStruct4
{
     char a; // 1字节
     long b; // 4字节
};
struct TestStruct5
{
     char c;  // 1字节
     TestStruct4 d; 
     long long e;// 8字节
};
#pragma pack()
                         a    b
TestStruct4 的内存布局： 1***,1111,
                        c   TestStruct4.a TestStruct4.b   d
TestStruct5 的内存布局： 1***,   1***,      1111, ****， 11111111

```

## 3.4 #运算符 变量取值  引号内替换变量
```c
#define SQR(x) printf("The square of x is %d.\n", ((x)*(x)));
如果这样使用宏：
SQR(8);
则输出为：
The square of x is 64.
注意到没有，引号中的字符 x 被当作普通文本来处理，而不是被当作一个可以被替换的语言符号。

#define SQR(x) printf("The square of "#x" is %d.\n", ((x)*(x)));
再使用：
SQR(8);
则输出的是：
The square of 8 is 64.
很简单吧？相信你现在已经明白#号的使用方法了。

```

## 3.5 ##运算符 用于宏函数的替换部分 把两个语言符号组合成单个语言符号
```c
#define XNAME(n) x ## n
如果这样使用宏：
XNAME(8)
则会被展开成这样：
x8
看明白了没？ ##就是个粘合剂，将前后两部分粘合起来。

```

# 4. 指针和数组
## 4.1 指针
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/img/pointer.PNG)
     
     如何将数值存储到指定的内存地址
     int *p = (int *)0x12ff7c; // 将地址 0x12ff7c 赋值给指针变量 p 的时候必须强制转换
     *p = 0x100;               // 通过钥匙“*”向这块内存写入一个数据 0x100
     *(int *)0x12ff7c = 0x100; // 这种一步搞定
     
## 4.2 数组 int a[5]; 
省政府和市政的区别----&a[0]和&a 的区别

     sizeof(&a[0])的值在 32 位系下为 4，这很好理解。取元素 a[0]的首地址。
     sizeof(&a)的值在 32 位系统下也为 4， 这也很好理解。 取数组 a 的首地址。
     但是在 VisualC++6.0 上，这个值为 20，我认为是错误的。
         
     sizeof(a)的值为 4*5 = 20


     a[0]是一个元素， a 是整个数组，虽然 a、 &a[0] 和 &a 的值一样，但其意义不一样。
     
     前者&a[0] / a 是 数组首元素 的 首地址，
     而后者 &a 是 数组 的首地址。
     
     举个例子：湖南的省政府在长沙，而长沙的市政府也在长沙。
     两个政府都在长沙，但其代表的意义完全不同。这里也是同一个意思。
     
     int a[5]={1,2,3,4,5};
     int *ptr=(int *)(&a+1); // 即 &a + 5*sizeof(int)
     printf("%d,%d",*(a+1),*(ptr-1));// 2,5
     

数组名 a 作为左值和右值的区别:   
     
     出现在赋值符“=”右边的就是右值，出现在赋值符“=”左边的就是左值。 比如,x=y
     
     左值：在这个上下文环境中，编译器认为 x 的含义是 x 所代表的地址。
           这个地址只有编译器知道，在编译的时候确定，编译器在一个特定的区域保存这个地址，
           我们完全不必考虑这个地址保存在哪里.
           
     右值：在这个上下文环境中，编译器认为 y 的含义是 y 所代表的地址里面的内容。
           这个内容是什么，只有到运行时才知道。
     
      a 作为右值时其意义与&a[0]是一样，代表的是 数组首元素的首地址，
      而不是数组的首地址。这是两码事。
      但是注意，这仅仅是代表，并没有一个地方（这只是简单的这么认为，其具体实现细节不作过多讨论）来存储这个地址，
      也就是说编译器并没有为数组 a分配一块内存来存其地址，这一点就与指针有很大的差别。
      
      a 不能作为左值，数组地址不能被改变。
      我们可以把 a[i]当左值，而无法把 a当左值。
      
## 4.3 数组与指针的关系
     数组就是数组，指针就是指针，它们是完全不同的两码事！他们之间没有任何关系，只是经常穿着相似的衣服来迷惑你罢了。

## 4.4 数组指针 与 指针数组
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/img/pointer_array.PNG)

    指向数组的指针： 数组指针
    存储指针的数组： 指针数组
```c
    int *p1[10];  // “[]”的优先级比“*”要高。 p1 先与“[]”结合，构成一个数组的定义.
                  // 数组的内容 由 int* 给出，即包含10个指向整形数据的指针的数组
    int (*p2)[10];// “（）”的优先级比“[]”高，“*”号和 p2 构成一个指针的定义,
                  // int[10] 这里表示的是一个包含10个整形数据的 数组
                  // 数组在这里并没有名字，是个匿名数组。
```
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/img/pointer_deep.PNG)

## 4.5 多维数组 多维空间 超过二维的数组和超过二级的指针其实并不多用
```c
char a[3][4];
// |a[0] - - - - | a[1] - - - - | a[2] - - - - |
// a[0],a[1],a[2]。 每个元素的大小为 sizeof(a[0]),即 sizeof(char)*4。 
// 由此可以计算出 a[0],a[1],a[2]三个元素的首地址分别为&a[0]， &a[0]+ 1*sizeof(char)*4， &a[0]+ 2*sizeof(char)*4。
//  a[i][j]的首地址为      &a[i] + j*sizeof(char)
//  a[i][j]元素的首地址为： a + i*sizof(char)*4 + j*sizof(char)。

// 多维数组定义 都是花括号
int a [3][2]={(0,1),(2,3),(4,5)};
//  花括号里面嵌套的是小括号，而不是花括号！
// 这里是花括号里面嵌套了逗号表达式！其实这个赋值就相当于
int a [3][2]={ 1, 3, 5};
```

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/img/pointer_deeper.PNG)

## 4.6 多级指针-二级指针-指针的指针
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/img/pointer_two.PNG)

## 4.7 数组与指针作为函数参数
```c
C 语言中，当一维数组作为函数参数的时候，编译器总是把它解析成一个指向其首元素首地址的指针。    
无法把指针变量本身传递给一个函数， 但可以改变指针指向的值。    
可以利用函数返回值，也可以传递二级指针，即 想改变的指针 的 地址。   

当数组超过一维时，将第一维改写为指向数组首元素首地址的指针之后，后面的维再也不可改写。  
比如： a[3][4][5]作为参数时可以被改写为（*p） [4][5]  
```
## 4.8 函数指针 函数指针数组
```c
char * fun3(char * p1,char * p2);    // fun3是函数，返回值为 char*类型    
char * *fun2(char * p1,char * p2);   // fun2也是函数, 返回值为 char** 类型     
char * (*fun1)(char * p1,char * p2); // fun1是函数指针，指向一个类型为 char*(char*,char*)的函数    
```
函数指针调用示例：
```c
#include <stdio.h>
#include <string.h>
char * fun(char * p1,char * p2)
{
     int i = 0;
     i = strcmp(p1,p2);
     if (0 == i)
     {
          return p1;
     }
     else
     {
          return p2;
     }
}
int main()
{
     char * (*pf)(char * p1,char * p2);// 定义函数指针
     pf = &fun;        // 赋值，初始化
     (*pf) ("aa","bb");// 调用函数指针指向的函数
     return 0;
}
```
### *(int*)&p 强转 指针地址类型，再解引用
```c
void Function()
{
printf("Call Function!\n");
}
int main()
{
     void (*p)(); // void(void)类型函数的指针 p, 
     // &p 是求指针变量 p 本身的地址，这是一个 32 位的二进制常数（32 位系统）。
     // (int*)&p 表示将地址强制转换成指向 int 类型数据的指针。
     // (int)Function 表示将函数的 入口 地址 强制转换成 int 类型 的 数据。
     *(int*)&p=(int)Function;// 表示将函数的入口地址赋值给指针变量 p。
     (*p) ();// 表示对函数的调用。
     return 0;
}
```
### (*(void(*) ())0)()
```c
void(*)() , 指向类型为 void(void)的函数的 函数指针
(void(*) ())0 ，将 0 强制转换为函数指针类型， 0 是一个地址，也就是说一个函数存在首地址为 0 的一段区域内。
(*(void(*) ())0)，取 0 地址开始的一段内存里面的内容，其内容就是保存在首地址为 0 的一段区域内的函数。
(*(void(*) ())0)()， 这是函数调用
同理 ：
(*(char**(*) (char **,char **))0) ( char **,char **);
```
### 函数指针数组 
```
char * (*pf)(char * p);   // 定义的是一个函数指针 pf, 指向一个类型为 char*(char*)的函数
char * (*pf[3])(char * p);// 定义一个函数指针数组, 数组名为 pf，数组内存储了 3 个指向函数的指针。
```
示例：
```c
#include <stdio.h>
#include <string.h>
char * fun1(char * p)
{
     printf("%s\n",p);
     return p;
}
char * fun2(char * p)
{
     printf("%s\n",p);
     return p;
}
char * fun3(char * p)
{
     printf("%s\n",p);
     return p;
}
int main()
{
     char * (*pf[3])(char * p);// pf 为 函数指针数组名
     // 为数组赋值
     pf[0] = fun1;  // 可以直接用函数名
     pf[1] = &fun2; // 可以用函数名加上取地址符
     pf[2] = &fun3;
     pf[0]("fun1");// 调用函数
     pf[1]("fun2");
     pf[2]("fun3");
     return 0;
}

```
### 函数指针数组的指针 
```c
char * (*pf)(char * p);   // 定义的是一个函数指针 pf, 指向一个类型为 char*(char*)的函数
char * (*pf[3])(char * p);// 定义一个 函数指针 数组, 数组名为 pf，数组内存储了 3 个指向函数的指针。
char * (*(*pf)[3])(char * p);// 定义一个指针，指向一个函数指针数组，该数组内存放3个 指向类型为char*(char*)的函数的函数指针。
```
用法：
```c
#include <stdio.h>
#include <string.h>
char * fun1(char * p)
{
     printf("%s\n",p);
     return p;
}
char * fun2(char * p)
{
     printf("%s\n",p);
     return p;
}
char * fun3(char * p)
{
     printf("%s\n",p);
     return p;
}
int main()
{
     char * (*pf[3])(char * p);// pf 为 函数指针 数组名
     char * (*(*pf)[3])(char * p);// 函数指针数组 的 指针
     pf = &a;
     // 为数组赋值
     a[0] = fun1;
     a[1] = &fun2;
     a[2] = &fun3;

     pf[0][0]("fun1");// 调用函数
     pf[0][1]("fun2");
     pf[0][2]("fun3");
     return 0;
}

```


# 5 内存管理
## 5.1 野指针 野猫 野狗 野孩子 熊孩子 NULL地址-狗链子
```c
野孩子：没人要，没人管的孩子；行为动作不守规矩，调皮捣蛋的孩子。    
野狗：没有主人的狗，没有链子锁着的狗，喜欢四处咬人。    
对付野孩子的最好办法是给他定一套规矩，好好管教。一旦发现没有按规矩办事就好.        
好收拾他。对付野狗最好的办法就是拿条狗链锁着它，不让它四处乱跑.       
对付也指针肯怕比对付野孩子或野狗更困难。   
我们需要把对付野孩子和野狗的办法都用上。既需要规矩，也需要链子。

前面我们把内存比作尺子，很轻松的理解了内存。
尺子上的 0 毫米处就是内存的 0 地址处，也就是 NULL 地址处。
这条栓“野指针”的链子就是这个“NULL”。
定义指针变量的同时最好初始化为 NULL，用完指针之后也将指针变量的值设置为 NULL。
也就是说除了在使用时，别的时间都把指针“栓”到 0 地址处。
这样它就老实了。
```
## 5.2 栈、堆和静态区
```c
一层教学楼，可能有外语教室，允许外语系学生和老师进入；
还可能有数学教师，允许数学系学生和老师进入；
还可能有校长办公室，允许校长进入。
同样，内存也是这样，内存的三个部分，不是所有的东西都能存进去的。

静态区：保存自动全局变量和 static 变量（包括 static 全局和局部变量）。
        静态区的内容在总个程序的生命周期内都存在，由编译器在编译的时候分配。
栈：保存局部变量。 栈上的内容只在函数的范围内存在，当函数运行结束，这些内容也会自动被销毁。
    其特点是效率高，但空间大小有限。
堆：由 malloc 系列函数或 new 操作符分配的内存。其生命周期由 free 或 delete 决定。
    在没有释放之前一直存在，直到程序结束。
    其特点是使用灵活，空间比较大，但容易出错。

```
## 5.3 常见内存错误
### 指针没有指向一块合法的内存
     定义了指针变量，但是没有为指针分配内存，即指针没有指向一块合法的内存。
```c
1. 结构体成员指针未初始化
#include <cstring>
#include <stdlib.h>
#include <iostream>
struct student
{
     char *name;
     int score;
}stu,*pstu;

int main()
{
     // 还没有为结构体分配内存
     //std::strcpy(stu.name,"Jimy");// 内存访问出错
     // stu.score = 99;

     struct student* pst = (struct student*)malloc(sizeof(struct student));
     pst->name = (char*)malloc(sizeof(char));// 这个好像不加上也可以，看编译器===
     std::strcpy(pst->name,"Jimy"); 
     pst->score = 99;
     
     std::cout << pst->name << std::endl;
     
     free(pst);
     return 0;
}


```
### 函数 入口 的 指针变量检测  使用 assert(NULL != p)对参数进行校验 

### 字符串拷贝 strlen(p1) 获取的长度 不包括结束符

```c
char *p1 = “abcdefg”;
// char *p2 = (char *)malloc(sizeof(char)*strlen(p1));// 未包括结束符
char* p2 = (char*)malloc(sizeof(char)*(strlen(p1)+1));// 
// 不要因为 char 类型大小为 1 个 byte 就省略 sizof（char）这种写法。这样只会使你的代码可移植性下降.
strcpy(p2,p1);

```
### memset 初始化内存
```c
int a[10];
memset(a,0,sizeof(a));
// memset 函数有三个参数，
// 第一个是要被设置的内存起始地址；
// 第二个参数是要被设置的值；
// 第三个参数是要被设置的内存大小，单位为 byte。
```
### 内存越界
```c
这种错误经常是由于操作数组或指针时出现“多 1”或“少 1”。比如：
int a[10] = {0};
for (i=0; i<=10; i++) // for (i=0; i<10; i++)  左闭右开写法=============
{
     a[i] = i;// 无 a[10]
}

```
### 内存泄漏 由malloc 系列函数或 new 操作符分配的内存。如果用完之后没有及时 free 或 delete，这块内存就无法释放，直到整个程序终止。

### malloc 返回指针 NULL 申请0字节内存

申请 0 字节内存，函数并不返回 NULL，而是返回一个正常的内存地址。  
但是你却无法使用这块大小为 0 的内存。  
这好尺子上的某个刻度，刻度本身并没有长度，只有某两个刻度一起才能量出长度。  
对于这一点一定要小心，因为这时候 if（NULL ！ = p）语句校验将不起作用。  


### malloc 与 free 配对使用 一定要一夫一妻制，不然肯定出错。
```c
malloc 两次只 free 一次会内存泄漏； malloc 一次 free 两次肯定会出错。
也就是说，在程序中 malloc 的使用次数一定要和 free 相等，否则必有错误。
这种错误主要发生在循环使用malloc 函数时，往往把 malloc 和 free 次数弄错了。
```

### 释放完块内存之后，没有把指针置 NULL，这个指针就成为了“野指针”，也有书叫“悬垂指针”。

这是很危险的，而且也是经常出错的地方。所以一定要记住一条： free 完之后，一定要给指针置 NULL。

# 6. 函数
## 函数头注释
```c
/************************************************************************
* Function Name : nucFindThread   函数名
* Create Date : 2000/01/07        创建日期
* Author/Corporation : your name/your company name   作者
*
* Description : Find a proper thread in thread array.  描述
* If it’s a new then search an empty.
*
* Param : ThreadNo： someParam description            参数
*         ThreadStatus： someParam description
*
* Return Code : Return Code description,eg:           返回值
ERROR_Fail: not find a thread
ERROR_SUCCEED: found
*
* Global Variable : DISP_wuiSegmentAppID
* File Static Variable : naucThreadNo
* Function Static Variable : None
*
*------------------------------------------------------------------------
* Revision History   版本信息
* No. Date Revised by Item Description
* V0.5 2008/01/07 your name … …
************************************************************************/
static unsigned char nucFindThread(unsigned char ThreadNo,unsigned char ThreadStatus)
{
…
}

```

## 复杂的函数中，在分支语句，循环语句结束之后需要适当的注释，方便区分各分支或循环体。
```c
while (condition)
{
     statement1;
     if (condition)
     {
          for(condition)
          {
               Statement2;
          }//end 'for(condition)'
     }
     else
     {
          statement3;
     }//'end if (condition)'
     statement4
}//end 'while (condition)'


```
## 修改别人代码的时候不要轻易删除别人的代码，应该用适当的注释方式
```C
while (condition)
{
     statement1;
     //////////////////////////////////////
     //your name , 2008/01/07 delete
     //if (condition)
     //{
     // for(condition)
     // {
     // Statement2;
     // }
     //}
     //else
     //{
     // statement3;
     //}
     ////////////////////////////////////////
     ///////////////////////////////////////
     // your name , 2000/01/07 add
          …
          new code
          …
     ///////////////////////////////////////
     statement4
}

```
### 递归函数的展开
     平时写代码，不到万不得已，尽量不要用递归。
     即便是要用递归，也要注意递归的层次不要太深，防止出现栈溢出的错误；
     同时递归的停止条件一定要正确，否则，递归可能没完没了。
     
```c
void fun(int i)
{
     if (i>0)
     {
          fun(i/2);
     }
     printf("%d\n",i);
}
int main()
{
     fun(10);
     return 0;
}
// 输出：
// 0 
// 1 
// 2
// 5
// 10
// 递归函数展开 =====================
void fun(int i)
{
     if (i>0)
     {
          //fun(i/2);
          if(i/2>0)
          {
               if(i/4>0)
               {
                    if(i/8>0)
                    {
                         if(i/16>0)
                         {
                         ... // 不执行
                         }
                         printf("%d\n",i/16);  // 0
                    }
                    printf("%d\n",i/8);// 1 
               }
               printf("%d\n",i/4); // 2
          }
          printf("%d\n",i/2); // 5
     }
printf("%d\n",i);// 10
}
```
# 7. 文件结构
## 文件头 
```c
/************************************************************************
* File Name : FN_FileName.c/ FN_FileName.h                      文件名
* Copyright : 2003-2008 XXXX Corporation,All Rights Reserved.   版权亦称“著作权”
* Module Name : Draw Engine/Display                             文件模块功能名称
*
* CPU : ARM7
* RTOS : Tron
*
* Create Date : 2008/10/01                                      创建日期
* Author/Corporation : WhoAmI/your company name                 作者
*
* Abstract Description : Place some description here.           文件描述信息
*
*-----------------------Revision History----------------------- 版本信息
* No Version Date Revised By Item Description
* 1 V0.95 08.05.18 WhoAmI abcdefghijklm WhatUDo
*
************************************************************************/

```





