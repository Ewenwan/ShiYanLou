/*
* 两维数组 最大最小值
*/
#include<stdio.h>

int main()
   {
       int i,j,row=0,colum=0,min_row=0,min_colum=0,max,min;
       int a[3][4]={{3,4,16,2},
                    {7,5,1,9},
                    {11,23,3,8}};// init row by row
       max=a[0][0];
       min=a[0][0];
       printf("the array is:\r\n");
       for(i=0;i<3;i++)//peer row 每一行
       {
         for(j=0;j<4;j++)// peer column 每一列
         {
          printf("%d\t",a[i][j]);
          if(a[i][j]>max)//最大值
              {
                 max=a[i][j];//max
                 row=i;
                 colum=j;
              }
          if(a[i][j]<min)//最小值
              {
                 min=a[i][j];//min
                 min_row=i;
                 min_colum=j;
              }
          }
         printf("\r\n");
         }
       printf("max=%d\nrow=%d\ncolum=%d\n",max,row+1,colum+1);
       printf("min=%d\nrow=%d\ncolum=%d\n",min,min_row+1,min_colum+1);
       return 0;
}
