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
 * 选择排序
 */
void SelectSort(int *array)
{
    int i, j, k, temp;
    for (i = 0; i < n; i++)//从第一个原素开始 
    {
        k = i;//当前 元素 下标 
        for (j = i + 1; j < n; j++)// 一次 选取下一个元素进行比较 
        {
            if (array[j] < array[k])//较小的 
            {
                k = j;//记录较小的 元素 位置 
            }
        }
        if (k != i)//依次把 小的元素放入前面的有序数列中  较小的元素小标发生了变化 
        {//
            temp = array[i];
            array[i] = array[k];
            array[k] = temp;
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
