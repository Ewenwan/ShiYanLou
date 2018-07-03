# 直接插入排序
    其平均复杂度是 O(n2)，因此应用场景较少。
    接插入排序的思路是： 每次处理一个数据，将其插入到一个已经排好序的子序列中，直到数据处理完毕。
    下面给出一个动画示例：
    这里写图片描述：
![](https://github.com/Ewenwan/ShiYanLou/blob/master/Algorithm/img/insert_sort.gif)

  从上面来看我们也可以发现一些优点：

  （1） 实现简单，比其它 O(n^2)排序算法高效；
  （2） 当数据已经基本有序时，效率较高；
  （3） 排序稳定，并且需要的额外存储空间小；
  （4） 可以快速处理新增加的数据；

  参照动画我们可以很容易的理解下面的c++代码：
```c
void insertsort (int data[], int n){
    int j, temp;    
    // 外层循环负责依次处理每一个元素（忽略第一个）
    for (int i = 1; i < n; i++){
        j = i;//无序起点 选择的数据
        // 内层循环负责移动数据直到在前面有序序列中找到合适的插入的位置
        while (j > 0 && data[j] < data[j-1]){// 在前面有序序列中找到一个比data[j]小的数
              temp = data[j];
              data[j] = data[j-1];
              data[j-1] = temp;
              j--;
        }
    }
}

```

      由于直接插入排序对小数据集执行效率较高，
      因此可以利用插入排序来优化快速排序算法。

      具体地，当快速排序划分的子分区数据量较少时(通常为8-20个元素)，
      不再继续分区，而是改为调用插入排序。

      伪代码如下：

      proc quicksort_k(A, lo, hi)
          if hi - lo < K // 子区间过小，可以选用 插入排序 来提高效率
              return
          pivot ← partition(A, lo, hi)
          quicksort_k(A, lo, pivot-1)
          quicksort_k(A, pivot + 1, hi)
      ————————————————————————————————————
      proc sort(A)
          quicksort_k(A, 0, length(A))// 先用快排
          insertionsort(A)// 改用 插入排序


    
