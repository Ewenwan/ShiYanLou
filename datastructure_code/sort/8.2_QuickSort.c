#include <stdio.h>
#include <stdlib.h>

int n;
/*
快速排序是对冒泡排序的改进，
它的基本思想是通过一趟排序将数据分成两部分，
一部分中的数据都比另一部分中的数据小，
再对这两部分中的数据再排序，直到整个序列有序，
*/ 


/*
 * 分割使枢轴记录的左边元素比右边元素小
 */
int Partition(int *array, int low, int high)
{
    int pivotkey = array[low];//初始化 中值元素   
    array[0] = array[low];//保存中间变量  区间左下标  元素 
    while (low < high)
    {
        while (low < high && array[high] >= pivotkey)//中值元素 找到比  pivotkey= array[low]小的元素 
        {
            high--;//
        }
        array[low] = array[high];//比  pivotkey= array[low]小的元素 
        while (low < high && array[low] <= pivotkey)//找到比  pivotkey= array[low]大的元素 
        {
            low++;//
        }
        array[high] = array[low];//大的放在高出 
    }
    array[low] = array[0];
    return low;
}

/*
 * 快速排序递归实现
 */
void QuickSort(int *array, int low, int high)
{
    if (low < high)
    {
        int pivotloc = Partition(array, low, high);
        QuickSort(array, low, pivotloc - 1);//递归左边的区间 
        QuickSort(array, pivotloc + 1, high);//递归右边的区间 
    }
}

int main()
{
    int i;
    int *array;
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * (n + 1));// 第一个元素 用来保存中间变量 
    printf("请输入数据（用空格分隔）：");
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &array[i]);
    }
    QuickSort(array, 1, n);
    printf("排序后为：");
    for (i = 1; i <= n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
