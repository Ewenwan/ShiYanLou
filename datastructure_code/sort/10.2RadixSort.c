#include <stdio.h>
#include <stdlib.h>
// 比较各个数位上的元素
/*
基数排序是跟前面的几种排序算法完全不一样的排序算法，
前面的排序算法主要通过关键字之间的比较和移动来实现，
而基数排序不需要进行关键字之间的比较，它是借助多关键字的思想来实现的。
对于数字，每一位上的数字就是一个关键字，每一位的数字范围就是关键字范围，它
的主要过程为：将所有待比较数值（正整数）统一为同样的数位长度，
数位较短的数前面补零。然后，从最低位开始，依次进行一次排序。
这样从最低位排序一直到最高位排序完成以后,数列就变成一个有序序列，
如下图所示。类似从低位到高位比较，就是从次关键字到主关键字比较，
这种称为最低位优先（LSD），反之称为最高位优先（MSD）。

*/ 

int n;         //元素个数      4  6 100 三个元素 
int bit_num;   //最大数字位数   100最大有三位    4 6 位数不够前面补零 

/*
 * 获取相应位置上的数（从右到左）
 */
int GetNumInPos(int num, int pos)
{
    int i, temp = 1;
    for (i = 0; i < pos - 1; i++)
    {
        temp *= 10;
    }
    return (num / temp) % 10;
}

/*
 * 基数排序（LSD）
 */
void RadixSort(int *array)
{
    int radix = 10;
    int *count, *bucket, i, j, k;
    count = (int*) malloc(sizeof(int) * radix);
    bucket = (int*) malloc(sizeof(int) * n);
    for (k = 1; k <= bit_num; k++)
    {
        for (i = 0; i < radix; i++)
        {
            count[i] = 0;
        }
        //统计各个桶中所盛数据个数
        for (i = 0; i < n; i++)
        {
            count[GetNumInPos(array[i], k)]++;
        }
        //count[i]表示第i个桶的右边界索引
        for (i = 1; i < radix; i++)
        {
            count[i] = count[i] + count[i - 1];
        }
        //分配
        for (i = n - 1; i >= 0; i--)
        {
            j = GetNumInPos(array[i], k);
            bucket[count[j] - 1] = array[i];
            count[j]--;
        }
        //收集
        for (i = 0, j = 0; i < n; i++, j++)
        {
            array[i] = bucket[j];
        }
    }
}

int main()
{
    int i;
    int *array;
    printf("请输入最大数字的位数：");
    scanf("%d", &bit_num);
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * n);
    printf("请输入数据（用空格分隔）：");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);
    }
    RadixSort(array);
    printf("排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
