#include <stdio.h>
#include <stdlib.h>

int n;
// 分组排序 再合并排序
/*
归并排序是建立在归并操作上的一种有效的排序算法，
它过程为：比较a[i]和a[j]的大小，若a[i]≤a[j]，
则将第一个有序表中的元素a[i]复制到r[k]中，并令i和k分别加上1；
否则将第二个有序表中的元素a[j]复制到r[k]中，并令j和k分别加上1，
如此循环下去，直到其中一个有序表取完，
然后再将另一个有序表中剩余的元素复制到r中从下标k到下标t的单元，

*/ 
/*
 * 合并
 */
void Merge(int *source, int *target, int i, int m, int n)
{
    int j, k;
    for (j = m + 1, k = i; i <= m && j <= n; k++)
    {
        if (source[i] <= source[j])
        {
            target[k] = source[i++];
        }
        else
        {
            target[k] = source[j++];
        }
    }
    while (i <= m)
    {
        target[k++] = source[i++];
    }
    while (j <= n)
    {
        target[k++] = source[j++];
    }
}

/* 
 * 归并排序
 */
 void MergeSort(int *source, int *target, int s, int t)
 {
     int m, *temp;
     if (s == t)
     {
         target[s] = source[s];
     }
     else
     {
         temp = (int*) malloc(sizeof(int) * (t - s + 1));
         m = (s + t) / 2;
         MergeSort(source, temp, s, m);
         MergeSort(source, temp, m + 1, t);
         Merge(temp, target, s, m, t);
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
    MergeSort(array, array, 0, n - 1);
    printf("排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
 }
