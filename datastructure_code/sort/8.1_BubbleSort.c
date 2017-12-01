#include <stdio.h>
#include <stdlib.h>

int n;

/*
 * 冒泡排序
  从数组开始依次比较相邻两个元素
  小的往后移      降序排列
  大的往后移      升序排列 
 */
void BubbleSort(int *array)
{
    int i, j, temp;//中间变量 
    for (i = 0; i < n - 1; i++)//n-1次比较 循环 
    {
        for (j = 0; j < n - 1 - i; j++)//每次比较循环 的比较次数 n-1-i 
        {
            if (array[j] > array[j + 1])//前面的元素大 移到 后面  升序排列 
            {
                temp = array[j];//保存大的 
                array[j] = array[j + 1];//小的移到前面 
                array[j + 1] = temp;//大的移到后面 
            }
        }
    }
}
void BubbleSort_digress(int *array)
{
    int i, j, temp;//中间变量 
    for (i = 0; i < n - 1; i++)//n-1次比较 循环 
    {
        for (j = 0; j < n - 1 - i; j++)//每次比较循环 的比较次数 n-1-i 
        {
            if (array[j] < array[j + 1])//前面的元素小 
            {
                temp = array[j+1];//保存大的 
                array[j+1] = array[j];//小的移到前面 
                array[j] = temp;//大的移到后面 
            }
        }
    }
}

int main()
{
    int i;
    int *array,*array_src;
    printf("请输入数组的大小：");
    scanf("%d", &n);
    array = (int*) malloc(sizeof(int) * n);//地址 
    array_src = (int*) malloc(sizeof(int) * n);
    printf("请输入数据（用空格分隔）：");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);
        array_src[i]=array[i]; 
    }
    
    BubbleSort(array);
    printf("升序排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    BubbleSort_digress(array_src);
    printf("降序排序后为：");
    for (i = 0; i < n; i++)
    {
        printf("%d ", array_src[i]);
    }
    printf("\n");
}
