# 语言学习记录

# 复合类型
## 引用 &   左值引用lvalue reference  别名; int val; &refval = val; 右值引用 rvalue reference 
    int val = 1024;//整形变量
    int &refVal = val;// 整形引用 refVal 指向 val ，是val的另一个名字，引用必须被初始化，引用不是对象，只是为所存在的对象起的一个别名。
    refVal = 2;       // 把2 赋值给refVal 也就是 赋值给了 val
    int i = refVal;   // 相当于 i = val; 
    int &refVal2 = 10;// 错误，引用只能绑定(bind)在对象向,不能与字面值或某个表达式的计算结果绑定在一起, 且引用的类型 必须和 对象的类型一致
    double dval = 3.14;// 浮点数
    int &refVal3 = dval; // 错误，引用的类型 和 对象 必须一致

## 指针 pointer * 指向 point to 另外一个对象（其存储地址）
    int ival = 42;  // 整数
    int *p = &ival; // 指针定义 p存放 ival对象存放的地址，也即是 p 指向 变量 ival的指针   &ival 为取的 ival变量的 存储地址
    double dval = 3.14; // 浮点数
    double *pd = &dval; // double 类型 指针
    double *pd2 = pd;   // pd存放 的是 dval存放的地址

    int *pi = pd; // 错误， 指针pi 的类型 和 右边指针对象 类型 不匹配
    pi = &dval;   // 错误， 视图把 double 类型对象的地址 给 int类型的指针 错误  左右两边类型 必须匹配
    
> 利用指针访问对象 取地址内存储的值 解引用符 来访问向

    int ival = 42; //整形 变量
    int *p = &ival;//指针变量定义初始化， p 存放着 变量ival 在内存中的地址
    std::cout << *p;// 表达式里 *p 为 解引用 取的p存放的地址 指向的值
    int &ref_i = ival; // 引用声明定义， ref_i 是一个引用
    int *p; //指针声明定义，p是一个指针
    p = &ival;// 表达式 p为指针（地址），&val 为取的 val 地址
    *p = ival;   //表达式  *p 相当于 ival *是一个解引用
    int &ref_i2 = *p;// &ref_i2 为引用定义 *p 相当于 ival， *是一个解引用
    
    
> 空指针

    int *p1 = nullptr;// 等价于 int *p1 = 0; 空指针  p1 不指向 任何 对象
    int *p2 = 0; // 空指针     等价于 int *p1 = 0;
    int *p3 = NULL;// 空指针   等价于 int *p1 = 0;
    int zero = 0;
    int *p4 = zero;// 错误，不能把 int 变量直接赋值 给指针

    int i = 42;
    int *pi1 = 0; // pi1 被初始化为空指针，但没有指向 任何对象
    int *pi2 = &i;// pi2 被初始化，存有 i 的地址
    int *pi3;// 定义 未初始化
    pi3 = pi2;// pi3 和 pi2指向同一个 对象 i
    pi2 = 0; // 现在 pi2 被赋值为 空指针 不指向任何对象  指针被改变， 指向的对象不变
    pi2 = &i;// 现在 pi2 又指向 i
    *pi2 = 0;// 指针pi2 没变 ，  *pi2 被改变 即 pi2指向的对象 i 发生了变化
    

> 使用指针作为条件 为空指针时，逻辑值为 False，其他指向一个对象的指针(非0指针) 逻辑值 均为 true

> 相等 == 操作符  不等 != 操作符 

> void* 类型的指针  可一指向 任意类型 的 地址 ， 可一存放任意类型的 地址，用作函数参数，函数体内使用时需要，转化成实际传入类型的指针

    double obj = 3.14, *pd = &obj; // double 类型 的变量 double类型的指针
    void *pv = &obj; // obj 可以是任意类型的对象
    pv = pd; // pv 可一存放任意类型的指针（地址）
    double *pd = (double*)(pv);// void* 类型的指针 强制转化成  double* 类型的指针
    

> 定义多个变量

    int i = 1024， *p = &i, &r = i;//i是整形变量 p是一个整形指针 r是一个整形引用
    int* p1, p2;// * 仅仅修饰 p1 ，即p1是 指向整形的指针 p2是整形变量
    int *p1, *p2;// p1 和 p2 都是指向 整形的指针
    

> 指针的指针

    int ival = 1024;
    int *pi = &ival; //指向整形变量的指针
    int **ppi = &pi; //指向整形指针的指针  ppi ---> pi ------> ival
    std::cout << ival << "\n"// 直接输出 变量值
              << *pi  << "\n"// 1次解引用 得到变量值
              << **ppi<< "\n";//2次解引用 的到变量值


> 指向指针的 引用





