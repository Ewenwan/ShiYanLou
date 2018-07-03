# 快速排序 
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
      
