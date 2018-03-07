#include <stdio.h>
#include <stdlib.h>

int n;

/*
 https://www.cnblogs.com/chengxiao/p/6104371.html
 * 希尔排序
   间隔增量 插入排序 
 希尔排序也是插入排序的一种，但它在效率上要比上面的直接插入排序高，
 它是对直接插入排序的改进，它的基本思想是先取一个小于n的整数d1作为第一个增量，
 把文件的全部记录分组。
 所有距离为d1的倍数的记录放在同一个组中。
 先在各组内进行直接插入排序；
 然后，取第二个增量d2< d1重复上述的分组和排序，
 直至所取的增量dt=1(dt<…< d2< d1)，即所有记录放在同一组中进行直接插入排序为止，
 增量序列尤为关键，一般的初次取序列的一半为增量，以后每次减半，直到增量为1


 */
void ShellSort(int *array)
{
    int k = n / 2; //增量序列（仅作举例）  10/2=5
    while (k > 0)
    {
        int i, j;
        for (i = k + 1; i <=n; i++)// 6~10
        {
            if (array[i] < array[i - k])// 原先的直接排序是一次取后面的一个元素 这里是 间隔一个增量 
            {
                array[0] = array[i];// 1   1+k，2 2+k  保存取出来的元素 小 
                for (j = i - k; j > 0 && array[0] < array[j]; j -= k)
                {
                    array[j + k] = array[j];//大的移到后面 
                }
                array[j + k] = array[0];
            }
        }
        k = k / 2;//增量减小 
    }
}

int main()
{
    int i;
    int *array;
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * (n + 1));
    printf("请输入数据（用空格分隔）：");
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &array[i]);
    }
    ShellSort(array);
    printf("排序后为：");
    for (i = 1; i <= n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
