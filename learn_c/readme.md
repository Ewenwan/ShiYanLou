# 学习 C 语言

## 鱼C工作室 >带你学C带你飞
[鱼C工作室 >带你学C带你飞](http://blog.fishc.com/category/c/page/7)

[C语言知识点整理](http://jinjaysnow.github.io/blog/2014-09/C%E8%AF%AD%E8%A8%80%E7%9F%A5%E8%AF%86%E7%82%B9%E6%95%B4%E7%90%86.html)

# 关键字
* 1. auto 自动变量声明
* 2. 整形 short int long  浮点型float double  字符型char signed unsigned   
     类型-模子---模具---（内存空间大小不一样）
* 3. struct结构体 union联合 enum枚举 static静态变量(内存静态区-唯一性-静态全局-静态局部-静态函数(内部函数))  
     const只读变量(常量)
* 4. case break default if else do while for continue
* 5. register寄存器变量-最快-CPU皇帝同志的太监总管-中转站
* 6. volatile易失型变量(确保本条指令不会因编译器的优化而省略，且要求每次直接读值)
* 7. typedef别名 extern声明-未分配内存(喝稀粥)， 定义才分配了内存(抱伊人) 
* 8. void return
* 9. goto无条件跳转语句
* 10. sizeof 计算对象所占内存空间大小
      sizeof 在计算变量所占空间大小时，括号可以省略，而计算类型(模子)大小时不能省略。
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
          
          
     

          
          
          
          
