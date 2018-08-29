# Standard Template Libraries
# 1. 容器
# 2. 迭代器
# 3. 算法

# 1. 函数模板、类模板 与 * 运算符 重载
```c
#include<iostream>
// 函数模板
template<typename T>
T square(T x){
  return x*x;
}
// 类模板
template<typename T>
class BoVector{
// class 默认为 provide 私有 类型
  T arry[1000];// 初始数组容量
  int size;    // 实际存储数据数量
public:
  BoVector(): size(0) {}; // 类默认构造函数 初始化 size=0
  void push(T x){ arry[size] = x; size++; } // 插入数据
  T get(int i) const {return arry[i]; }// 需要是否越界检查
  int getsize() const {return size;}
  void print() const { for(int i=0; i<size; i++){std::cout << arry[i] << std::endl;} }
};

// 类的BoVector  * 运算符 重载
template<typename T>
BoVector<T> operator*(const BoVector<T>& bv1, BoVector<T>& bv2){
  BoVector<T> ret;
  for(int i=0; i<bv1.getsize(); i++){
    ret.push(bv1.get(i) * bv2.get(i));
  }
  return ret;
}


int main(){
  std::cout << square(5) << std::endl;   // square<int>(5) 函数模板 模板类型可省略, 会自己的推断
  std::cout << square(5.5) << std::endl; // square<double>(5.5)

  BoVector<int> bv;// 类模板的模板类型不可省略
  bv.push(1);
  bv.push(2);
  bv.push(3);
  bv.push(4);
  bv.print();// bv = {1,2,3,4}

  bv = square(bv); // 模板函数 --> 乘法运算符重载
  bv.print();      // bv = {1,4,9,16}
}

```
