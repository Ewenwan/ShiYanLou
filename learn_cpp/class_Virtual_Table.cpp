/*
C++的多态性用一句话概括就是：
在基类的函数前加上virtual关键字，在派生类中重写该函数，运行时将会根据对象的实际类型来调用相应的函数。
如果对象类型是派生类，就调用派生类的函数；如果对象类型是基类，就调用基类的函数
Derive::f// 被覆盖的函数 存在于 对应被覆盖的 父类的 虚函数表中
Base1::g
Base1::h
Derive::g1// 子类新增添的 函数 只存在于 继承源自的 第一个子类的虚函数表中
Derive::h1// 子类新增添的 函数 只存在于 继承源自的 第一个子类的虚函数表中
1 
Derive::f// 被覆盖的函数 存在于 对应被覆盖的 父类的 虚函数表中
Base2::g
Base2::h
1 
Derive::f// 被覆盖的函数 存在于 对应被覆盖的 父类的 虚函数表中
Base3::g
Base3::h
0 

// 子类 Derive 继承于 父类3ase1 Base2 Base3
// 子类 内存空间 中的 虚函数表
 |Derive::f()|Base1::g()|Base1::h()|Derive::g1()|Derive::h1()|1|
 |Derive::f()|Base2::g()|Base2::h()|1|
 |Derive::f()|Base3::g()|Base3::h()|0|
////////////////////////////////////////
 
*/
#include <iostream>
using namespace std;
 
// 基类(父类) Base1
class Base1 {
public:
            virtual void f() { cout << "Base1::f" << endl; }
            virtual void g() { cout << "Base1::g" << endl; }
            virtual void h() { cout << "Base1::h" << endl; }
 
};

// 基类(父类) Base2 
class Base2 {
public:
            virtual void f() { cout << "Base2::f" << endl; }
            virtual void g() { cout << "Base2::g" << endl; }
            virtual void h() { cout << "Base2::h" << endl; }
};

// 基类(父类) Base3 
class Base3 {
public:
            virtual void f() { cout << "Base3::f" << endl; }
            virtual void g() { cout << "Base3::g" << endl; }
            virtual void h() { cout << "Base3::h" << endl; }
};
 
// 子类(派生类) 继承于 父类 Base1  Base2  Base3  子类Derive 有 继承自父类的函数 （3-1）*3 和 自己的3个函数
class Derive : public Base1, public Base2, public Base3 {
public:
            virtual void f() { cout << "Derive::f" << endl; }  // 覆盖了 3个父类的 虚函函数f()
            virtual void g1() { cout << "Derive::g1" << endl; }// 独有的  子类 函数
            virtual void h1() { cout << "Derive::h1" << endl; }// 独有的  子类 函数
};
 
typedef void(*Fun)(void);// 类型别名定义 Fun是函数指针 
// 指向一个函数类型为 void(void)的函数，即返回类型为void 形参参数也为void
 
int main()
{
            Fun pFun = NULL;//定义函数指针  初始化为 NULL
 
            Derive d;//子类
            int** pVtab = (int**)&d;// 虚函数表 函数指针数组
 
//父类Base1的 虚函数表 Base1's vtable
//  |Derive::f()|Base1::g()|Base1::h()|Derive::g1()|Derive::h1()|.|

            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+0);
            pFun = (Fun)pVtab[0][0];
            pFun();// 执行 Derive::f()   打印 Derive::f
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+1);
            pFun = (Fun)pVtab[0][1];
            pFun();// 执行 Base1::g()    打印 Base1::g
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+2);
            pFun = (Fun)pVtab[0][2];
            pFun();// 执行 Base1::h()    打印 Base1::h
   
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+3);
            pFun = (Fun)pVtab[0][3];
            pFun();// 执行 Derive::g1()   打印 Derive::g1
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+4);
            pFun = (Fun)pVtab[0][4];
            pFun();// 执行 Derive::h1()   打印 Derive::h1
            
            //虚函数表 尾部 结束符 |.|
            pFun = (Fun)pVtab[0][5];
            cout<<pFun<<endl;
 
 // 父类 Base2 虚函数表
 //|Derive::f()|Base2::g()|Base2::h()|.|
 //Base2's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+0);
            pFun = (Fun)pVtab[1][0];
            pFun();// 执行  Derive::f()  打印 Derive::f
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+1);
            pFun = (Fun)pVtab[1][1];
            pFun();// 执行  Base2::g()   打印 Base2::g

            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+2);
            pFun = (Fun)pVtab[1][2];
            pFun();// 执行  Base2::h()   打印 Base2::h
 
            //虚函数表 尾部 结束符 |.|
            pFun = (Fun)pVtab[1][3];
            cout<<pFun<<endl;
 

 // 父类 Base3 虚函数表
 //|Derive::f()|Base3::g()|Base3::h()|.|
 //Base3's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+2)+0);
            pFun = (Fun)pVtab[2][0];
            pFun();// 执行  Derive::f()  打印 Derive::f
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+2)+1);
            pFun = (Fun)pVtab[2][1];
            pFun();// 执行  Base3::g()   打印 Base3::g
 
            //pFun = (Fun)*((int*)*(int*)((int*)&d+2)+2);
            pFun = (Fun)pVtab[2][2];
            pFun();// 执行  Base3::h()   打印 Base3::h
 
           //虚函数表 尾部 结束符 |.|
            pFun = (Fun)pVtab[2][3];
            cout<<pFun<<endl;
 
            return 0;
}

