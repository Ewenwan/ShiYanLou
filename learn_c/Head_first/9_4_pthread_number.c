#include <stdio.h>
#include <stdlib.h>
#include <string.h>// strerror 
#include <unistd.h>// 进程 
#include <errno.h>//errno 
#include <pthread.h>//线程 

 void* do_stuff(void* param)//进程 一个 void* 类型参数 
 {
   long thread_no = (long)param;//转化成long 
   printf("Thread number %ld\n", thread_no);//打印结果 
   return (void*)(thread_no + 1);//返回 参数+1 void* 类型 
 }
  
 int main()
 {
   pthread_t threads[20];
   long t; 
   for (t = 0; t < 3; t++) {
     pthread_create(&threads[t], NULL, do_stuff, (void*)t);//将long 型变量t 转化成 void*指针类型 
   }
   void* result;
   for (t = 0; t < 3; t++) {
     pthread_join(threads[t], &result);
     printf("Thread %ld returned %ld\n", t, (long)result);//转成long型  返回 进程结果 
   }
   return 0;
 }
