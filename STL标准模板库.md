# Standard  Library 标准库
# Standard Template Library 标准模板库
## 1. 容器   containers
## 2. 迭代器 iterators
## 3. 算法   algorithms  
## 4. 函数器 functional



# 0.1 函数模板、类模板 与 * 乘法运算符 重载
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

# 0.2容器 迭代器 算法 示例
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
        // 算法 通过 迭代器 对 容器元素 进行操作 !!!!!!!!!!!!!!!
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
# 1. 容器
## 1.1 序列容器
	vector向量 deque双向队列 双向链表list 单向链表forward_list array数组
	#include <vector> 
	#include <deque>
	#include <list>
	#include <forward_list>
	#include <array>
### a. vector向量  动态数组
        |||  ........... -----> 
```c

/*
1. 在向量尾部 插入/删除元素 特别快 O(1)
2. 在向量头部 或者 中间插入元素 特别慢 O(n)
3. 在向量中查找一个元素特别慢 O(n)
|||  ........... ----->
头            尾
*/
#include<iostream>
#include<vector>
using namespace std;

int main(){

	vector<int> vec_i;  // vec_i.size() == 0
	vec_i.push_back(5);
	vec_i.push_back(2);
	vec_i.push_back(0); // vec_i = {5, 2, 0}, vec_i.size() == 3

// 访问数据 随机访问
    cout << vec_i[2] << endl;    // 0, []访问，无范围检查
    cout << vec_i.at(1) << endl; // 2, .at()访问，会进行范围检查，访问速度稍慢，但是安全
    
// 通过 索引 遍历元素
    for(int i = 0; i < vec_i.size(); ++i){
    	cout << vec_i[i] << " "; //确定索引不会超范围，就用索引访问  vec_i.at(i)
    }
    cout << endl;
    
// 通过迭代器 遍历元素
    for(vector<int>::iterator iter = vec_i.begin(); iter != vec_i.end(); ++iter){
    	cout << *iter << " "; // 对迭代器进行解引用得到
    }
    cout << endl;
    
//范围for c++ 11 遍历元素
//    for(item:vec_i){
//   	cout << item << " ";
//    }
//    cout << endl;

// 指针访问
	int* p = &vec_i[0];
	for(int i = 0; i < vec_i.size(); ++i){
		cout << p[i]<< " ";
	}
    cout << endl;
    
// 部分容器的成员函数
	if(vec_i.empty()) cout << "vec_i has no num" << endl; // 是否包含元素
	
	cout << vec_i.size() << endl; // 存储的元素数量
	
	vector<int> vec_i2(vec_i); // 拷贝初始化， vec_i2 = {5, 2, 0}
	
	vec_i.clear(); // 清空元素 vec_i.size() == 0
	
	vec_i2.swap(vec_i); // 两向量交换元素，vec_i2.size() == 0, vec_i = {5, 2, 0}
    
    
	return 0;
}

```
### b. deque 双向队列 
	<-----|..................|----->
	     队首               队尾 
```c
/*
1. 在队列 头部和尾部插入元素，特别快 O(1)
2. 在队列中间插入元素，特别慢 O(n)
3. 在队列中查找元素，特别慢 O(n)
<-----|..................|----->
     队首               队尾 
*/

#include<deque>
using namespace std;
deque<int> deq = {5, 2, 0};
deq.push_back(5);  // 队尾  插入 {5, 2, 0, 5}
deq.push_front(0); // 队首  插入 {0, 5, 2, 0, 5}

// 其他接口和 vector 的类似
cout << deq[1] ; // 5
cout << deq.at(2) ;// 2
deque<int>::iterator iter = deq.begin() + 3;
cout << *iter ; // 0
// size() empty()  clear() swap()


```

## 1.2 有序关联容器（红黑二叉树） 
	set, multiset 单值树集合  multi 关键字可以重复出现
	map, multimap 配对双值树集合
	
	#inlcude<set>
	#inlcude<multiset>
	#inlcude<map>
	#inlcude<multimap>
	
## 1.3  无序关联容器（哈希表）
	unordered set/multiset  单值桶集合
	unordered map/multimap  配对双值桶集合 
	
	#inlcude<unordered_set>
	#inlcude<unordered_multiset>
	#inlcude<unordered_map>
	#inlcude<unordered_multimap>
	
	// 和另一个库里的类似
	hash_map
	hash_set
	hash_multimap
	hash_multiset
	


