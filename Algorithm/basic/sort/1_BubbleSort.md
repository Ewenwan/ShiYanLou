# 冒泡排序 BubbleSort
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
## 改进2 跳过整体有序 + 跳过部分有序


## 改进3 鸡尾酒排序 左到右找最大，再从右到左找最小

