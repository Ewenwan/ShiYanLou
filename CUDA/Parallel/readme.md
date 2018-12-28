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

// needed for call to qsort()
int cmpints(int *u, int *v)
{  if (*u < *v) return -1;
   if (*u > *v) return 1;
   return 0;
}

// adds xi to the part array, increments npart, the length of part
void grab(int xi, int *part, int *npart)
{
    part[*npart] = xi;
    *npart += 1;
}

// finds the min and max in y, length ny,
// placing them in miny and maxy
void findminmax(int *y, int ny, int *miny, int *maxy)
{  int i,yi;
   *miny = *maxy = y[0];
   for (i = 1; i < ny; i++) {
      yi = y[i];
      if (yi < *miny) *miny = yi;
      else if (yi > *maxy) *maxy = yi;
   }
}

// sort the array x of length n
void bsort(int *x, int n)
{  // these are local to this function, but shared among the threads
   float *bdries; int *counts;
   #pragma omp parallel
   // entering this block activates the threads, each executing it
   {  // variables declared below are local to each thread
      int me = omp_get_thread_num();
      // have to do the next call within the block, while the threads
      // are active
      int nth = omp_get_num_threads();
      int i,xi,minx,maxx,start;
      int *mypart;
      float increm;
      int SAMPLESIZE;
      // now determine the bucket boundaries; nth - 1 of them, by
      // sampling the array to get an idea of its range
      #pragma omp single  // only 1 thread does this, implied barrier at end
      {
         if (n > 1000) SAMPLESIZE = 1000;
         else SAMPLESIZE = n / 2;
         findminmax(x,SAMPLESIZE,&minx,&maxx);
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

int NNodes,  // number of nodes in computation
    N,  // find all primes from 2 to N
    Me;  // my node number
double T1,T2;  // start and finish times

void Init(int Argc,char **Argv)
{  int DebugWait;
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
{  int I,ToCheck,Dummy,Error;
   for (I = 1; I <= N/2; I++)  {
      ToCheck = 2 * I + 1;  // latest number to check for div3
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

NodeEnd()
{  int ToCheck,PrimeCount,I,IsComposite,StartDivisor;
   MPI_Status Status;
   MPI_Recv(&StartDivisor,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
   PrimeCount = Me + 2;  /* must account for the previous primes, which
                            won't be detected below */
   while (1)  {
      MPI_Recv(&ToCheck,1,MPI_INT,Me-1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
      if (Status.MPI_TAG == END_MSG) break;
      IsComposite = 0;
      for (I = StartDivisor; I*I <= ToCheck; I += 2)
         if (ToCheck % I == 0)  {
            IsComposite = 1;
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
   if (Me == 0) Node0();
   else if (Me == NNodes-1) NodeEnd();
        else NodeBetween();
   // mandatory for all MPI programs
   MPI_Finalize();
}



```



