#include <stdio.h>
#include <stdlib.h>
// 每次找到剩下序列中最小的元素 放入前面的有序数列中 
int n;
/*
简单排序的思想是通过n-1次数据元素的比较，从n-i+1个记录中选择最小的数据，
并与第i个数据进行交换，它的时间复杂度是O(n^2)。
堆排序就是利用堆的特征来进行排序，它的时间复杂度是O(nlogn)
*/ 
/*
 1】遍历每一个 待排序数组的 元素  i=0 ~ n-2
 2】初始化 无序数组中 最小元素的 下标 为 待排序数组的 首元素下标 min = i
 3】遍历每一个无序数组的 元素  j=i+1 ~ n-1 
 4】找到无序数组 中最小元素的下标   if(a[j] < a[min]) min = j;
 5】最小元素发生了变换 min != i  交换最小元素 和  待排序数组 首元素 swap(a, min, i);
 */
void SelectSort(int *a)
{ 
    // 1】遍历每一个 待排序数组的 元素  i=0 ~ n-2
    for (int i = 0; i < n-1; ++i) 
    {
        // 2】初始化 无序数组中 最小元素的 下标 为 待排序数组的 首元素下标 
        int min = i;
        // 3】遍历每一个无序数组的 元素  i+1 ~ n-1 
        for (j = i + 1; j < n; ++j)
        {
            if (a[j] < a[min])//较小的 
                min = j;// 4】找到无序数组 中最小元素的下标 
        }
        if (min != i)// 5】最小元素发生了变换  交换最小元素 和  待排序数组 首元素
        {
            int temp = a[i];
            a[i] = array[min];
            a[min] = temp;
        }
    }
}

int main()
{
    int i;
    int *array;
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * n);
    printf("请输入数据（用空格分隔）：");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);
    }
    SelectSort(array);
    printf("排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
