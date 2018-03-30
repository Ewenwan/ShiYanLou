#include <stdio.h>
#include <stdlib.h>
/*
二分查找是比顺序查找效率高的一种查找算法，
但它只适用于有序的数据集。二分查找也叫折半查找，
它的查找步骤为：
首先，假设表中元素是按升序排列，
将表 ###中间位置##### 记录的关键字与查找关键字比较，
如果两者相等，则查找成功；
否则利用中间位置记录将表分成前、后两个子表，
如果中间位置记录的关键字大于查找关键字，
则进一步查找前一子表，否则进一步查找后一子表。
重复以上过程，直到找到满足条件的记录，使查找成功，
或直到子表不存在为止，此时查找不成功，

第一次数组长度 n
第二次数组长度 n/2
第三次数组长度 n/(2^2)
...
第m次数组长度 n/(2^(m-1)  如果找到  此时 数组长度为1
则 n/(2^(m-1)  = 1
2^(m-1) = n
m-1 = log2(n)
m = log2(n) + 1

则时间复杂度为 O(log2(n))

*/    

// 循环迭代实现
                 //升序数组 查找的元素  范围开始  结束 
int BinarySearch(int *array, int key, int low, int high)
{
    int mid;
    while (low <= high)// 范围正常 
    {
        //mid = (low + high) / 2;//中间元素的 下标 low + high 可能会有整数溢出情况
        mid = low + ((high-low) >> 1);// 避免整数溢出  mid = low + (high-low) / 2;
        if (key == array[mid])//中间元素是否 所查找的元素 
            return mid;//相等 返回元素下标  
      //所查元素比 中间元素小  则在 前区间查找 
        else if (key < array[mid]) high = mid - 1;//将区间 右截至提至 中间元素下标前一个元素 
      //所查元素比 中间元素大 则 在后区间查找 
        else low = mid + 1;//将区间  左截至 提至  中间元素下标后一个元素 
    }
    return -1;
}
// 递归实现 递归思想会被经常用到，更加突出了编程解决问题的高效
int binSearch(int *a, int low, int high, int &key)
{
  int mid = low + ((high - low) >> 1);
  if(low > high) return -1;//查找完毕没有找到答案
  else{
     if(key == a[mid])      return mid;// 返回查找的下标
     else if(key > a[mid])  return binSearch(a, mid+1, high, key);//在后半区间查找
     else                   return binSearch(a, low, mid-1, key);//在前半区间查找
  }
}


int main()
{
    int n, i, key, position;
    int *array;// 升序数组 
    printf("请输入有序数组的大小：");
    scanf("%d", &n);//数组大小 
    array = (int*) malloc(sizeof(int) * n);//开辟内存 
    printf("请按升序输入数据：\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);//数组的每一个元素  升序排列 
    }
    printf("请输入想要查找的数：");
    scanf("%d", &key);//需要查找的数 
    if (position = BinarySearch(array, key, 0, n - 1)) 
    {
        printf("%d的位置为：%d\n", key, position);
    }
    else
    {
        printf("%d不存在\n", key);    
    }
}
