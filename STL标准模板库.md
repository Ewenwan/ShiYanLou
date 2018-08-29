# Standard  Library 标准库
# Standard Template Library 标准模板库
## 1. 容器   Containers
## 2. 迭代器 Iterators
## 3. 算法   Algorithms

# 1. 函数模板、类模板 与 * 乘法运算符 重载
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

# 容器 迭代器 算法 示例
```c
#include<iostream>// std::cout & std::endl 
#include<vector>  // std::vector
#include<algorithm>// std::sort
using namespace std;

int main(){
// 容器========
	vector<int> vec_i;
	vec_i.push_back(5);
	vec_i.push_back(2);
	vec_i.push_back(0); // vec_i = (5, 2, 0);
// 迭代器======
	vector<int>::iterator iter_beg = vec_i.begin(); // [begin, end) 左闭右开区间
	vector<int>::iterator iter_end = vec_i.end();

    for(vector<int>::iterator it = iter_beg; it!=iter_end; ++it){
		cout << *it << endl;// vec_i = (5, 2, 0);
	}
// 算法========
	sort(iter_beg+1, iter_end);//vec_i = (5, 0, 2);
	
    for(vector<int>::iterator it = iter_beg; it!=iter_end; ++it){
		cout << *it << endl;
	}
	
	sort(iter_beg, iter_end);//vec_i = (0, 2, 5);
	for(vector<int>::iterator it = iter_beg; it!=iter_end; ++it){
		cout << *it << endl;
	}
	
	return 0;
}
```

