/*
* 函数调用 打印1~m中可以被7/11整除的数
*/
#include <stdio.h>

int main(){
  // 函数声明
  int fun(int m);
  // 输入
  int m;
  printf("Please input a int number:\r\n");
  scanf("%d",&m);
  // 函数调用求解
  fun(m);
  
  return 0;
}

int fun(int m)
{
    int numbers[1000],a[1000],i,n=0; //1~m, ...
    // 初始化数组
    for(i=0;i<m;i++) numbers[i]=i+1;
    
    for(i=0;i<m;i++)
    {
        if(numbers[i]%7==0 || numbers[i]%11==0)//被7或者11整除
        {
            a[n]=numbers[i];     //记录
            printf("%d,",a[n]);  //显示
            ++n;  //count
        }
    }
    putchar('\b');    // 删除显示中的最后一个字符 delet the last char ','
    //putchar('\n');  //
    return n;  //return the munber
}
