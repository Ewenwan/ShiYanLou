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
0】 取区间第一个元素 值作为 比较值 k
1】 从右向左开始检查。如果high位置的值大于k，high--，继续往前检查，直到遇到一个小于k的值。
2】 此时将小于k的这个值放入low的位置。
3】 然后从low位置开始从左向右检查，low++， 直到遇到一个大于k的值。
4】此时将大于k的值也就是此时low位置的值放入high位置，
5】重复第一步，直到low和high重叠 转6。
6】 将k放入此位置 返回位置
 */
int Partition(int *array, int low, int high)
{
    int pivotkey = array[low];//初始化 中值元素   选择区间的第一个元素 作为比较元素
    array[0] = array[low];//保存中间变量  区间左下标  元素 
    while (low < high)// 从右找小的元素  从左找大的元素 调换 直到 碰头
    {
        // 从右向左寻找 比 比较元素pivotkey小的 数 array[high]
        while (low < high && array[high] >= pivotkey)
        {
            high--;//
        }
        
        //把 比 比较元素pivotkey 小的元素 放到 原先比较元素的位置上 
        array[low] = array[high];
        
        // 从左向右 找 比 比较元素 pivotkey 大的数  array[low]
        while (low < high && array[low] <= pivotkey)//找到比  pivotkey= array[low]大的元素 
        {
            low++;//
        }
        
        // 大的数放在 原来 从右向左找 比 比较元素小的位置上 
        array[high] = array[low];
    }
    array[low] = array[0];// 这里直接等于 pivotkey 也可以
    return low;// 返回 分割区间
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
