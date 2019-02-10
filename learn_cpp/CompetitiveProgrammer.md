# cpp手册
## 一、基础知识
### 1. 简介
    竞争性编程Competitive programming 结合了两个主题：
    1）算法设计 the design of algorithms 和
    2）算法的实现 the implementation of algorithms。
    
    1. 算法的设计
      包括问题解决problem solving 和数学思维mathematical thinking。
      需要有  analyzing problems分析问题 和 创造性地解决问题的技巧。
      用于解决问题的算法必须是正确且有效的，并且问题的核心通常是发明有效的算法。
      算法的理论知识Theoretical knowledge 对竞争程序员很重要。
      通常，问题的解决方案是 众所周知的技术 和 新见解new insights  的组合。
      竞争性编程中出现的技术也构成了算法科学研究的基础。
      
    2. 算法的实现
      需要良好的 编程技能programming skills 。
      在竞争性编程中，通过测试已实施的方案对解决方案进行评级算法使用一组测试用例。
      因此，算法的思想是正确的是不够的，但实现也必须是正确的。
      比赛中良好的编码风格简洁明了。应该快速编写程序，因为没有太多时间可用。
      不同于传统的软件工程，程序很短（通常最多几百行代码），并且在比赛后不需要维护maintained。  

c++ python  java

> 输入输出
```c
int a, b;
string x;
cin >> a >> b >> x; // 输入 c function  scanf("",&)  ; scanf("%d %d", &a, &b);
>>> 123 456 monkey

int a = 123, b = 456;
string x = "monkey";
cout << a << " " << b << " " << x << "\n";// 输出 c function  printf("",) ; printf("%d %d\n", a, b);

string s;
getline(cin, s);  // 读取一整行

// 输入数据量未知====
while (cin >> x) {
    // code
}

// 文件 
freopen("input.txt", "r", stdin);
freopen("output.txt", "w", stdout);


```

> 数字
```c
// integers 整数=======================
long long x = 123456789123456789LL; // 后缀suffix 
int a = 123456789;
long long b = a*a; // 错误，溢出
cout << b << "\n"; // -1757895751
// 正确
long long b = (long long)a*a; // 强制转换,static_cast<long long>(a*a) 


//=========================
// 取余技术
(a + b) mod m = (a mod m + b mod m) mod m
(a − b) mod m = (a mod m − b mod m) mod m
(a * b) mod m = (a mod m * b mod m) mod m // 可以直接展开

// 计算n的阶乘 取余 m
long long x = 1;
for (int i = 2; i <= n; i++) 
{
  x = (x*i)%m;
}
cout << x%m << "\n";// 最后再取余m


// 浮点数 Floating point numbers==========
printf("%.9f\n", x);
double x = 0.3*3+0.1;
printf("%.20f\n", x); // 0.99999999999999988898

// 浮点数判等
if (abs(a-b) < 1e-9) 
{
     // a 和 b 相等
}

```

> 缩短代码技术 Shortening code 
```c
// typedef 起外号=======类别别名==========
typedef long long ll; // 起小名，外号，好记，好写

//long long a = 123456789;
//long long b = 987654321;
ll a = 123456789;
ll b = 987654321;

cout << a*b << "\n";

// 复杂类型 重命名  后面为简写 =======================
typedef vector<int> vi;   // 整形 向量   后面为简写
typedef pair<int,int> pi; // 整形-整形 数据对


// #define 宏macros定义  前面为简写==================
#define F first   // 前面为简写
#define S second
#define PB push_back
#define MP make_pair

v.push_back(make_pair(y1,x1));
v.push_back(make_pair(y2,x2));
int d = v[i].first+v[i].second;

// ====>
v.PB(MP(y1,x1));
v.PB(MP(y2,x2));
int d = v[i].F+v[i].S;

// 接收参数的宏
#define REP(i,a,b) for (int i = a; i <= b; i++) // 简化 for循环 repare

for (int i = 1; i <= n; i++) 
{
  search(i);
}
// ====>
REP(i,1,n) 
{
  search(i);
}

// 一些宏导致的错误，较难发现
#define SQ(a) a*a
cout << SQ(3+3) << "\n";
cout << 3+3*3+3 << "\n"; // 15 
// 正确方式
#define SQ(a) (a)*(a)
cout << SQ(3+3) << "\n";
cout << (3+3)*(3+3) << "\n"; // 36
```

> 数学
```c
1 + 2 +,...,+ n = n(n+1)/2
1^2 + 2^2 +,...,+ n^2 = n(n+1)(2n+1)/6
1^3 + 2^3 +,...,+ n^3 = n^2(n+1)^2/4

S = a + ak + ak^2 +,···,+ b.
kS = ak + ak^2 + ak^3 +··· + bk,
kS − S = bk − a
S = (bk − a)/(k-1)
```

### 2. 复杂度
> loops循环
```c
for (int i = 1; i <= n; i++)  // for (int i = 1; i <= 3*n; i++) // i <= n+5;  i += 2;
{
    // code  一层循环 O(n)
}

for (int i = 1; i <= n; i++) 
{
    for (int j = 1; j <= n; j++) 
    {
        // code 二层循环 O(n^2)
    }
}
// n层循环 O(n^k)====

for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
        // code   两层变量不一致  O(n*m)
    }
}
```

> 递归 Recursion
```c


```



### 3. 排序

### 4. 数据结构

### 5. 穷举搜索 Complete search

### 6. 贪婪算法 Greedy algorithms

### 7. 动态规划 Dynamic programming

### 8. 平摊分析 Amortized Analysis

### 9. 范围查询 Range queries

### 10. 位运算Bit manipulation

## 二、图算法  Graph algorithms
### 1. 图结束 Basics of graphs 
### 2. 图遍历 Graph traversal
### 3. 最短路 Shortest paths
### 4. 树算法 Tree algorithms
### 5. 生成树 Spanning trees
### 6. 有向图 Directed graphs
### 7. 强连通 Strong connectivity
### 8. 树查询 Tree queries
### 9. 路径 path
### 10. 流 和 割 Flows and cuts

## 三、高级话题 Advanced topics
### 1. 数论 Number theory 
### 2. 组合数学 Combinatorics 
### 3. 矩阵 Matrices
### 4. 概率 Probability
### 5. 博弈论 对策论 Game theory
### 6. 字符串算法 String algorithms
### 7. 平方根算法 Square root algorithms 
### 8. 分割树 遍历  Segment trees revisited
### 9. 几何学 Geometry
### 10. 扫描线算法 Line Sweep Algorithm 

## 
### 1. 
### 2. 
### 3.
### 4. 
### 5. 
### 6.
### 7. 
### 8. 
### 9.
### 10. 
### 11. 
### 12.
