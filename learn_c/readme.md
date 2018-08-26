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
* 4. case break default if else do while for continue 
* 5. register寄存器变量-最快-CPU皇帝同志的太监总管-中转站 
* 6. volatile易失型变量(确保本条指令不会因编译器的优化而省略，且要求每次直接从内存中读值) 
* 7. typedef别名 extern声明-未分配内存(喝稀粥)， 定义才分配了内存(抱伊人)  
* 8. void色即是空空即是色，世间万物  return返回 
* 9. goto无条件跳转语句，尽量少用，或不用 
* 10. sizeof 计算对象所占内存空间大小 
      sizeof 在计算变量所占空间大小时，括号可以省略，而计算类型(模子)大小时不能省略 
      int i; sizeof i; sizeof(i); sizeof(int)都可以； sizeof int;编译出错 
      
## 技巧
**bool 变量与“零值”进行比较**

          if(bTestFlag); if(!bTestFlag);
**float 变量与“零值”进行比较**
          
          if((fTestVal >= -EPSINON) && (fTestVal <= EPSINON)); //EPSINON 为定义好的精度
 **指针变量与“零值”进行比较**   
          
          int * p = NULL;//定义指针一定要同时初始化
          if(NULL == p); if(NULL != p);
**代码的缩进一般为 4 个字符，但不要使用 Tab 键，因为不同的编辑器 Tab 键定义
的空格数量不一样，别的编辑器打开 Tab 键缩进的代码可能会一片混乱。**

**if 语句使用建议**
          
          把正常情况的处理放在 if 后面，而不要放在 else 后面，else执行的会少一些。

**case 语句建议**
          
          case 后面只能是整型或字符型的常量或常量表达式（想想字符型数据在内存里是怎么存的）
          把正常情况放在前面，而把异常情况放在后面。
          把最常执行的情况放在前面，而把最不常执行的情况放在后面。
**do、while、for循环 语句建议**
          
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

**struct定义的柔性数组**
```c
typedef struct st_type
{
int i;
int a[]; // int a[0];
}type_a;
// 定义一个可变长的结构体，用 sizeof(type_a)得到的只有 4，就是 sizeof(i)=sizeof(int)。
// 给结构体分配内存
type_a *p = (type_a*)malloc(sizeof(type_a)+100*sizeof(int));
// 为结构体指针 p 分配了一块内存。用 p->item[n]就能简单地访问可变长元素。
// 我们再用 sizeof（*p）测试结构体的大小，发现仍然为 4。
// 在定义这个结构体的时候，模子的大小就已经确定不包含柔性数组的内存大小。
// 柔性数组只是编外人员，不占结构体的编制。


```
          
          
          
