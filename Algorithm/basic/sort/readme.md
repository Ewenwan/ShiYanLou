# 排序算法
## 冒泡排序 BubbleSort
    冒泡排序是一种比较简单的 稳定排序 算法，效率不高，因此实际当中用到的机会并不多。但
    作为快速排序算法的基础，还是有必要了解一下。
    顾名思义，冒泡就是指大的数字（气泡）会优先从底部升到顶端（大的数浮力较大，上升）。
    具体实现的方式就是重复的 【交换排序】，通过两两比较不断地将大的数字右移。
    具体看下面的GIF图就很清楚了。
![](https://github.com/Ewenwan/ShiYanLou/blob/master/Algorithm/img/bublesort.gif)


    冒泡排序在 最坏情况下的时间复杂度是O(n²)，
    最坏的情况比如将一组降序排好的数据按升序排列。
    下面是一个相对效率较高的冒泡排序实现，
    通过设置一个标志位来控制提前跳出(跳过前面已经整体有序的序列)。

```c
// 冒泡排序
void BubbleSort(int data[], int n)  
{  
    bool  swapped;// 有交换的标志 未出现有序的序列
    int temp_data;// 条换
    for(int i=n-1; i>=0; i--)// n行大排序
    {
        swapped = false;
        for(int j=0; j<i; j++)// i次交换排序
        {
            if (data[j+1] < data[j])// 前面的大，需要交换，移到后面
            {
                // 交换数据
                temp_data = data[j+1];
                data[j+1] = data[j];
                data[j] = temp_data;
                swapped = true;// 设置 未出现有序的序列 标志， 还需要继续执行
            }
        }
        if(false == swapped)
            break;
    }
}
```
### 改进2 跳过整体有序 + 跳过部分有序
```c
// 冒泡排序
void BubbleSort(int data[], int n)  
{  
    bool  swapped;// 有交换的标志 未出现有序的序列
    int temp_data;// 条换
    for(int i=n-1; i>=0; i--)// n行大排序
    {
        int last = i;
        for(int j=0; j<i; j++)// i次交换排序
        {
            if (data[j+1] < data[j])// 前面的大，需要交换，移到后面
            {
                // 交换数据
                temp_data = data[j+1];
                data[j+1] = data[j];
                data[j] = temp_data;
                // swapped = true;// 设置 未出现有序的序列 标志， 还需要继续执行
                last = j;
            }
        }
        i = last;// 直接跳过后面已经有序的部分
        //if(false == swapped) break;
    }
}
```
### 改进3 鸡尾酒排序 左到右找最大，再从右到左找最小
待添加
## 简单选择排序
      简单选择排序的平均复杂度为 O(n2),
      但效率通常比相同平均复杂度的直接插入排序还要差。
      但由于选择排序是 内部排序，因此在内存严格受限的情况下还是可以用的。

      选择排序的原理很简单，如下图所示：
        持续从未处理元素中找到最小值并加入到前面已排序列的后面。
        黄色 表示已经排好序的子序列；
        蓝色表示当前处理项；
        红色表示当前最小值
        
![](https://github.com/Ewenwan/ShiYanLou/blob/master/Algorithm/img/select_sort.gif)

```c
#include  <iostream>
using namespace std;

// 数据交换子函数
void swap(int *data1, int *data2)
{
    int temp = *data1;
    *data1 = *data2;
    *data2 = temp;
}

// 选择排序函数，输入数组data和数据个数n
void selectSort(int data[], int n)
{
    int i, j, min_idx;

    // 外层循环，依次处理每一个元素
    for (i = 0; i < n-1; i++)
    {
        // 内层循环，从未处理元素中找到最小值位置
        min_idx = i;//无序数值起点
        for (j = i+1; j < n; j++)
          if (data[j] < data[min_idx])
            min_idx = j;// 记录最小数的下标

        // 交换最小值
        swap(&data[min_idx], &data[i]);
    }
}

// 打印数组
void printarray(int data[], int n)
{
    for (int i=0; i<n; i++)
        cout << data[i] << " ";
}

int main()
{
    int data[] = {12, 34, 54, 2, 3, 12, 27, 1};
    int n = 8;

    cout << "data before shell sorting: \n";
    printarray(data, n);

    selectSort(data, n);

    cout << "\ndata after shell sorting: \n";
    printarray(data, n);

    return 0;
}
```

## 直接插入排序
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


    




## 快速排序 
      快速排序算法一种最常见的排序算法，其核心思想就是 分治 ，具体的：
      1） 选定一个基准数；
      2） 分区，将所有大于基准数的数据分为一区，将所有小于等于基准数的数据分为一区；
      3） 递归，对上述分区重复(1)(2)，直到每个分区只有一个数。 
      
    下面看一个动画来快速理解该算法是怎么工作的：
![](https://github.com/Ewenwan/ShiYanLou/blob/master/Algorithm/img/quick_sort.gif)
      
```c
// 快速排序递归实现
void QuickSort(int *array, int low, int high)
{
    if (low < high)
    {   // 找到新的分区点
        int pivotloc = Partition(array, low, high);
        QuickSort(array, low, pivotloc - 1); // 递归左边的区间 
        QuickSort(array, pivotloc + 1, high);// 递归右边的区间 
    }
}

// 分区
int Partition(int *array, int low, int high)
{
    int pivotkey = array[low];//初始化,中值元素,选择区间的第一个元素 作为比较元素
    array[0] = array[low];//保存中间变量,区间左下标,元素 
    while (low < high)// 从右找小的元素  从左找大的元素 调换 直到 碰头
    {
        // 从右向左寻找 比 比较元素pivotkey小的 数 array[high]
        while (low < high && array[high] >= pivotkey)
        {
            high--;//右索引减小
        }
        
        //把 比 比较元素pivotkey 小的元素 放到 原先比较元素的位置上 
        array[low] = array[high];
        
        // 从左向右 找 比 比较元素 pivotkey 大的数  array[low]
        while (low < high && array[low] <= pivotkey)//找到比  pivotkey= array[low]大的元素 
        {
            low++;//左索引上升
        }
        
        // 大的数放在 原来 从右向左找 比 比较元素小的位置上 
        array[high] = array[low];
    }
    array[low] = array[0];// 这里直接等于 pivotkey 也可以
    return low;// 返回 分割区间
}
```
     
## 堆排
