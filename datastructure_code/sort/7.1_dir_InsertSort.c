#include <stdio.h>
#include <stdlib.h>

int n;
/*
 * 直接插入排序
 直接插入排序，它的做法是：每次从无序表中取出第一个元素，
 把它插入到有序表的合适位置，使有序表仍然有序。
 第一趟比较前两个数，然后把第二个数按大小插入到有序表中；
  第二趟把第三个数据与前两个数从前向后扫描，
  把第三个数按大小插入到有序表中；
  依次进行下去，进行了(n-1)趟扫描以后就完成了整个排序过程

 */
void InsertSort(int *array)//原数组有效数据从 1位置开始 
{
    int i, j;
    for (i = 2; i <= n; i++)//从第二个元素开始 array[1] 拿出来放入前面的有序数列 
    {
        if (array[i] < array[i - 1])//后面的元素小，应该要放入前面的有序数列中（一次比较） 
        {
            array[0] = array[i];//保存当前位置的元素  array[0]用于保存中间变量 
            array[i] = array[i - 1];//把大的  array[i - 1] 放在 array[i] 位置 
            for (j = i - 2; array[0] < array[j]; j--)//将 array[0] = array[i] 元素依次和前面元素比较, 大的元素 依次后移 
            {
                array[j + 1] = array[j];//大的元素依次后移 
            }
            array[j + 1] = array[0];//将元素放入合适的位置 
        }
    }
}
/*
 1】 遍历 无序表 的每一个  元素 无序表范围 i ~ size-1
 2】 取出无序表的第一个元素 取名为temp
 3】 从后向前遍历 有序表的 每一个元素
 4】 将有序表中比temp大的元素 依次后移
 5】 在有序表中找到比temp 小的元素 a[j]
 6】 将temp放在有序表中 a[j]后面 a[j+1]的位置上
*/
void InsertSort2(int *a, int size)//原数组有效数据从 0位置开始 
{
       // 1】 遍历 无序表 的每一个  元素 无序表范围 i ~ size-1
    for (int i = 1; i < size; ++i) 
    {
       // 2】 取出无序表的第一个元素 取名为temp
      int temp = a[i];
      int j;
       // 3】 从后向前遍历 有序表的 每一个元素
      for (j = i-1; j >= 0; --j)
      {
       // 4】 将有序表中比temp大的元素 依次后移
       if(a[j] > temp) a[j + 1] = a[j];
       else break; // 5】 在有序表中找到比temp 小的元素 a[j]
      }
     a[j+1] = temp;// 6】 将temp放在有序表中 a[j]后面 a[j+1]的位置上
    }
}


int main()
{
    int i;
    int *array;//数组指针 
    printf("请输入数组的大小：");
    scanf("%d", &n);//数组大小 
    array = (int*) malloc(sizeof(int) * (n + 1));//分配内存空间  多一个空间 用于保存中间变量 0的位置 
    printf("请输入数据（用空格分隔）：");
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &array[i]);//输入无序 数列  从1开始 
    }
    InsertSort(array);//直接插入排序 
    printf("排序后为：");
    for (i = 1; i <= n; i++)
    {
        printf("%d ", array[i]);//输出排序后的 数组元素 
    }
    printf("\n");
}
