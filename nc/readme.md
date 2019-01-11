# Numerical Calculation 数值计算
[数值计算第一次实验(C语言版)](https://blog.csdn.net/xr469786706/article/details/71435592)

## C语言编程常用数值计算的高性能实现
```c
/* 高位全0，低N位全1 */
#define Low_N_Bits_One(N)     ((1<<N) -1)

/* 高位全1，低N位全0 */
#define Low_N_Bits_Zero(N)    (~((1<<N) -1))

/* 低位第N位置1 */
#define Set_Bit_N(NUM, N)     (NUM | (1 << (N - 1)))

/* 低位第N位置0 */
#define Clear_Bit_N(NUM, N)   (NUM & (~(1 << (N - 1))))

/* 低位第N位反转 */
#define Reverse_Bit_N(NUM, N) ((NUM) ^ (1 << (N - 1)))

/* 上取整 */
#define UpRoun8(Num)  (((Num) + 0x7)  & (~0x7))
#define UpRoun16(Num) (((Num) + 0xf)  & (~0xf))
#define UpRoun32(Num) (((Num) + 0x1f) & (~0x1f))

/* 下取整 */
#define LowRoun8(Num)  ((Num) & (~0x7))
#define LowRoun16(Num) ((Num) & (~0xf))
#define LowRoun32(Num) ((Num) & (~0x1f))

/* 求商 */
#define Div8(Num)         ((Num)>>3)
#define Div16(Num)        ((Num)>>4)
#define Div32(Num)        ((Num)>>5)

/* 余数进位求商 */
#define UpDiv8(Num)        (((Num)>>3) + !!((Num) & 0x7))
#define UpDiv16(Num)        (((Num)>>4) + !!((Num) & 0xf))
#define UpDiv32(Num)        (((Num)>>5) + !!((Num) & 0x1f))

/* 求余 */
#define M8(Num)        ((Num) & 0x7)
#define M16(Num)       ((Num) & 0xf)
#define M32(Num)       ((Num) & 0x1f)

/* 求反余
即求最小的x，满足(Num + x) % 模数 == 0
例如 RM16(Num) 等价于 (16 - Num%16) % 16 */
#define RM8(Num)        ((~(Num) + 1) & 0x7)
#define RM16(Num)        ((~(Num) + 1) & 0xf)
#define RM32(Num)        ((~(Num) + 1) & 0x1f)
```

## 线性方程组求解
    1. 二分法方程求根 
    2. 顺序消元法解线性方程组 
    3. 列选主元消去法解线性方程组
    4. 全选主元消去法解线性方程组
    5. Doolittle(杜立特尔)分解     
    6. Crout(克洛特)分解解线性方程组 
    7. 平方根法法解线性方程组       
    8. 拉格朗日插值法                 
    9.  牛顿插值法       
    10. Hermite插值法                
    11. 最小二乘法(线性拟合)     
    12. 变步长梯形求积法计算定积分  
    13.  Romberg算法    
    14.  改进欧拉法  
    
> main.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "head.h"

// 这些函数 的实现在 head.h头文件中
double f(double);   //方 程f(x)=0的函数
double f2(double);  //变步长积分函数
void Dodichotomy();   //二分法
void Doleastsquares();    //最小二乘法
void Doshunxu();    //顺序消元法
void Doliezhu();    //列主消元法
void Doquanxuan();  //全选主元消去
void Dodoolittle(); //杜利特尔分解法
void Docrout(); //Crout(克洛特)分解解线性方程组
void Dopingfang();  //平方根法解线性方程组
void Dolagrange();  //拉格朗日插值法
void Donewton();    //牛顿插值法
void Dohermite();   //Hermite插值法
void Doleastsquares();  //最小二乘法
void Dochangestep();    //变步长梯形求积法计算定积分
void Doromberg();   //龙贝格算法
void Doeuler(); //改进欧拉算法

int main()
{
    char ch;
    int n;
    for(;;)
    {
        n = Menu();
        switch(n)
        {
            case 1:
                Dodichotomy();
                break;
            case 2:
                Doshunxu();
                break;
            case 3:
                Doliezhu();
                break;
            case 4:
                Doquanxuan();
                break;
            case 5:
                Dodoolittle();
                break;
            case 6:
                Docrout();
                break;
            case 7:
                Dopingfang();
                break;
            case 8:
                Dolagrange();
                break;
            case 9:
                Donewton();
                break;
            case 10:
                Dohermite();
                break;
            case 11:
                Doleastsquares();
                break;
            case 12:
                Dochangestep();
                break;
            case 13:
                Doromberg();
                break;
            case 14:
                Doeuler();
                break;
            case 15:
                return 0;
            default:
                printf("输入错误,请重新输入\n");
                break;
        }
    }
}

void Dodichotomy()
{
    printf("%lf\n",Dichotomy(f,-4,4,1e-5));
}

void Doleastsquares()
{
    double x[5] = {1,2,3,4,5};
    double y[5] = {0,2,2,5,4};
    double a,b;
    LeastSquares(x,y,5,&a,&b);
    printf("a = %lf;b = %lf\n",a,b);
}

double f(double x)
{
    double y;
    y = x*(x*(2.0*x-4.0)+3.0)-6.0;
    return y;
}

void Doshunxu()
{
    double a[3][3]={
                   2.5,2.3,-5.1,
                   5.3,9.6,1.5,
                   8.1,1.7,-4.3
                   };
    double b[3] = {3.7,3.8,5.5};

    Shunxu(a,3,3,b);
}

void Doliezhu()
{
    double a[3][3]={
                   2.5,2.3,-5.1,
                   5.3,9.6,1.5,
                   8.1,1.7,-4.3
                   };
    double b[3] = {3.7,3.8,5.5};

    Liezhu(a,3,3,b);
}

void Doquanxuan()
{
    double a[3][3]={
                   2.5,2.3,-5.1,
                   5.3,9.6,1.5,
                   8.1,1.7,-4.3
                   };
    double b[3] = {3.7,3.8,5.5};

    Quanxuan(a,3,3,b);
}

void Dodoolittle()
{
    float a[3][3] = {
                    1,-1,3,
                    2,-4,6,
                    4,-9,2
    };
    float b[4] = {1,4,1};
    Doolittle(a,3,3,b);
}

void Docrout()
{
    float a[3][3] = {
                    1,-1,3,
                    2,-4,6,
                    4,-9,2
    };
    float b[4] = {1,4,1};
    Crout(a,3,3,b);
}

void Dopingfang()
{
    int i;
    double a[4][4] = {
                     {1,2,1,-3},
                     {2,5,0,-5},
                     {1,0,14,1},
                     {-3,-5,1,15}
        };
    double b[4] = {1,2,16,8};
    double d[4] = {0};
    double x[4] = {0};
    double y[4] = {0};
    double L[4][4] = {0};
    Cholesky(a,b,L,d,x,y,4);
    printf("方程的解为：\n");
    for(i=1;i<=4;i++)
        printf("x(%d)=%f\n",i,x[i-1]);
}

void Dolagrange()
{
    Point points[20] = {{0.56160,0.82741},{0.56280,0.82659},
                        {0.56401,0.82577},{0.56521,0.82495}
    };
    Lagrange(points,4,0.5635);
}

void Donewton()
{
    double b[6][7] = {
        {0.4,0.41075},
        {0.55,0.57815},
        {0.65,0.69675},
        {0.80,0.88811},
        {0.90,1.02652},
        {1.05,1.25382}
    };
    Newton(b,6,0.596);
}

void Dohermite()
{
    static double xi[3] = {0.1,0.3,0.5};
    static double yi[3] = {0.099833,0.295520,0.479426};
    static double dyi[3] = {0.995004,0.955336,0.877583};
    Hermite(0.4,xi,yi,dyi,3,1.0e-12);
}

double f2(double x)
{
    double sum;
    if(x = 0) return 1;
    sum = sin(x)/x;
    return sum;
}

void Dochangestep()
{
    Changestep(f2,1,2,0.0000001);
}

double f3(double x,double y)
{
    return -y + x + 1;
}

void Doromberg()
{
    Romberg(f2,0,1,0.0000001);
}

void Doeuler()
{
    Euler(f3,0,0.5,1,5);
}
```

> head.h
```c
/*
* 输入点结构
*/
typedef struct stPoint
{
    double x;
    double y;
}Point;

/*
* 开始菜单
*/
int Menu()
{
    int n;
    printf("----------------数值实验------------------\n");
    printf("|                                        |\n");
    printf("|     [1]:二分法方程求根                 |\n");
    printf("|     [2]:顺序消元法解线性方程组         |\n");
    printf("|     [3]:列选主元消去法解线性方程组     |\n");
    printf("|     [4]:全选主元消去法解线性方程组     |\n");
    printf("|     [5]:Doolittle(杜立特尔)分解        |\n");
    printf("|     [6]:Crout(克洛特)分解解线性方程组  |\n");
    printf("|     [7]:平方根法法解线性方程组         |\n");
    printf("|     [8]:拉格朗日插值法                 |\n");
    printf("|     [9]:牛顿插值法                     |\n");
    printf("|     [10]:Hermite插值法                 |\n");
    printf("|     [11]:最小二乘法(线性拟合)          |\n");
    printf("|     [12]:变步长梯形求积法计算定积分    |\n");
    printf("|     [13]:Romberg算法                   |\n");
    printf("|     [14]:改进欧拉法                    |\n");
    printf("|     [15]:退出                          |\n");
    printf("|                                        |\n");
    printf("------------------------------------------\n");
    printf("请输入你要选择的操作:");
    scanf("%d",&n);
    return n;
}

/*
* 二分法方程求根
* fun: 求方程fun(x) = 0的函数fun(x)
* left: 根区间左边点
* right: 根区间右边点
* eps:  二分法的精度
*/
double Dichotomy(double (*fun)(double),double left,double right,double eps)
{
    double yleft,yright,ym;
    double middle;
    do
    {
        yleft = fun(left);
        yright = fun(right);
        middle = (left + right)/2;
        ym = fun(middle);
        if(ym*yleft < 0)
        {
            right = middle;
        }
        else
        {
            left = middle;
        }
    }while(fabs(ym) >= eps);
    return left;
}

/*
* 高斯顺序消元法
* a[][]:系数矩阵
* n,m:系数矩阵的维数
* b[]:常数矩阵
*/
void Shunxu(double (*a)[3],int n,int m,double b[])
{
    int i,j,t,tmp;
    double x[20];
    //下面高斯顺序消元
    for(i = 0 ; i < n ; i++)//将矩阵化成上三角矩阵
        for(j = i+1 ; j < m ; j++)
        {
            tmp = a[j][i]/a[i][i];
            for(t = i+1 ; t < m; t++)
                a[j][t] = a[j][t] - tmp*a[i][t];
            b[j] = b[j] - tmp*b[i];
            a[j][i] = 0;
        }
    //回代
    x[n-1] = b[n-1]/a[n-1][n-1];
    for(i = n-2 ; i >= 0 ; i--)
    {
        x[i] = b[i];
        for(j = i+1 ; j < m ; j++)
            x[i] -= a[i][j]*x[j];
            x[i]/=a[i][i];
    }

    //输出结果
    printf("结果为:\n");
    for(i = 0 ; i < n ; i++)
        printf("x[%d] = %lf\n",i+1,x[i]);
}

/*
* 获取主元所在的行
* a[][]: 系数矩阵
* n: 系数矩阵的维数
* k: 所在的列
*/
int getMaxzhu(double (*a)[3],int n,int k)
{
    int i;
    int laber = k;
    double maxlaber = 0;

    for(i = k ; i < n ; i++)
    if(maxlaber < fabs(a[i][k]))
    {
        maxlaber = fabs(a[i][k]);
        laber = i;
    }
    return laber;
}

/*
* 高斯列主消元法
* a[][]:系数矩阵
* n,m:系数矩阵的维数
* b[]:常数矩阵
*/
void Liezhu(double (*a)[3],int n,int m,double b[])
{
    int i,j,t,tmp,laber;
    double x[20];
    //高斯消元法
    for(i = 0 ; i < n ; i++)
    {
        laber = getMaxzhu(a,3,i);
        if(laber != i)
        {
            for(j = 0 ; j < m ; j++)
            {
                tmp = a[i][j];
                a[i][j] = a[laber][j];
                a[laber][j] = tmp;
            }
            tmp = b[laber];
            b[laber] = b[i];
            b[i] = tmp;
        }
        //消元
       for(j = i+1 ; j < m ; j++)
       {
            tmp = a[j][i]/a[i][i];
            for(t = i+1 ; t < m; t++)
                a[j][t] = a[j][t] - tmp*a[i][t];
            b[j] = b[j] - tmp*b[i];
            a[j][i] = 0;
       }
    }
    //回代
    x[n-1] = b[n-1]/a[n-1][n-1];
    for(i = n-2 ; i >= 0 ; i--)
    {
        x[i] = b[i];
        for(j = i+1 ; j < m ; j++)
            x[i] -= a[i][j]*x[j];
            x[i]/=a[i][i];
    }

    //输出结果
    printf("结果为:\n");
    for(i = 0 ; i < n ; i++)
        printf("x[%d] = %lf\n",i+1,x[i]);
}

/*
* 根据L和U的矩阵求解方程组的解
* l[][20]: 矩阵L u[][20]: 矩阵U
* b[]: 常数矩阵
* x[]: 解的矩阵
* n: 维数
*/
void solve(float l[][20],float u[][20],float b[],float x[],int n)
{
    int i,j;
    float t,s1,s2;
    float y[20];

    //第一次回带过程
    for(i = 0 ; i < n ; i++)
    {
        s1 = 0 ; 
        for(j = 0 ; j < i-1 ; j++)
        {
            t = -l[i][j];
            s1 = s1 + t*y[j];
        }
        y[i] = (b[i] + s1)/l[i][i];
    }

    for(i = n-1 ; i >= 0 ; i--)
    {
        s2 = 0;
        for(j = n-1 ; j > i-1 ; j--)
        {
            t = -u[i][j];
            s2 = s2 + t*x[j];
        }
        x[i] = (y[i] + s2)/u[i][i];
    }
}

/*
* 高斯全选主元消元法
* a[][]:系数矩阵
* n,m:系数矩阵的维数
* b[]:常数矩阵
*/
void Quanxuan(double (*a)[3],int n,int m,double b[])
{
    int i,j,t,p,tmp,max,w = 0,y = 0;
    double x[20];
    for(i = 0 ; i < n ; i++)
    {
        for(p = i ; p < m ; p++)
        {
            max = getMaxzhu(a,3,p);
            if(a[w][y] < a[max][p])
            {
                w = max;
                y = p;
            }
        }
        for(j = 0 ; j < m ; j++)
        {
            tmp = a[i][j];
            a[i][j] = a[w][j];
            a[w][j] = tmp;
        }
        for(j = i ; j < n;  j++)
        {
            tmp = a[j][i];
            a[j][i] = a[i][y];
            a[i][y] = tmp;
        }
        //消元
       for(j = i+1 ; j < m ; j++)
       {
            tmp = a[j][i]/a[i][i];
            for(t = i+1 ; t < m; t++)
                a[j][t] = a[j][t] - tmp*a[i][t];
            b[j] = b[j] - tmp*b[i];
            a[j][i] = 0;
       }
    }
    //回代
    x[n-1] = b[n-1]/a[n-1][n-1];
    for(i = n-2 ; i >= 0 ; i--)
    {
        x[i] = b[i];
        for(j = i+1 ; j < m ; j++)
            x[i] -= a[i][j]*x[j];
            x[i]/=a[i][i];
    }

    //输出结果
    printf("结果为:\n");
    for(i = 0 ; i < n ; i++)
        printf("x[%d] = %lf\n",i+1,x[i]);
}

/*
* Doolittle(杜利特尔分解法)
* (*a)[4]: 系数矩阵a
* int m,n: 矩阵a的维数
* b[]: 矩阵b
*/
void Doolittle(float (*a)[3],int n,int m,float b[])
{
    float l[20][20],u[20][20],x[20];
    int i,j,k;
    float s1,s2;
    // 将所有的数组置零,同时将L矩阵的对角值设为1 
    for(i = 0 ; i < 20 ; i++)
        for(j = 0 ; j < 20 ; j++ )
        {
            l[i][j] = 0;
            u[i][j] = 0;
            if(j == i) l[i][j] = 1;
        }

    // 下面求解矩阵L和U
    for(i = 0 ; i < n ; i++)
    {
        for(j = i ; j <= n ; j++)
        {
            s1 = 0;
            for(k = 0 ; k < i-1 ; k++)
                s1 = s1 + l[i][k]*u[k][j];
            u[i][j] = a[i][j] - s1;
        }
        for(j = i+1; j < n ;j++)
        {
            s2 = 0; 
            for(k = 0 ; k < i-1 ; k++)
                s2 = s2 + l[j][k]*u[k][i];
            l[j][i] = (a[j][i] - s2)/u[i][i];
        }
    }
    printf("array L:\n");/*输出矩阵L*/
    for(i = 0;i < n;i++)
    {
        for(j = 0;j < n;j++)
            printf("%7.3f ",l[i][j]);
        printf("\n");
    }
    printf("array U:\n");/*输出矩阵U*/
    for(i = 0;i < n;i++)
    {
        for(j = 0;j < n;j++)
            printf("%7.3f ",u[i][j]); printf("\n");
    }
    solve(l,u,b,x,n);
    printf("解为:\n");
    for(i = 0 ; i < n ; i++)
    {
        printf("x%d = %f\n",i,x[i]);
    }
}

/*
* (*a)[3]: 系数矩阵a
* int n,m: 矩阵a的维数
* b[]: 常数矩阵
*/
void Crout(float (*a)[3],int n,int m,float b[])
{
    int i,j,k;
    float l[20][20],u[20][20];
    float x[20],y[20];
    // 将所有的数组置零,同时将U矩阵的对角值设为1 
    for(i = 0 ; i < 20 ; i++)
        for(j = 0 ; j < 20 ; j++ )
        {
            l[i][j] = 0;
            u[i][j] = 0;
            if(j == i) u[i][j] = 1;
        }

    l[0][0]=a[0][0];
    for(i = 0 ; i < n ; i++)
    {
        l[i][0]=a[i][0];//计算第一行的l[][]
        u[0][i]=a[0][i]/l[0][0];//计算第一列的u[][]
    }
    for(i = 0 ; i < n - 1 ; i++)
    {
        for(j = 0 ; j <= i ; j++)//计算第2行到第size-1行的l[][]
        {
            l[i][j] = a[i][j];
            for(k = 0 ; k < j ; k++)
            {
                l[i][j] = l[i][j] - l[i][k]*u[k][j];
            }
        }
        printf("\n");
        for(j = i ; j < n ; j++)//计算第2行到第size行的u[][]
        {
            u[i][j] = a[i][j];
            for(k = 0 ; k <= i-1 ; k++)
            {
                u[i][j] = u[i][j] - l[i][k]*u[k][j];
            }
            u[i][j] = u[i][j]/l[i][i];
        }
        printf("\n");
    }
    for(j = 0;j < n;j++)//计算第n行的l[][]
    {
        l[n-1][j]=a[n-1][j];
        for(k=0;k<=j-1;k++)
        {
            l[n-1][j]=l[n-1][j]-l[n-1][k]*u[k][j];
        }
    }

    printf("\n");
    printf("array L:\n");/*输出矩阵L*/
    for(i = 0;i < n;i++)
    {
        for(j = 0;j < n;j++)
            printf("%7.3f ",l[i][j]);
        printf("\n");
    }
    printf("array U:\n");/*输出矩阵U*/
    for(i = 0;i < n;i++)
    {
        for(j = 0;j < n;j++)
            printf("%7.3f ",u[i][j]); printf("\n");
    }
    solve(l,u,b,x,n);
    printf("解为:\n");
    for(i = 0 ; i < n ; i++)
    {
        printf("x%d = %f\n",i,x[i]);
    }
}

/*
* 平方根法法解线性方程组
* a[][],b[]: 系数矩阵
* n: 矩阵的维数
* Cholesky():算法函数
*/
void Cholesky(double a[][4],double b[],double L[][4],double d[],double x[],double y[],int n)
{
    int i,j,k;
    double s;
    for(i=0;i<n;i++)
    L[i][i]=1;
    d[0]=a[0][0];
    for(i=1;i<n;i++)
    {
        for(j=0;j<i;j++)
        {
            s=0;
            for(k=0;k<j;k++)
                s=s+L[i][k]*d[k]*L[j][k];
            L[i][j]=(a[i][j]-s)/d[j];
        }
        s=0;
        for(k=0;k<i;k++)
            s=s+L[i][k]*L[i][k]*d[k];
        d[i]=a[i][i]-s;
    }
    y[0]=b[0];
    for(i=1;i<n;i++)
    {
        s=0;
        for(k=0;k<i;k++)
            s=s+L[i][k]*y[k];
        y[i]=b[i]-s;
    }
    x[n-1]=y[n-1]/d[n-1];
    for(i=n-2;i>=0;i--)
    {
        s=0;
        for(k=i+1;k<n;k++)
        s=s+L[k][i]*x[k];
        x[i]=y[i]/d[i]-s;
    }
    for(i=1;i<=n;i++)
        printf("d(%d)=%f\n",i,d[i-1]);
    printf("矩阵L为：\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<=i;j++)
            printf("%f    ",L[i][j]);
        printf("\n");
    }
}

/*
* Point *point: 插值节点结构体指针
* int n: 插值点的个数
* double s: 待求插值点
*/
void Lagrange(Point *points,int n,double x)
{
    int i,j;
    double tmp,lagrange = 0;
    /* tmp:拉格朗日基函数,lagrange:根据拉格朗日函数得出的f(x) */
    //利用拉格朗日插值公式计算函数x的值
    for(i = 0 ; i <= n ; i++)
    {
        for(j = 0 , tmp = 1 ; j <= n ; j++)
        {
            if(j == i)  // 去掉xi与xj相等的情况
                continue;   //范德蒙行列式下标就是j!=k,相等分母就没有意义了
            tmp = tmp*(x - points[j].x)/(points[i].x-points[j].x); //拉格朗日基函数
            lagrange=lagrange+tmp*points[i].y; //最后计算基函数*y,全部加起来，就是该x项的拉格朗日函数了
        }
    }
    //拉格朗日函数计算完毕，代入所求函数x的值，求解就ok了
    printf("\n拉格朗日函数f(%lf)=%lf\n",x,lagrange);
}

/*
* 最小二乘法
* x[],y[]: 观测点数据
* n: 观测点数量
* a,b: y = ax + b,线性拟合的两个系数
*/
void LeastSquares(double x[],double y[],int n,double *a,double *b)
{
    int i;
    double sumx = 0,sumy = 0,sumxx = 0,sumxy = 0;
    for(i = 0 ; i < n ; i++)
    {
        sumx += x[i];
        sumy += y[i];
        sumxx += x[i]*x[i];
        sumxy += x[i]*y[i];
    }

    // n*a + sumx*b = sumy;
    // sumx*a + sumxx*b = sumxy;

    *b = (sumy*sumx - sumxy*n)/(sumx*sumx - sumxx*n);
    *a = (sumy - sumx*(*b))/n;
}

/*
* 牛顿插值法
* b[][]: 均差表
* n: 插值点的个数
* x: 待求插值点
*/
void Newton(double (*b)[7],int n,double x)
{
    double sum = 0,w = 1,cc[2][4];
    // sum:保存牛顿插值多项式
    int i,j,k,m,z = 0;
    // 求均差表
    for(i = 2 , k = 1; i < 7;i++,k++)
    {
        for(j = i - 1 ; j < n ; j++)
        {
            b[j][i] = (b[j-1][j-1] - b[j][i - 1])/(b[j - k][0] - b[j][0]);
        }
    }
    for(m = 0 ; m < 4 ; m++,z = 0)
    {
        cc[0][m] = b[m+1][m+2];
        do{
            w *= (x - b[z][0]);
        }while(z++ != m);
        cc[1][m] = w;
    }
    sum = b[0][1];
    for(m = 0 ; m < 4 ; m++)
        sum += (cc[0][m]*cc[1][m]);
    printf("\n插值y为:%lf\n",sum);
}

void Hermite(double x,double xi[],double yi[],double dyi[],int N,double EPS)
{
    int i,j;
    double li,sum,y,gix[N],hix[N];
    for(i = 0 ; i < N ; i++)
    {
        li = 1.0; sum = 0.0;
        for(j = 0 ; j < N ; j++)
        {
            if(j != i)
            {
                li *= (x - xi[j])/(xi[i] - xi[j]);
                sum += 1.0/(xi[i] - xi[j]);
            }
            li = li*li;
            gix[i] = (1.0 - 2.0*(x - xi[i])*sum)*li;
            hix[i] = (x - xi[i])*li;
        }
        y = 0.0;
        for(i = 0; i < N; i++)
            y += yi[i] * gix[i] + dyi[i] * hix[i];

        printf("x = %15.6e    p(x) = %15.8e\n",x,y);
    }
}

/*
* 变步长梯形求积法计算定积分
* 原理: 利用若干个小梯形面积代替原方程的积分
* fun(): 积分函数
* a,b: 积分上下限
* eps: 误差
*/
void Changestep(double (*fun)(double),double a,double b,double eps)
{
    int i,n; // i为分段变步长的数量
    double fa,fb,h,t1,p,s,x,t;
    // fa,fb: 代表边界值带入函数后的值
    // h: 分段后每个子区间的长度
    // x: 分段后的分段点
    // p: 为当前误差
    fa = fun(a);
    fb = fun(b);
    n = 1;
    h = b - a;
    t1 = h*(fa + fb)/2;
    p = eps + 1;
    while(p >= eps)
    {
        s = 0;
        for(i = 0 ; i <= n-1 ; i++)
        {
            x = a + (i + 0.5)*h;
            s += fun(x);
        }
        t = t1/2 + h*s/2; //根据公式计算
        p = fabs(t1 - t);
        printf("步长为%d时的Tn = %lf\tT2n = %lf\t,误差变化为:%lf\n",n,t1,t,p);
        t1 = t;
        n = n*2;
        h = h/2;
    }
    printf("经过变长梯形求积法得到的方程的结果为:%lf\n",t);
}

/*
* Romberg算法
* fun(): 积分函数
* a,b: 积分上下限
* eps: 误差限
*/
void Romberg(double (*f)(double),double a,double b,double eps)
{
    double h,T1,T2,S,S1,S2,C1,C2,R1,R2,z[100][4];
    //h:分段后每个子区间的长度
    int i,j,k = 1;
    h = b-a;
    T1 = (f(a) + f(b))*h/2;
    z[0][0] = T1;
    while(1)
    {
        S = 0;
        double x;
        for(x = a + h/2; x < b ; x = x+h)
            S += f(x);
        T2 = (T1 + h*S)/2;
        z[k][0] = T2;

        S2 = T2 + (T2 - T1)/3; // 利用T1,T2计算S2
        z[k][1] = S2;

        if(k == 1);
        else{
            C2 = S2 + (S2 - S1)/15; // 利用S1,S2计算C2
            z[k][2] = C2;

            if(k == 2);
            else{
                R2 = C2 + (C2 - C1)/63; //利用C1,C2计算R2
                z[k][3] = R2;

                if(k == 3);
                else{
                    if(fabs(R2 - R1) < eps);
                        break;
                }
                R1 = R2;
            }
            C1 = C2;
        }
        k++;
        h /= 2;
        T1 = T2;
        S1 = S2;
    }
    printf("%3c%6c%12c%12c%12c\n",'k','T','S','C','R');
    for(i = 0; i < k;i++)
    {
        printf("%3d",i);
        if(i < 3)
        {
            for(j = 0 ; j <= i ; j++)
            {
                printf("%12.7lf",z[i][j]);
            }
            printf("\n");
        }
        else
        {
            for(j = 0;j < 4 ; j++)
            {
                printf("%12.7lf",z[i][j]);
            }
            printf("\n");
        }
    }
}

/*
* 改进的欧拉算法
* f(): 积分函数
* a,b: 积分上下限
* y0: 初值
* n: 将区间等分的数目
*/
void Euler(double (*f)(double, double),double a,double b,double y0,int n)
{
    double h;
    double x,y,xn,yn,yp;
    int i;
    h = (b - a)/n;
    x = a;
    y = y0;
    for(i = 1 ; i <= n ; i++)
    {
        xn = x + h;
        yn = y + h*f(x,y);
        yp = yn + h/2*(f(x,y) + f(xn,yn));
        printf("x%d = %lf,y%d = %lf\n",i,xn,i,yp);
        x = xn;
        y = yp;
    }

}
```

> 编译
gcc.sh

    #!/bin/bash

    # File Name : gccfile.sh
    # Author : xiaorui
    # Created Time :2017年05月01日 星期一 16时47分31秒
    # Program Funtion :gcc File

    gcc main.c -lm -o main
    # -lm: 链接相应的数学库函数


