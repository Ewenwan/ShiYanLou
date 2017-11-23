输入一组整数,求出最大子序列的和.
例如:序列： - 2 11 - 4 13 - 5 - 2，则最大子序列和为20。
序列： - 6 2 4 - 7 5 3 2 - 1 6 - 9 10 - 2，则最大子序列和为16

遍历是一种方法：
int Max_Son(int *p, int len)  
{  
    int Max_Sum = 0;  
    int i, j;  
    for (i = 0; i < len; ++i)  
    {  
        int sum = 0;  
        for (j = i; j < len; ++j)  
        {  
            sum = sum + p[j];  
            if (sum > Max_Sum)  
            {  
                Max_Sum = sum;  
            }  
        }  
    }  
    return Max_Sum;  
} 

还有一种思想。联机算法。
重点的一个思想是：如果a[i]是负数那么它不可能代表最大序列的起点，因为任何包含a[i]的作为起点的子序列都可以通过用a[i+1]作为起点来改进。
类似的有，任何的负的子序列不可能是最优子序列的前缀。

int Max_Son(int *arr, int len)  
{  
    int Max_Sum = 0;  
    int sum = 0;  
    int j;  
    assert(arr != NULL);  
    for (j = 0; j < len; ++j)  
    {  
        sum = sum + arr[j];  
        if (sum > Max_Sum)  
            Max_Sum = sum;  
        else if (sum < 0) //前面序列和小于0，可以抛弃
            sum = 0;  
    }  
    return Max_Sum;  
}  
