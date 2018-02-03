#include<stdlib.h>
#include<stdio.h>

int main(int argc,char *argv[]){

   if(argc<=1){
   printf("not operater!");
   return 1;
  }
  int sum=0;
  for(int i=1; i<=argc-1; i++){
    sum += atoi(argv[i]);//转成整形 求和
  }
  printf("%d\r\n",sum);//打印和
  return 0;
}
