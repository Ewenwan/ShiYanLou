# 学习C++
    C++ 是一个难学易用的语言！
    C++ 的难学，不仅在其广博的语法，以及语法背後的语意，以及语意背後的深层思维，以及深层思维背後的物件模型；
    C++ 的难学，还在於它提供了四种不同（但相辅相成）的程式设计思维模式：
       基于程序procedural-based，基于对象object-based，面向对象object-oriented，泛型思想/设计generic paradigm。
    世上没有白吃的午餐。又要有效率，又要有弹性，又要前瞻望远，又要回溯相容，
    又要能治大国，又要能烹小鲜，学习起来当然就不可能太简单。
    
    C++ 相关书籍之多，车载斗量；如天上繁星，如过江之鲫。
    广博如四库全书者有之（The C++ Programming Language、C++ Primer），
    深奥如重山复水者有之（The Annotated C++ Reference Manual, Inside the C++ Object Model），
    细说历史者有之（The Design and Evolution of C++, Ruminations on C++），
    独沽一味者有之（Polymorphism in C++, Genericity in C++），
    独树一帜者有之（Design Patterns,Large Scale C++ Software Design, C++ FAQs），
    程式库大全有之（The C++ Standard Library），
    另辟蹊径者有之（Generic Programming and the STL），
    工程经验之累积亦有之（Effective C++, More Effective C++, Exceptional C++）。
    这其中，「工程经验之累积」对已具 C++ 相当基础的程式员而言，有著致命的吸引力与立竿见影的帮助。
    Scott Meyers 的 Effective C++ 和 More Effective C++ 是此类佼佼，
    Herb Sutter 的 Exceptional C++ 则是後起之秀。 
    
##  参考资料
[鸡啄米：C++编程入门系列之目录和总结 ](www.jizhuomi.com/software/129.html)

[++98基础上学习C++11新特性](https://www.cnblogs.com/skyfsm/p/9038814.html)

[Effective Modern C++](https://github.com/Ewenwan/Effective-Modern-Cpp-Zh)

[C++ 入门教程](https://www.ctolib.com/docs-cpp-getting-started-guide-c-85ddd5e55c8156076e75d45a2b33c972.html#)

[鱼C工作室 C++快速入门](http://blog.fishc.com/category/cpp/page/5)

[C++ Primer 5 代码](https://github.com/Ewenwan/Cpp-Primer)

[C++设计成这样的原因 《C++演化和设计》](http://vdisk.weibo.com/s/vfFpMc1XDsGn)

[boost库学习](https://www.cnblogs.com/lvdongjie/category/579434.html)

[C++17 High Performance](https://github.com/PacktPublishing/Cpp-High-Performance)

[C++17 STL Cookbook 代码](https://github.com/PacktPublishing/Cpp17-STL-Cookbook)

[C++  响应式编程（Reactive Programming）](https://github.com/Ewenwan/CPP-Reactive-Programming)

[C++ Template 进阶指南](https://github.com/wuye9036/CppTemplateTutorial)

[C++17 高性能计算](https://github.com/Ewenwan/Cpp-High-Performance)

[CPP-Data-Structures-and-Algorithms](https://github.com/Ewenwan/CPP-Data-Structures-and-Algorithms)

[数据结构和算法动态可视化](https://visualgo.net/zh)

[VS2010/MFC编程入门教程之目录和总结](www.jizhuomi.com/software/257.html)

# 基础议题
    pointers（指针）
    references（引用）
    casts（类型转换）
    arrays（数组）
    constructors（构造） default constructors（默认构造函数）
    
## 指针与引用的区别
    指针与引用看上去完全不同（指针用操作符“*”和“->”，引用使用操作符“. ”），
    但是它们似乎有相同的功能。指针与引用都是让你间接引用其他对象。
    
    引用内部实现为常量指针
    
```c
string& rs; // 错误，引用必须被初始化
string s("xyzzy");
string& rs = s; // 正确，rs 指向 s
指针没有这样的限制。
string *ps; // 未初始化的指针
 // 合法但危险
```
不存在指向空值的引用这个事实意味着使用引用的代码效率比使用指针的要高。
因为在使用引用之前不需要测试它的合法性。
```c
void printDouble(const double& rd)
{
 cout << rd; // 不需要测试 rd,它
} // 肯定指向一个 double 值

// 相反，指针则应该总是被测试，防止其为空：
void printDouble(const double *pd)
{
 if (pd) { // 检查是否为 NULL
    cout << *pd;
  }
}
```
指针与引用的另一个重要的不同是指针可以被重新赋值以指向另一个不同的对象。但是
引用则总是指向在初始化时被指定的对象，以后不能改变。
```c
string s1("Nancy");
string s2("Clancy");
string& rs = s1; // rs 引用 s1
string *ps = &s1; // ps 指向 s1
rs = s2; // rs 仍旧引用 s1,
 // 但是 s1 的值现在是 "Clancy"
 
ps = &s2; // ps 现在指向 s2;
 // s1 没有改变 
 
```
    在以下情况下你应该使用指针，
     一是你考虑到存在不指向任何对象的可能（在这种情况下，你能够设置指针为空），
     二是你需要能够在不同的时刻指向不同的对象（在这种情况下，你能改变指针的指向）。
     
    如果总是指向一个对象并且一旦指向一个对象后就不会改变指向，那么你应该使用引用。 

## 尽量使用 C++风格的类型转换 
    仔细想想地位卑贱的类型转换功能（cast），其在程序设计中的地位就象 goto 语句一样令人鄙视。
    但是它还不是无法令人忍受，因为当在某些紧要的关头，类型转换还是必需的，这时它是一个必需品。
    
    C风格的类型转换,过于粗鲁，能允许你在任何类型之间进行转换.
    C风格的类型转换在程序语句中难以识别。在语法上，类型转换由圆括号和标识符组成，而这些可以用在 Cpp中的任何地方。
    C++通过引进四个新的类型转换操作符克服了 C 风格类型转换的缺点，这四个操作符是,
    
    static_cast,
    const_cast,
    dynamic_cast, 
    和 reinterpret_cast。
    
    c强制类型转换  (type) expression
    
    1. static_cast
    
       static_cast<type>(expression) 
        a. 基础类型之间互转。如：float转成int、int转成unsigned int等。
        
            int firstNumber, secondNumber;
            ...
            double result = ((double)firstNumber)/secondNumber; // c风格 
            如果用上述新的类型转换方法，你应该这样写：
            double result = static_cast<double>(firstNumber)/secondNumber;// c++风格
            
            static_cast 不能从表达式中去除 const 属性，
            因为另一个新的类型转换操作符 const_cast 有这样的功能。
            const_cast 最普通的用途就是转换掉对象的 const 属性。

        b. 指针与void*之间互转。如：float*转成void*、CBase*转成void*、函数指针转成void*、void*转成CBase*等
        c. 派生类指针【引用】转成基类指针【引用】。如：Derive*转成Base*、Derive&转成Base&等
        d. 非virtual继承时，可将基类指针【引用】转成派生类指针【引用】（多继承时，会做偏移处理）。
           如：Base*转成Derive*、Base&转成Derive&等
```c
class Widget { ... };
class SpecialWidget: public Widget { ... };
void update(SpecialWidget *psw);
SpecialWidget sw; // sw 是一个非 const 对象。
const SpecialWidget& csw = sw; // csw 是 sw 的一个引用
 // 它是一个 const 对象
update(&csw); // 错误!不能传递一个 const SpecialWidget* 变量
 // 给一个处理 SpecialWidget*类型变量的函数
 
             // 2.const_cast<type>(expression)  转换掉对象的 const 属性====
            
update(const_cast<SpecialWidget*>(&csw));
 // 正确，csw 的 const 被显示地转换掉（
 // csw 和 sw 两个变量值在 update
 //函数中能被更新）
update((SpecialWidget*)&csw);
 // 同上，但用了一个更难识别
 //的 C 风格的类型转换
Widget *pw = new SpecialWidget;
update(pw); // 错误！pw 的类型是 Widget*，但是 
            // update 函数处理的是 SpecialWidget*类型
            

update(const_cast<SpecialWidget*>(pw));
 // 错误！const_cast 仅能被用在影响
 // constness or volatileness 的地方上。,
 // 不能用在向继承子类进行类型转换。 


```

    3. dynamic_cast  
        dynamic_cast<type>(expression) 
        专门用于处理多态机制，对继承体系内的对象（类中必须含有至少一个虚函数）
        的指针【引用】进行转换，转换时会进行类型检查.
        它被用于安全地沿着类的继承关系向下进行类型转换。
        用 dynamic_cast 把指向基类的指针或引用转换成指向其派生类或其兄弟类的指针或引用，而且你能知道转换是否成功。
        失败的转换将返回空指针（当对指针进行类型转换时）或者抛出异常（当对引用进行类型转换时）： 
```c
Widget *pw; // 基类 对象 指针
...
update(dynamic_cast<SpecialWidget*>(pw));
 // 正确，传递给 update 函数一个指针
 // 是指向变量类型为 SpecialWidget 的 pw 的指针
 // 如果 pw 确实指向一个对象,
 // 否则传递过去的将使空指针。
void updateViaRef(SpecialWidget& rsw);
updateViaRef(dynamic_cast<SpecialWidget&>(*pw));
 //正确。 传递给 updateViaRef 函数
 // SpecialWidget pw 指针，如果 pw
 // 确实指向了某个对象
 // 否则将抛出异常 

int firstNumber, secondNumber;
...
double result = dynamic_cast<double>(firstNumber)/secondNumber;
 // 错误！没有继承关系，想在没有继承关系的类型中进行转换，你可能想到 static_cast。
const SpecialWidget sw;
...
update(dynamic_cast<SpecialWidget*>(&sw));
 // 错误! dynamic_cast 不能转换掉 const。
 // 为了去除const，你总得用 const_cast。 
```
        
        
        4.reinterpret_cast   重新解释
        reinterpret_cast <new_type> (expression)
        用来处理无关类型之间的转换；
        它会产生一个新的值，这个值会有与原始参数（expressoin）有完全相同的比特位.
        字面意思：重新解释（类型的比特位）
            a.从指针类型到一个足够大的整数类型
            b.从整数类型或者枚举类型到指针类型
            c.从一个指向函数的指针到另一个不同类型的指向函数的指针
            d.从一个指向对象的指针到另一个不同类型的指向对象的指针
            e.从一个指向类函数成员的指针到另一个指向不同类型的函数成员的指针
            f.从一个指向类数据成员的指针到另一个指向不同类型的数据成员的指针
            
        使用reinterpret_casts 的代码很难移植。
        reinterpret_casts 的最普通的用途就是在函数指针类型之间进行转换。
```c  
typedef void (*FuncPtr)(); // FuncPtr is 一个指向函数的指针，该函数没有参数
                           // 返回值类型为 void
FuncPtr funcPtrArray[10]; // funcPtrArray 是一个能容纳10 个 FuncPtrs 指针的数组 

//  如果要把一个指向下面函数的指针存入 funcPtrArray 数组：
// int doSomething();      

// 你不能不经过类型转换而直接去做，因为 doSomething 函数对于 funcPtrArray 数组来说有一个错误的类型。
// 在 FuncPtrArray 数组里的函数返回值是 void 类型，而 doSomething函数返回值是 int 类型。
funcPtrArray[0] = &doSomething; // 错误！类型不匹配
// reinterpret_cast 可以让你迫使编译器以你的方法去看待它们：
funcPtrArray[0] = // 可编译通过
       reinterpret_cast<FuncPtr>(&doSomething);
```

可以用下面的宏替换来模拟新的类型转换语法：
```c
#define static_cast(TYPE,EXPR) ((TYPE)(EXPR))   // 后面为 c语言强转方式
#define const_cast(TYPE,EXPR) ((TYPE)(EXPR))
#define reinterpret_cast(TYPE,EXPR) ((TYPE)(EXPR))
```
你可以象这样使用使用：
```c
double result = static_cast(double, firstNumber)/secondNumber; 
update(const_cast(SpecialWidget*, &sw));
funcPtrArray[0] = reinterpret_cast(FuncPtr, &doSomething); 


#define dynamic_cast(TYPE,EXPR) (TYPE)(EXPR)
// 请记住，这个模拟并不能完全实现 dynamic_cast 的功能，它没有办法知道转换是否失败。
``` 

## 不要对数组使用多态 
    类继承的最重要的特性是你可以通过基类(父类) 指针或引用 来 操作 派生类(子类)。
    多态和指针算法不能混合在一起来用，所以数组与多态也不能用在一起。 
    
## 避免无用的缺省构造函数
    在一个完美的世界里，无需任何数据即可建立对象的类可以包含缺省构造函数，
    而需要数据来建立对象的类则不能包含缺省构造函数。
    唉！可是我们的现实世界不是完美的，所以我们必须考虑更多的因素。
    特别是如果一个类没有缺省构造函数，就会存在一些使用上的限制。 
    
    
    
    
    
    

# C++类成员和数据成员初始化总结
    C++为类中提供类成员的初始化列表
    类对象的构造顺序是这样的：
    1.分配内存，调用构造函数时，隐式／显示的初始化各数据成员
    2.进入构造函数后在构造函数中执行一般计算
    规则：
      1.类里面的任何成员变量在定义时是不能初始化的。
      2.一般的数据成员可以在构造函数中初始化。
      3.const数据成员必须在构造函数的初始化列表中初始化。
      4.static要在类的定义外面初始化。   
      5.数组成员是不能在初始化列表里初始化的。
      6.不能给数组指定明显的初始化。  
      
    这6条一起，说明了一个问题：C++里面是不能定义常量数组的！
    因为3和5的矛盾。这个事情似乎说不过去啊？没有办法，我只好转而求助于静态数据成员。
    
    到此，我的问题解决。
    但是我还想趁机复习一下C++类的初始化：
      1.初始化列表：CSomeClass::CSomeClass() : x(0), y(1){}
      2.类外初始化：int CSomeClass::myVar=3;
      3.const常量定义必须初始化，C++类里面使用初始化列表;
      4.C++类不能定义常量数组。

#  浅拷贝：只是将数据成员的值进行简单的拷贝
#  深拷贝：在浅拷贝的基础上，也将堆中的数据也进行拷贝


#    C++  泛型技术  泛化技术  增加不确定性  通用性  灵活性

> 所谓泛型技术，说白了就是试图使用不变的代码来实现可变的算法

> 比如：模板技术，RTTI技术，虚函数技术

> 要么是试图做到在编译时决议，要么试图做到运行时决议。


## 【【A】】 RTTI技术
    RTTI(Run-Time Type Identification)是面向对象程序设计中一种重要的技术。
    
现行的C++标准对RTTI已经有了明确的支持。不过在某些情况下出于特殊的开发需要，

我们需要自己编码来实现。本文介绍了一些关于RTTI的基础知识及其原理和实现。

    RTTI需求：
　　和很多其他语言一样，C++是一种静态类型语言。其数据类型是在编译期就确定的，
  
不能在运行时更改。然而由于面向对象程序设计中多态性的要求，C++中的指针或引用

(Reference)本身的类型，可能与它实际代表(指向或引用)的类型并不一致。有时我们需

要将一个多态指针转换为其实际指向对象的类型，就需要知道运行时的类型信息，

这就产生了运行时类型识别的要求。


      C++对RTTI的支持：
    
      C++提供了两个关键字typeid（指示类型） 和dynamic_cast（类型强转）和一个type_info类来支持RTTI

#############################################################
###【1】dynamic_cast操作符：     运行时强制类型转换
它允许在运行时刻进行类型转换，
从而使程序能够在一个类层次结构安全地转换类型。
dynamic_cast提供了两种转换方式，
把基类指针转换成派生类指针，
或者把指向基类的左值转换成派生类的引用。
见下例讲述：

    void company::payroll(employee *pe) {//指针

      //对指针转换失败，dynamic_cast返回NULL

      if(programmer *pm=dynamic_cast(pe)){  //基类 employee >>> 派生类 programmer

        pm->bonus(); 

       }

    }
    void company::payroll(employee &re) {//引用  变量别名

    try{

    //对引用转换失败的话，则会以抛出异常来报告错误

      programmer &rm = dynamic_cast(re);

      rm->bonus();
    }

    catch(std::bad_cast){
      }
    }
这里bonus是programmer的成员函数，基类employee不具备这个特性。

所以我们必须使用安全的由基类到派生类类型转换，识别出programmer指针。



    int a=1;int *p=&a;//指针是变量的地址    *p  定义时  和 在函数参数中时  是 表示指针变量   其他表示取值

    int a=1;int &b=a;//引用 是 变量别名     &放在左边 以及在 函数参数中 是引用   方在右边是 取地址

上面定义了一个整形变量和一个指针变量p，该指针变量指向a的存储单元，

即p的值是a存储单元的地址。

而下面2句定义了一个整形变量a和这个整形a的引用b，

事实上a和b是同一个东西，在内存占有同一个存储单元。

区别：
【1】可以有const指针，但是没有const引用；

【2】指针可以有多级，但是引用只能是一级（int **p；合法 而 int &&a是不合法的；

【3】指针的值可以为空，但是引用的值不能为NULL，并且引用在定义的时候必须初始化；

【4】指针的值在初始化后可以改变，即指向其它的存储单元，而引用在进行初始化后就不会再改变了；

【5】"sizeof引用"得到的是所指向的变量(对象)的大小，而"sizeof指针"得到的是指针本身的大小；

【6】指针和引用的自增(++)运算意义不一样；

【7】引用作为函数的参数进行传递，传递的是实参本身，不是实参的一个拷贝；

【8】 用指针传递参数，可实现对实参进行改变的目的，是因为传递过来的是实参的地址，但是指针不会改变。


    #include<iostream>
    using namespace std;

    void test(int *&p)//这里是 指针p的引用  ；如果是 *p 指针 p修改不了 可以修改p指向的内容
    {
      int a=1;
      p=&a;//可以修改p   这里的& 是取地址
      cout<<p<<" "<<*p<<endl;//这里的*是取值
    }

    int main(void)
    {
        int *p=NULL;//这里的 *是 指针变量定义
        test(p);
        if(p!=NULL)
        cout<<"指针p不为NULL"<<endl;
        system("pause");
        return 0;
    }



### 【2】typeid操作符：它指出指针或引用指向的对象的实际派生类型。

例如：

    employee* pe=new manager;

    typeid(*pe) == typeid(manager) //等于true

    typeid的返回是type_info类型。

    class type_info {

    private:

       type_info(const type_info&);

       type_info& operator=( const type_info& );

    public:

       virtual ~type_info();

       int operator==( const type_info& ) const;

       int operator!=( const type_info& ) const;

       const char* name() const;
    };

##############################
## 【【B】】模板

### 【1】函数模板

#### 函数的重载。例如:
    int add(int a, int b)  
    {  
        return a + b;  
    }  
    double add(double a, double b)  
    {  
        return a + b;  
    }  
    char add(char a, char b)  
    {  
        return a + b;  
    }  
这些函数几乎相同，每个函数的函数体是相同的，功能也是相同的，
它们之间唯一的不同在于形参的类型和函数返回值的类型。

C++有模板(template)机制，可以使用函数模板解决上述存在的问题。
函数模板(function template)是一个独立于类型的函数，
可作为一种模式，产生函数的特定类型版本。

    template<模板形参表>返回值类型 函数名(形式参数列表)  
    {  
        函数体  
    }  

模板形参表(template parameter list)是用一对尖括号<>括起来的
一个或多个模板形参的列表，不允许为空，形参之间以逗号分隔。

    第一种形式如下所示:  
         <typename 类型参数名1,typename 类型参数名2，..>

    第二种形式如下所示:  
         <class 类型参数名1,class 类型参数名2，...> 

在函数模板形参表中，typename和class具有相同含义，可以互换使用，

或者两个关键字都可以在同一模板形参表中使用。

不过由于C++中class关键字往往容易与类联系在一起，

所以使用关键字typename比使用class更直观，

typename可以更加直观的反映出后面的名字是一个类型名。

模板定义的后面是函数定义，在函数定义中，可以使用模板形参表中的类型参数。

例如:
    template<typename T>T add(T a, T b)  
    {  
        return a + b;  
    }  

函数模板定义语法的含义是一个通用型函数，

这个函数类型和形参类型没有具体的指定，而是一个类型记号表示

，类型记号由编译器根据所用的函数而确定，这种通用型函数成为函数模板。


    #include<iostream>  

    using namespace std;  

    template<typename T>T add(T a, T b)//函数模板
    {  
        return a + b;  
    }  

    int main()  
    {  
        std::cout << "int_add    = "    << add(10,20)<< std::endl;  
        std::cout << "double_add = "    << add(10.2, 20.5) << std::endl;  
        std::cout << "char_add   = "    << add(10, 20) << std::endl;  
        std::system("pause");  
        return 0;  
    }


### 【2】类模板

类似于函数模板的做法，类模板对数据成员的

数据类型和成员函数的参数类型进行泛化。

如下是类模板的一个基本定义形式，

关键字template说明类型T1~Tn是模本类型， typename 或 class关键字 

成员函数可在类模板的声明中定义。

    template<class T1,class T2, ... ,class Tn> class 类名  
    {  
        //数据成员声明或定义;  
    };  
    template<class T1, class T2, ....., class Tn> 返回值 类名<T1,T2, ....., Tn>::成员函数1  
    {  
        //函数定义  
    }  
    template<class T1, class T2, ....., class Tn> 返回值 类名<T1, T2, ....., Tn>::成员函数2  
    {  
        //函数定义  

    }  

不同于非模板代码的组织方式，函数模板和类模板的声明和定义代码，

一般都编写在.h头文件中，以免由于为具现而提示编译链接错误。

下面给出一个类模板表示平面上点的示例:

    template<class T> //【0】类模板定义 

    class Point       //【1】Point不是类名是模板名  

    {  

    public:  

    Point::x(0), y(0) {} //【2】默认构造函数  初始化为0 
    
    Point(const T a, const T b) :(x)(a), y(b) {}//【3】带参数的构造函数    a赋值给x b赋值给y 
    
    void Set(const T a, const T b);  
    
    void Display();  
    
    private:  

        T x;  

        T y;  
    }; 


## 【【C】】虚函数技术
[参考](https://blog.csdn.net/haoel/article/details/1948051)

C++中的虚函数的作用主要是实现了多态的机制。

关于多态，简而言之就是用父类型别的指针指向其子类的实例

，然后通过父类的指针调用实际子类的成员函数。

这种技术可以让父类的指针有“多种形态”.

```c
class A{
    int a;
    // a的后面直接是虚函数，内存中存在为虚函数表指针    
 public:
    virtual void f();
    virtual void g(int);
    virtual void h(double);
};

class B: public A{
 public:
    int b;
    void g(int);// 会覆盖 A类的 A::g
    virtual void m(B*);
};

class C: public B{
 public:
    int c;
    void h(double);// 会覆盖 B的父类A 的 A:h
    virtual void n(C*);
}

&C, 类C的实例的内存空间大概如下：
变量a
虚函数表指针vptr  ------>  |&A::f|&B::g|&C::h|&B::m||&C::n|.|  按继承的先后顺序存放函数
变量b
变量c

```




### 虚函数表
 
对C++ 了解的人都应该知道虚函数（Virtual Function）

是通过一张虚函数表（Virtual Table）来实现的。简称为V-Table。

这个表中，主是要一个类的虚函数的地址表，这张表解决了继承、覆盖的问题，

保证其容真实反应实际的函数。这样，在有虚函数的类的实例中这个表被分配在了

这个实例的内存中，所以，当我们用父类的指针来操作一个子类的时候，

这张虚函数表就显得由为重要了，它就像一个地图一样，指明了实际所应该调用的函数。


    class Base {
         public:
                virtual void f() { cout << "Base::f" << endl; }
                virtual void g() { cout << "Base::g" << endl; }
                virtual void h() { cout << "Base::h" << endl; }

    };

我们可以通过Base的实例来得到虚函数表。 下面是实际例程：
 
            typedef void(*Fun)(void);//函数指针  Fun  void(void) 返回值为void、输入值为void
            Base b;// 定义Base类 的实例b 
            
            Fun pFun = NULL;// 定义一个函数指针Fun 变量 pFun， 初始化为 NULL
            cout << "虚函数表地址：" << (int*)(&b) << endl;//  &b 取地址 (int*) 强制转换成int类型的指针（表id）

            cout << "虚函数表 — 第一个函数地址：" << (int*)*(int*)(&b) << endl;
            // (int*)(&b) 虚函数表地址  *(int*)(&b) 取虚函数表地址 内的内容 为 虚函数地址  (int*) 强制转换成int类型的指针

            pFun = (Fun)*((int*)*(int*)(&b));//得到第一个函数 *((int*)*(int*)(&b))
            pFun();
 
实际运行经果如下：
 
虚函数表地址：0012FED4

虚函数表 — 第一个函数地址：0044F148

Base::f

            (Fun)*((int*)*(int*)(&b)+0);  // Base::f()
            (Fun)*((int*)*(int*)(&b)+1);  // Base::g()
            (Fun)*((int*)*(int*)(&b)+2);  // Base::h()

|Base::f()|Base::g()|Base::h()|.|

    对象的内存布局 和 虚函数表 ：
    
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/base_class_virtual_table.PNG)

    (&b) 对象的地址
    (int*)(&b) 强行把&b 转成int*，取得 虚函数表 的地址
    (int*)*(int*)(&b)  解引用后再强转 得到 虚函数 的地址

    注意：在上面这个图中，我在虚函数表的最后多加了一个结点，
    这是虚函数表的结束结点，就像字符串的结束符“/0”一样，
    其标志了虚函数表的结束。这个结束标志的值在不同的编译器下是不同的。
    在WinXP+VS2003下，这个值是NULL。
    而在Ubuntu 7.10 + Linux 2.6.22 + GCC 4.1.3下，这个值如果是1，
    表示还有下一个虚函数表，如果值是0，表示是最后一个虚函数表。
    
    
    分别说明“无覆盖”和“有覆盖”时的虚函数表的样子。
    没有覆盖父类的虚函数是毫无意义的。
    我之所以要讲述没有覆盖的情况，主要目的是为了给一个对比。
    在比较之下，我们可以更加清楚地知道其内部的具体实现。
    

#### 【1】一般继承（无虚函数覆盖,子类中定义的函数名与父类中的不同）

假设有如下所示的一个继承关系：

Derive 类 继承 父类 Base

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/inheritance_class.PNG)

在这个继承关系中，子类没有重载任何父类的函数。那么，在派生类的实例中，

对于子类对象的实例：Derive d; 的虚函数表如下：

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/inheritance_class_virtual_table.PNG)

我们可以看到下面几点：

1）虚函数按照其声明顺序放于表中。

2）父类的虚函数在子类的虚函数前面。

    |Base::f()|Base::g()|Base::h()|Derive::f1()|Derive::g1()|Derive::h1()|.|


#### 【2】一般继承（有虚函数覆盖）
如果子类中有虚函数重载了父类的虚函数   f()

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_over.PNG)

1）子类覆盖的f()函数被放到了虚表中原来父类虚函数的位置。

2）没有被覆盖的函数依旧。

    |Derive::f1()|Base::g()|Base::h()|Derive::g1()|Derive::h1()|.|

这样，我们就可以看到对于下面这样的程序，
 
            Base *b = new Derive();// 父类指针Base* b 指向了子类 Derive()
            b->f();// 会调用子类中覆盖了父类的同名的函数 Derive::f()

子类 Derive 的虚函数表：

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_over_virtual_table.PNG)

由b所指的内存中的虚函数表的f()的位置已经被 Derive::f() 函数地址所取代，

于是在实际调用发生时，是Derive::f()被调用了。这就实现了多态。


#### 【3】多重继承（无虚函数覆盖）
Derive 继承 于 Base1 Base2 Base3

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_more_class.PNG)

     |Base1::f()|Base1::g()|Base1::h()|Derive::f1()|Derive::g1()|Derive::h1()|.|
     
     |Base2::f()|Base2::g()|Base2::h()|.|
     
     |Base3::f()|Base3::g()|Base3::h()|.|

子类内存空间：

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_more_class_virtual_table.PNG)

我们可以看到：

1）  在子类内存空间中，存在每个父类的虚表。

2）  子类的成员函数被放到了第一个父类的表中。（所谓的第一个父类是按照声明顺序来判断的）

这样做就是为了解决不同的父类类型的指针指向同一个子类实例，而能够调用到实际的函数。


#### 【4】多重继承（有虚函数覆盖）
 Derive 继承 于 Base1 Base2 Base3   且有 同名函数 f()
![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_more_class_over.PNG)

     |Derive::f()|Base1::g()|Base1::h()|Derive::g1()|Derive::h1()|.|
     |Derive::f()|Base2::g()|Base2::h()|.|
     |Derive::f()|Base3::g()|Base3::h()|.|

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_cpp/img/class_derive_more_class_over_virtual_table.PNG)

 三个父类虚函数表中的f()的位置被替换成了子类的函数指针。
 
 这样，我们就可以 用 任一静态类型的父类来指向 子类，并调用子类的f()了。
 
 如：

            Derive d;// 子类
            Base1 *b1 = &d;// 父类1的指针 b1 指向子类d
            Base2 *b2 = &d;// 父类2的指针 b2 指向子类d
            Base3 *b3 = &d;// 父类3的指针 b3 指向子类d
            b1->f(); // Derive::f()  之类d的内存空间中 三个子类的 虚函数表 的第一个函数都是 Derive::f()
            b2->f(); // Derive::f()
            b3->f(); // Derive::f()
 
            b1->g(); //Base1::g()
            b2->g(); //Base2::g()
            b3->g(); //Base3::g()
            
            b1->g(); //Base1::h()
            b2->g(); //Base2::h()
            b3->g(); //Base3::h()


#### 【5】安全性
##### 一、通过父类型的指针访问子类自己的虚函数  会出错
          Base1 *b1 = new Derive();
            b1->g1();  //编译出错   g1() 为子类自己的虚函数
            b1->h1();  //编译出错   h1() 为子类自己的虚函数
            
任何妄图使用父类指针想调用子类中的未覆盖父类的成员函数的行为都会被编译器视为非法。


##### 二、访问non-public (private 或者 protected)的虚函数

如果父类的虚函数是private或是protected的，但这些非public的虚函数同样会存在于虚函数表中，

所以，我们同样可以使用访问虚函数表的方式来访问这些non-public的虚函数，这是很容易做到的。

    class Base {
        private:
                virtual void f() { cout << "Base::f" << endl; } 
    };
    class Derive : public Base{// 子类Derive 继承 于 Base父类
    };
    typedef void(*Fun)(void);  // 函数指针

    void main() {
        Derive d;
        Fun  pFun = (Fun)*((int*)*(int*)(&d)+0);//通过虚函数指针 调用父类的 私有虚函数
        pFun();// 会打印 Base::f
    }


下面是一个关于多重继承的虚函数表访问的例程：
[多重继承的虚函数表访问的例程](class_Virtual_Table.cpp)
