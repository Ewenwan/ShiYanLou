# 并行编程
[参考](https://github.com/thirdwing/ParaBook/blob/master/chapter/Intro.tex)

## 1. pthread 求素数
// PrimesThreads.c
```c
// PrimesThreads.c

// threads-based program to find the number of primes between 2 and n;
// uses the Sieve of Eratosthenes, deleting all multiples of 2, all
// multiples of 3, all multiples of 5, etc.

// for illustration purposes only; NOT claimed to be efficient

// Unix 下编译  gcc -g -o primesthreads PrimesThreads.c -lpthread -lm

// 运行  primesthreads n num_threads

#include <stdio.h>
#include <math.h>
#include <pthread.h>  // 多线程

#define MAX_N 100000000 // 最大数
#define MAX_THREADS 25  // 最多线程数

// 全局 共享变量
int nthreads,  // 线程数
    n,         // 寻找的最大自然数范围
    prime[MAX_N+1],  // 每个数的素数标记，素数标记为1
    nextbase;        // next sieve multiplier to be used
    
//多线程都需要访问的数据
pthread_mutex_t nextbaselock = PTHREAD_MUTEX_INITIALIZER;

// 线程id
pthread_t id[MAX_THREADS];

// k的倍数都是偶数，非素数==
void crossout(int k)
{  
   int i;
   for (i = 3; i*k <= n; i += 2)  {
      prime[i*k] = 0;// 非素数==
   }
}

// 每个线程指向的函数
void *worker(int tn)  // 线程号 tn 
{  int lim,base,
       work = 0;  // 该线程完成的 工作量
       
   // 检测到 sqrt(n) 之内的数
   lim = sqrt(n);
   do  {
      // 对检查对象 上锁
      pthread_mutex_lock(&nextbaselock);
      base = nextbase;// 当前已经检查的数
      nextbase += 2; // 跳过偶数
      // 解锁
      pthread_mutex_unlock(&nextbaselock);
      
      if (base <= lim) 
      {
         // 奇数初始化为1，如果为素数，则设置为0
         if (prime[base])  
         {
            crossout(base);
            work++;  // 该线程 执行的次数
         }
      }
      else return work;
   } while (1);
}

int main(int argc, char **argv)
{  
   int nprimes,  // 找到素数 数量
       i,work;
       
   n = atoi(argv[1]);// 寻找最大范围
   
   nthreads = atoi(argv[2]);// 使用的线程数量
   
   // 初始化检查列表标记
   for (i = 3; i <= n; i++)  
   {
      if (i%2 == 0) prime[i] = 0;// 偶数置零
      else prime[i] = 1; // 奇数置1，带检查是否为素数
   }
   nextbase = 3; // 开始检测的index
   
   // 线程执行开始
   for (i = 0; i < nthreads; i++)  // 启动nthreads 个线程
   { 
      // 线程指针 id[i]  执行 worker 函数
      pthread_create(&id[i],NULL,worker,i);
   }

   // 等待所有线程执行完毕
   for (i = 0; i < nthreads; i++)  
   {
      pthread_join(id[i],&work);// 等待线程结束
      printf("%d values of base done\n",work);// 该线程，执行的次数
   }

   // 打印 找到素数的数量
   nprimes = 1;
   for (i = 3; i <= n; i++)
      if (prime[i])  
      {
         nprimes++;// 数量+1
      }
   printf("the number of primes found was %d\n",nprimes);
  
  return 0;
}

```

## 2.  omp OpenMP 封装了pthread，高层 多线程库 桶排序 Bucket Sort
```c
// 桶排序 Bucket Sort
// OpenMP introductory example:  sampling bucket sort

// 编译 gcc -fopenmp -o bsort bucketsort.c

// set the number of threads via the environment variable
// OMP_NUM_THREADS, e.g. in the C shell

// setenv OMP_NUM_THREADS 8
// 使用 bsort

#include <omp.h>  // required
#include <stdlib.h>

// 比较两元素大小，快排 qsort()调用
int cmpints(int *u, int *v)
{  if (*u < *v) return -1;
   if (*u > *v) return 1;
   return 0;
}

// adds xi to the part array, increments npart, the length of part
void grab(int xi, int *part, int *npart)
{
    part[*npart] = xi; # 添加元素
    *npart += 1;       # 位置+1
}

// 找到 数组y中的最大值和最小值===
void findminmax(int *y, int ny, int *miny, int *maxy)
{  int i,yi;
   *miny = *maxy = y[0];// 最大值最小值初始化
   for (i = 1; i < ny; i++) // 遍历每一个元素
   {
      yi = y[i];// 当前元素   
      if (yi < *miny) *miny = yi; // 最小值
      else if (yi > *maxy) *maxy = yi;// 最大值
   }
}

// 对数组x进行排序
void bsort(int *x, int n)
{  // 函数内变量，多个线程内共享
   float *bdries; int *counts;
   
   #pragma omp parallel  # omp 多线程
   { 
      // 每个线程内部局部变量
      int me = omp_get_thread_num();// 当前线程id
      int nth = omp_get_num_threads();
      int i,xi,minx,maxx,start;
      int *mypart;
      float increm;
      int SAMPLESIZE;
      // now determine the bucket boundaries; nth - 1 of them, by
      // sampling the array to get an idea of its range
      #pragma omp single  // 代码将仅被一个线程执行，具体是哪个线程不确定，
      {
         if (n > 1000) SAMPLESIZE = 1000;
         else SAMPLESIZE = n / 2; // 一半
         findminmax(x,SAMPLESIZE,&minx,&maxx);// 找 最大值和最小值
         
         bdries = malloc((nth-1)*sizeof(float));
         increm = (maxx - minx) / (float) nth;
         for (i = 0; i < nth-1; i++)
            bdries[i] = minx + (i+1) * increm;
         // array to serve as the count of the numbers of elements of x
         // in each bucket
         counts = malloc(nth*sizeof(int));
      }
      // now have this thread grab its portion of the array; thread 0
      // takes everything below bdries[0], thread 1 everything between
      // bdries[0] and bdries[1], etc., with thread nth-1 taking
      // everything over bdries[nth-1]
      mypart = malloc(n*sizeof(int)); int nummypart = 0;
      for (i = 0; i < n; i++) {
         if (me == 0) {
            if (x[i] <= bdries[0]) grab(x[i],mypart,&nummypart);
         }
         else if (me < nth-1) {
            if (x[i] > bdries[me-1] && x[i] <= bdries[me])
               grab(x[i],mypart,&nummypart);
         } else
            if (x[i] > bdries[me-1]) grab(x[i],mypart,&nummypart);
      }
      // now record how many this thread got
      counts[me] = nummypart;
      
      // sort my part
      qsort(mypart,nummypart,sizeof(int),cmpints);
      
      #pragma omp barrier  // other threads need to know all of counts
      // copy sorted chunk back to the original array; first find start point
      start = 0;
      for (i = 0; i < me; i++) start += counts[i];
      for (i = 0; i < nummypart; i++) {
         x[start+i] = mypart[i];
      }
   }
   // implied barrier here; main thread won't resume until all threads
   // are done
}

int main(int argc, char **argv)
{
   // test case
   int n = atoi(argv[1]), *x = malloc(n*sizeof(int));
   int i;
   for (i = 0; i < n; i++) x[i] = rand() % 50;
   if (n < 100)
      for (i = 0; i < n; i++) printf("%d\n",x[i]);
   bsort(x,n);
   if (n <= 100) {
      printf("x after sorting:\n");
      for (i = 0; i < n; i++) printf("%d\n",x[i]);
   }
}

```

## 3. Message Passing Interface, MPI 消息传递的并行编程技术, 找素数

```c
// 与OpenMP并行程序不同，MPI是一种基于消息传递的并行编程技术。
// 消息传递接口是一种编程接口标准，而不是一种具体的编程语言。
// 简而言之，MPI标准定义了一组具有可移植性的编程接口。
// 各个厂商或组织遵循这些标准实现自己的MPI软件包，典型的实现包括开放源代码的MPICH、LAM MPI以及不开放源代码的Intel MPI。
// 由于MPI提供了统一的编程接口，程序员只需要设计好并行算法，使用相应的MPI库就可以实现基于消息传递的并行计算。
// MPI支持多种操作系统，包括大多数的类UNIX和Windows系统。
#include <mpi.h>  // mandatory

#define PIPE_MSG 0  // type of message containing a number to be checked
#define END_MSG 1  // type of message indicating no more data will be coming

int NNodes,  // 节点数量
    N,       // 寻找范围
    Me;      //当前节点id
double T1,T2;  // start and finish times

void Init(int Argc,char **Argv)
{  
   int DebugWait;
   N = atoi(Argv[1]);
   // start debugging section
   DebugWait = atoi(Argv[2]);
   while (DebugWait) ;  // deliberate infinite loop; see below
   /* the above loop is here to synchronize all nodes for debugging;
      if DebugWait is specified as 1 on the mpirun command line, all
      nodes wait here until the debugging programmer starts GDB at
      all nodes (via attaching to OS process number), then sets
      some breakpoints, then GDB sets DebugWait to 0 to proceed; */
   // end debugging section
   MPI_Init(&Argc,&Argv);  // mandatory to begin any MPI program
   // puts the number of nodes in NNodes
   MPI_Comm_size(MPI_COMM_WORLD,&NNodes);
   // puts the node number of this node in Me
   MPI_Comm_rank(MPI_COMM_WORLD,&Me);
   // OK, get started; first record current time in T1
   if (Me == NNodes-1) T1 = MPI_Wtime();
}

void Node0()
{  
   int I,ToCheck,Dummy,Error;
   // 变量一半就可以了
   for (I = 1; I <= N/2; I++)  
   {
      ToCheck = 2 * I + 1;  // 只需要 验证奇数是否为素数
      if (ToCheck > N) break;
      if (ToCheck % 3 > 0)  // not divis by 3, so send it down the pipe
         // send the string at ToCheck, consisting of 1 MPI integer, to
         // node 1 among MPI_COMM_WORLD, with a message type PIPE_MSG
         Error = MPI_Send(&ToCheck,1,MPI_INT,1,PIPE_MSG,MPI_COMM_WORLD);
         // error not checked in this code
   }
   // sentinel
   MPI_Send(&Dummy,1,MPI_INT,1,END_MSG,MPI_COMM_WORLD);
}

// 中间节点=================
void NodeBetween()
{  int ToCheck,Dummy,Divisor;
   MPI_Status Status;
   // first received item gives us our prime divisor
   // receive into Divisor 1 MPI integer from node Me-1, of any message
   // type, and put information about the message in Status
   MPI_Recv(&Divisor,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
   while (1)  {
      MPI_Recv(&ToCheck,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
      // if the message type was END_MSG, end loop
      if (Status.MPI_TAG == END_MSG) break;
      if (ToCheck % Divisor > 0)
         MPI_Send(&ToCheck,1,MPI_INT,Me+1,PIPE_MSG,MPI_COMM_WORLD);
   }
   MPI_Send(&Dummy,1,MPI_INT,Me+1,END_MSG,MPI_COMM_WORLD);
}

// 结尾节点======================
NodeEnd()
{  int ToCheck,PrimeCount,I,IsComposite,StartDivisor;
   MPI_Status Status;
   MPI_Recv(&StartDivisor,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
   PrimeCount = Me + 2;  /* must account for the previous primes, which
                            won't be detected below */
   while (1) 
   {
      MPI_Recv(&ToCheck,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
      if (Status.MPI_TAG == END_MSG) break;
      IsComposite = 0;
      for (I = StartDivisor; I*I <= ToCheck; I += 2)
         if (ToCheck % I == 0) 
         {
            IsComposite = 1;// 非素数=====
            break;
         }
         
      if (!IsComposite) PrimeCount++;
   }
   /* check the time again, and subtract to find run time */
   T2 = MPI_Wtime();
   printf("elapsed time = %f\n",(float)(T2-T1));
   /* print results */
   printf("number of primes = %d\n",PrimeCount);
}

int main(int argc,char **argv)
{  Init(argc,argv);
   // all nodes run this same program, but different nodes take
   // different actions
   if (Me == 0) Node0();// 开始节点
   else if (Me == NNodes-1) NodeEnd();// 结尾节点
   else NodeBetween();// 中间节点
   
   // mandatory for all MPI programs
   MPI_Finalize();
}



```

## 4.  omp OpenMP Dijkstra 最短路径图搜索算法
```c
// Dijkstra.c

// OpenMP example program:  Dijkstra shortest-path finder in a bidirectional graph;
// finds the shortest path from vertex 0 to all others
// 编译 gcc Dijkstra.c -fopenmp -o dijkstra
// 用法  dijkstra nv print

// where nv 图大小, and print is 1 if graph and min
// distances are to be printed out, 0 otherwise

#include <omp.h>

// 全局变量，多个线程内部共享，可以放在 "parallel" pragma in dowork() 之前

int nv,       // 图 顶点数量
    *notdone, // 未检查的节点数量
    nth,      // 线程数量
    chunk,    // 每个线程处理的节点数量
    md,       // 所有线程中，当前最小值
    mv,       // 最小值对应的 顶点
    largeint = -1;  // max possible unsigned int

// 距离表=====1对多====多对多
unsigned *ohd,  // 1-hop distances between vertices; "ohd[i][j]" is
         // ohd[i*nv+j]
         *mind;  // min distances found so far

// 初始化
void init(int ac, char **av)
{  
   int i,j,tmp;
   nv = atoi(av[1]);// 顶点数量
   ohd = malloc(nv*nv*sizeof(int)); // 距离表
   mind = malloc(nv*sizeof(int));   // 初始最短距离
   notdone = malloc(nv*sizeof(int));// 节点 未检查标志
   
   // 生成随机的一个图
   for (i = 0; i < nv; i++)
      // 生成 距离图
      for (j = i; j < nv; j++)  
      {
         if (j == i) ohd[i*nv+i] = 0; // 顶点到自身距离为0
         else 
         {
            ohd[nv*i+j] = rand() % 20;
            ohd[nv*j+i] = ohd[nv*i+j];// 对称======
         }
      }
   for (i = 1; i < nv; i++)  
   {
      notdone[i] = 1;   // 节点 未检查标志
      mind[i] = ohd[i]; // 初始值
   }
}

// 在 s 到 e之间，找未检查节点 中的 最小值 最小的值d，对应节点v
void findmymin(int s, int e, unsigned *d, int *v)
{  int i;
   *d = largeint;// 初始 较大的值
   for (i = s; i <= e; i++)
      if (notdone[i] && mind[i] < *d)  // 未检查节点 中的 最小值 
      {
         *d = mind[i];// 较小的值，距离distance
         *v = i;// 对应节点
      }
}

// 更新 s到e之间的最小值
void updatemind(int s, int e)
{  int i;
   for (i = s; i <= e; i++)
      if (mind[mv] + ohd[mv*nv+i] < mind[i])
         mind[i] = mind[mv] + ohd[mv*nv+i];// 这个更小====
}

void dowork()
{
   #pragma omp parallel
   {  int startv,endv,  // 当前线程的 开始和结束节点
          step,  // 总步骤
          mymv,  // 当前最小值？？
          me = omp_get_thread_num();
          unsigned mymd;  // min value found by this thread
      #pragma omp single// 代码将仅被一个线程执行，具体是哪个线程不确定=====
      {  
         nth = omp_get_num_threads();  // 单个线程内部变量
         if (nv % nth != 0) 
         {
            printf("nv must be divisible by nth\n");
            exit(1);
         }
         chunk = nv/nth;
         printf("there are %d threads\n",nth);
      }
      startv = me * chunk;
      endv = startv + chunk - 1;
      for (step = 0; step < nv; step++) 
      {
         // find closest vertex to 0 among notdone; each thread finds
         // closest in its group, then we find overall closest
         
         #pragma omp single // 代码将仅被一个线程执行，具体是哪个线程不确定，
         {  md = largeint; mv = 0;  }
         findmymin(startv,endv,&mymd,&mymv);
         // update overall min if mine is smaller
         
         #pragma omp critical // 指定一块同一时间只能被一条线程执行的代码区域
         {  if (mymd < md)
               {  md = mymd; mv = mymv;  }
         }
         #pragma omp barrier //  栅障（Barrier）是OpenMP用于线程同步的一种方法。
                             // 线程遇到栅障是必须等待，直到并行区中的所有线程都到达同一点。
         // mark new vertex as done
         #pragma omp single // 代码将仅被一个线程执行，具体是哪个线程不确定，
         {  notdone[mv] = 0;  }
         // now update my section of mind
         updatemind(startv,endv);
         #pragma omp barrier
      }
   }
}

int main(int argc, char **argv)
{  int i,j,print;
   double startime,endtime;
   init(argc,argv);
   startime = omp_get_wtime();
   // parallel
   dowork();
   // back to single thread
   endtime = omp_get_wtime();
   printf("elapsed time:  %f\n",endtime-startime);
   print = atoi(argv[2]);
   if (print)  {
      printf("graph weights:\n");
      for (i = 0; i < nv; i++)  {
         for (j = 0; j < nv; j++)
            printf("%u  ",ohd[nv*i+j]);
         printf("\n");
      }
      printf("minimum distances:\n");
      for (i = 1; i < nv; i++)
         printf("%u\n",mind[i]);
   }
}


```


## 5. omp 矩阵转置
```c
#include <omp.h>

// translate from 2-D to 1-D indices
int onedim(int n,int i,int j) {  return n * i + j;  }

void transp(int *m, int n)
{
   #pragma omp parallel
   {  int i,j,tmp;
      // walk through all the above-diagonal elements, swapping them
      // with their below-diagonal counterparts
      #pragma omp for
      for (i = 0; i < n; i++) {
         for (j = i+1; j < n; j++) {
            tmp = m[onedim(n,i,j)];
            m[onedim(n,i,j)] = m[onedim(n,j,i)];
            m[onedim(n,j,i)] = tmp;
         }
      }
   }
}


```

## 6. omp 快排
```c
// OpenMP example program:  quicksort; not necessarily efficient

#include <omp.h>

void swap(int *yi, int *yj)
{  int tmp = *yi;
   *yi = *yj;
   *yj = tmp;
}

int *separate(int *x, int low, int high)
{  int i,pivot,last;
   pivot = x[low];  // would be better to take, e.g., median of 1st 3 elts
   swap(x+low,x+high);
   last = low;
   for (i = low; i < high; i++) {
      if (x[i] <= pivot) {
         swap(x+last,x+i);
         last += 1;
      }
   }
   swap(x+last,x+high);
   return last;
}

// quicksort of the array z, elements zstart through zend; set the
// latter to 0 and m-1 in first call, where m is the length of z;
// firstcall is 1 or 0, according to whether this is the first of the
// recursive calls
void qs(int *z, int zstart, int zend, int firstcall)
{
   #pragma omp parallel
   {  int part;
      if (firstcall == 1) {
         #pragma omp single nowait
         qs(z,0,zend,0);
      } else {
         if (zstart < zend) {
            part = separate(z,zstart,zend);
            #pragma omp task
            qs(z,zstart,part-1,0);
            #pragma omp task
            qs(z,part+1,zend,0);
         }

      }
   }
}

// test code
main(int argc, char**argv)
{  int i,n,*w;
   n = atoi(argv[1]);
   w = malloc(n*sizeof(int));
   for (i = 0; i < n; i++) w[i] = rand();
   qs(w,0,n-1,1);
   if (n < 25)
      for (i = 0; i < n; i++) printf("%d\n",w[i]);
}


```


