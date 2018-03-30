 # 向量是最简单的 STL 容器，其数据结构与数组相似，占据着一个连续的内存块。
       由于内存位置是连续的，所以向量中的元素可以随机访问，访问向量中任何一个元素的时间也是固定的。
       存储空间的管理是自动的，当要将一个元素插入到已满的向量中时，会为向量分配一个更大的内存块，将
       向量中的元素复制进新的内存块中，然后释放旧的内存块。所以，向量是一个灵活的数组，是能够
       动态改变自身大小的数组。
      #include <iostream>
      #include <vector>// 为了使用向量类
      #include <algorithm>
      #include <functional> // greater<T>

      using namespace std;

      // 通用打印 向量类vector的 元素 
      template<class T>
      void printVector(char *s, const vector<T>& v) {
        cout << s << " = (";
        if (v.size() == 0) {
          cout << ")\n";//输出 换行 
          return;
          }
          // 类型别名  常量迭代器  不能修改指向的对象 
      typename vector<T>::const_iterator i = v.begin();
        for( ; i != v.end()–1; i++)// for( ; i < v.end()-1; ++i)
          cout << *i << ' ';//打印元素 

        cout << *i << ")\n";//输出最后一个元素 并 换行 
      }

      bool f1(int n) {
        return n < 4;
      }

      int main() {
      int a[] = {1,2,3,4,5};
      vector<int> v1; // 向量v1 为空, size = 0, capacity = 0
      printVector("v1",v1);

      for (int j = 1; j <= 5; j++)
        v1.push_back(j); // v1 = (1 2 3 4 5), 大小size = 5, 当前最大存储能力 capacity = 8
      // 注意，向量没有 push_front()成员函数。


      vector<int> v2(3,7); // v2 = (7 7 7)

      vector<int> ::iterator i1 = v1.begin()+1;

      vector<int> v3(i1,i1+2); // 截取赋值 v3 = (2 3), size = 2, capacity = 2

      vector<int> v4(v1); // 拷贝赋值 v4 = (1 2 3 4 5), size = 5, capacity = 5

      vector<int> v5(5); // v5 = (0 0 0 0 0)

      v5[1] = v5.at(3) = 9; // 指定位置赋值 v5 = (0 9 0 9 0)

      // 向量的容量 可以通过函数 reserve()来改变(尽力改变)。
      v3.reserve(6); // v3 = (2 3), size = 2, capacity = 6

      v4.resize(7); // v4 = (1 2 3 4 5 0 0), size = 7, capacity = 10
      v4.resize(3); // v4 = (1 2 3), size = 3, capacity = 10
      v4.clear(); //清空 数据但是 分配的内存为减少 v4 is empty, size = 0, capacity = 10 ！！！！ 
      // 向量的插入 
      v4.insert(v4.end(),v3[1]); // v4 = (3)
      v4.insert(v4.end(),v3.at(1)); // v4 = (3 3)
      v4.insert(v4.end(),2,4); // v4 = (3 3 4 4)
      v4.insert(v4.end(),v1.begin()+1,v1.end()-1); // v4 = (3 3 4 4 2 3 4)
      // 删除指定元素 
      v4.erase(v4.end()-2); // v4 = (3 3 4 4 2 4)
      v4.erase(v4.begin(), v4.begin()+4); // v4 = (2 4)

      // assign替换 先删除原有元素，在插入元素 
      v4.assign(3,8); // v4 = (8 8 8)
      v4.assign(a,a+3); // v4 = (1 2 3)

      // 逆 迭代器 从尾部向前 迭代 
      vector<int>::reverse_iterator i3 = v4.rbegin();
      for ( ; i3 != v4.rend(); i3++)
      cout << *i3 << ' '; // print: 3 2 1
      cout << endl;

      // 算法 algorithms
      v5[0] = 3; // v5 = (3 9 0 9 0)
      // 条件替换   <4 的都被替换成 7 
      replace_if(v5.begin(),v5.end(),f1,7); // v5 = (7 9 7 9 7)
      //  replace_if(v5.begin(),v5.end(), bind2nd(less<int>(),4) ,7);
      //  bind2nd(op, a)是一个通用函数


      //直接替换 
      v5[0] = 3; v5[2] = v5[4] = 0; // v5 = (3 9 0 9 0)
      // 指定元素替换 0 -> 7 
      replace(v5.begin(),v5.end(),0,7); // v5 = (3 9 7 9 7)
      // sort 排序  快排 堆排 
      sort(v5.begin(),v5.end()); // v5 = (3 7 7 9 9)  升序排序 
      sort(v5.begin(),v5.end(),greater<int> ()); // v5 = (9 9 7 7 3)  降序排序呢
      // 前方插入 
      // 例如对于整型向量，成员函数 front()的原 型为：int& front() ; 可以 放在 左边 也可以放在右边  
      v5.front() = 2; // v5 = (2 9 7 7 3)
      return 0;
      }

