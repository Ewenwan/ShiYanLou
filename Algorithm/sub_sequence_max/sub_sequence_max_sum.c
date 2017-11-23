#include <stdio.h>  
  
int Max_Son_n2(int *p, int len)  
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

//线性复杂度
int Max_Son_n(int *arr, int len)  
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
        else if (sum < 0)  
            sum = 0;  
    }  
    return Max_Sum;  
}  
 
int main()  
{  
    int arr[] = { -2, 11, -4, 13, -5, -2 };  
    int array[] = { -6, 2, 4, -7, 5, 3, 2, -1, 6, -9, 10, -2 };  
    int len = sizeof(arr) / sizeof(arr[0]);  
    int lena = sizeof(array) / sizeof(array[0]);  
    printf("%d\n", Max_Son_n2(arr, len));//20  
    printf("%d\n", Max_Son_n2(array, lena));//16 
    printf("%d\n", Max_Son_n(arr, len));//20  
    printf("%d\n", Max_Son_n(array, lena));//16 	
    return 0;  
}  