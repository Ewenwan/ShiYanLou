/*
类构造 与析构函数的自动执行

Eigen 类 noalias() 操作的 简化对象构造机制

输出：
init A()
init A()
init A()
init A()
delet ~A()
3 4
1 2
delet ~A()
delet ~A()
delet ~A()

CMakeLists.txt:
cmake_minimum_required( VERSION 2.8)
project(class_init_delet)
include_directories("/usr/include/eigen3")
add_executable(class_i_d calss_init_dead.cpp)

编译：
mkdir build
cd build
cmake ..
make 

运行：
./class_i_d
 
*/
#include <iostream>
#include <stdio.h>

#include <eigen3/Eigen/Dense>

using namespace std;

template <typename T>
// 类A
class A {
public:
    // 类构造函数 定义类时 自动执行
    A() {printf("init A()\n");}
    // 类析构函数 对象销毁时 自动执行
    ~A() {printf("delet ~A()\n");}
    // 乘法运算符 重载 
    template <typename U>
    A operator*(A<U>&){
        A a;// 定义一个对象 会自动执行 类构造函数  局部变量(对象)
        return a;
    }// 离开函数时  局部变量会被销毁

};

int main()
{
    A<int> a1,b1;// 定义类A的两个对象 会自动执行2次 构造函数
    A<int> c1;   // 会自动执行 1次 构造函数
    c1 = a1*b1;  // 调用乘法运算符重载函数 会定义一个局部对象 返回时 会销毁一个对象

    Eigen::Matrix2f a, b, c;

    c.noalias() = a * b;
    a(0, 0) = 0;
    a(0, 1) = 1;
    a(1, 0) = 1;
    a(1, 1) = 0;
    b(0, 0) = 1;
    b(0, 1) = 2;
    b(1, 0) = 3;
    b(1, 1) = 4;
    c.noalias()  = a * b;
    std::cout << c << std::endl;

    return 0;
}// 程序结束时   a1,b1,c1 三个对象 会被销毁 会自动执行三次析构函数 
// 而 Eigen 定义的对象 不会执行 
