# 语言学习记录

# 复合类型
## 引用 &   左值引用lvalue reference  别名; int val; &refval = val; 右值引用 rvalue reference 
    int val = 1024;//整形变量
    int &refVal = val;// 整形引用 refVal 指向 val ，是val的另一个名字，引用必须被初始化，引用不是对象，只是为所存在的对象起的一个别名。
    refVal = 2;       // 把2 赋值给refVal 也就是 赋值给了 val
    int i = refVal;   // 相当于 i = val; 
    int &refVal2 = 10;// 错误，非常量引用只能绑定(bind)在对象向,不能与字面值或某个表达式的计算结果绑定在一起, 且引用的类型 必须和 对象的类型一致
    const int &rci = 10;// 常量引用可以绑定到 字面值 、 非常量 一般表达式
    double dval = 3.14;// 浮点数
    int &refVal3 = dval; // 错误，引用的类型 和 对象 必须一致

## 指针 pointer * 指向 point to 另外一个对象（其存储地址）
    int ival = 42;  // 整数
    int *p = &ival; // 指针定义 p存放 ival对象存放的地址，也即是 p 指向 变量 ival的指针   &ival 为取的 ival变量的 存储地址
    double dval = 3.14; // 浮点数
    double *pd = &dval; // double 类型 指针
    double *pd2 = pd;   // pd存放 的是 dval存放的地址

    int *pi = pd;     // 错误， 指针pi 的类型 和 右边指针对象 类型 不匹配
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
        *pi2 = 0;// 指针pi2 没变 , *pi2 被改变 即pi2指向的对象 i 发生了变化
    

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
    

> 指向指针的指针

    int ival = 1024;
    int *pi = &ival; //指向整形变量的指针
    int **ppi = &pi; //指向整形指针的指针  ppi ---> pi ------> ival
    std::cout << ival << "\n"// 直接输出 变量值
              << *pi  << "\n"// 1次解引用 得到变量值
              << **ppi<< "\n";//2次解引用 的到变量值


> 指向指针的 引用 指针对象的别名  引用不是对象 不存在 指向引用的指针

        int i = 42;//整形对象
        int *p;//指向整形类型的指针
        int *&r_p = p;//引用r_p的类型为 指向整形的指针 int * ， 该引用 r_p 是指针p的一个别名 从右向左 离变量最近的符号为 & 即为 引用，* 引用的为 指针类型
        r_p = &i;// 相当于 p = &i p 指向 i
        *r_p = 0;// 相当于 *p = 0 即 i =0 改变了指针指向对象的值  这里 *p *解引用指针的类型其实相当于 原变量的别名 引用


## const 限定符
> 修饰变量后，可以防止变量被修改

        const int bufSize = 512;//初始化常量 一旦创建后就不能被修改
        bufSize = 1024;// 错误，常量不能被赋值
        const int i = get_size();// 一个函数赋值 运行时初始化
        const int j = 42;//编译时初始化
        const int k;//错误，常量定义式必须初始化
        double dvel = 3.14;//  也可以由其他类型变量 强制转换成 常量
        const int ci = dvel;// 由double类型变量 强制转换成 整形常量 3

        extern const int bufSize = 1024;//定义了一个变量，想在其他文件也使用 bufSize 必须在定义之前 加extern
        extern const int bufSize;// 另一个文件中 声明 bufSize 后 就可以使用了
        
> 绑定到常量的引用 const 的引用 即对常量的引用 reference to const 常量 的 别名 不能修改 不存在常量引用 &const 引用不是对象

        const int c_i = 1024;   // 常整数
        const int &r_c_i = c_i; // 常整数 c_i 的 引用（别名）
        r_c_i = 42;// 错误r_c_i 是常量引用 不能被修改
        int &r_c_i2 = c_i;//错误 常整数 不能赋值给 int变量 左右两边类型必须一样

> 允许将一个常量引用绑定到 非常量对象、字面值，甚至是个 一般表达式

        int i = 52;
        const int &r1 = i; // 允许将 常量引用 const int & 绑定到 int变量上
        const int &r2 = 42;// 绑定到 字面值上
        const int &r3 = r1 * 2;// 绑定到一个 表达式上
        int &r4 = r1 * 2;// 错误，r4是非常量 引用，只能绑定到 其对应类型的对象上

        double dval = 3.14;//浮点数
        const int &ri = dval;//常量引用 绑定到 非常量上 
        //相当于 先把 非常量转化成常量 强制类型转换 常量引用实际上绑定了一个临时变量
        const int temp = dval;//

        int i = 42;// int 变量
        int &r1 = i;// 整数变量引用 r1为 i 的别名
        const int &r2 = i;//常量 引用 r2 绑定到 变量 i上
        r1 = 0;// 相当于 i =0
        r2 = 0;// 错误 r2 时常量引用 不允许改变

> 指向常量的指针  const int *   指针指向的值不能变  也就是 *p 不能被赋值 不能改变


            const double pi = 3.14;//双精度 浮点型 常量
            double *ptr_d = &pi;// 错误，浮点型变量指针 不能绑定一个 常量的存储地址
            const double *ptr_d_c = &pi;// 双精度常量 针 ptr_d_c  指向一个 双精度常量
            *ptr_d_c = 42;// 不能给 pi 赋值 指向常量的指针的解引用相当于 绑定的常量 ，因此不能赋值
            doubel dvel = 3.14;//双精度变量
            ptr_d_c = &dvel;//允许 常量指针ptr_d_c 指向一个变量dvel 但是不能通过 常量指针ptr_d_c 修改变量dvel


> 常量指针 *const 指针本身不能改变 也就是指向不能改变  p不能改变 但是其指向的对象 无影响

        int errNumber = 0;//
        int *const conpErr = &errNumber;// * 可变指针  *const 常量指针不可变 指向整形的 常量指针，conpErr 一直指向 errNumber
        *conpErr = 3;//相当于 errNumber = 3
        const double pi = 3.14;//
        const double *const cd_cp = &pi;//指向 常量的常量指针，即 指针本身cd_cp不能变， 其指向的值 *cd_cp也不能变

> 常量表达式 编译时就能确定的量/式子 constexpr int ce = 20;

        const int *p = nullptr;// 指向整形常量 的指针
        constexpr int *p = nullptr;//指向整形变量的 常量指针   constexpr 声明中出现 指针 仅仅说明 指针为常量指针
        int *const p =nullptr;//指向 整形变量的 常量指针
        const int *const p = nullptr;// 指向 整形常量 的常量指针
        constexpr const int *p = nullptr;// 指向 整形常量 的常量指针

        constexpr int ci = 42; // ci 是整形常量
        int j = 0;
        constexpr int *pci = &j; // 指向整形的 常量指针

## 处理类型

### 类型别名  type alias

        typedef double wages;   // wages 是double类型的 同义词
        typedef wages base, *p; // base 也是double类型的 同义词。 p是double * 的同义词
        wages d_money = 1.00;//等有价于 double d_money = 1.00
        p p_dmoney =   &d_money;//等价于 double *p_dmoney =   &d_money;
        cout << p_dmoney << endl; 
        // 别名声明 alias declaration
        using SI = Sales_Item; // SI 是 Sales_Item 的 同义词
        SI item;//等价于 Sales_Item item;
        // 指针、常量const 类别别名
        typedef char *pstring;// pstring等价于 char *  指向char 的指针（是指针）
        const pstring cstr = 0;// cstr是指向 char 的 常量指针 ！！！！不是指向常量字符的 指针 不能直接替换 const修饰的主语是指针
        const pstring *ps;     // ps是一个指针 它指向的对象 是 指向char的常量指针

### auto 类型说明符  让 编译器根据右式 类型 自动推算左式的类型  且用表达式的值 初始化变量

        auto item = val1 +val2;//item 初始化为 val1 和 val2相加的结果 类型 相同
        // 一条语句定义多个变量时，各变量类型必须一致
        auto i=0, *p = &i;//正确 i是整数， p是指向整形的指针
        auto sz = 0, pi = 3.4;// 错误 sz 和 pi 类型不一致

        // 引用 指针 常量 与 auto
        int i = 0, &r = i;//r是i的别名 int类型
        auto a = r;// a 是一个整形数

        // auto 会忽略掉 顶层const
        const int ci = i, &cir = ci;// 常整数
        auto b = ci;  // b是一个整数，ci的顶层 const(最外层修饰 为顶层)特性被忽略
        auto c = cir; // c是一个整数，ci的顶层 const特性被忽略
        auto d = &i;  // d是一个指向整形的指针
        auto e = &ci; // e是一个指向整形常量的指针 const 外又被 取地址符号修饰，所以这里的 const是 底层const 不被忽略

        // 如果希望 auto 推断出的类型是 一个顶层 const，需要在其前面 明确指出
        const auto f = ci; // ci的推演类型是 int ，f是 const int

        // 将引用设置为 auto 还按之前的初始化规则  保留 顶层 const
        auto &g = ci; // g 是一个 整形常量的 引用 （别名）
        auto &h = 42; // 错误，非常量 引用 不能绑定 到 字面值
        const auto &j = 42; // 正确， 常量引用可以绑定到 字面值

        // 将 指针设置为 auto， 也按之前的初始化规则 保留顶层 const
        auto *k = &ci; // k为指向 整形常量的 指针
        auto *l = 0;   // l为空指针
        const auto *m = &ci; // m 为 指向整形常量的 常量指针

        int i = 1024;
        const int c_i = i;
        auto b = c_i; // b是一个 整数 ,c_i的 顶层 const被忽略
        auto e = &c_i;// e 是一个 指向 整形常量的引用  这里的const是底层const 不被忽略
        auto k = c_i, &ll = i;// k 是整数， ll 是一个整数引用
        auto k = c_i, &o = c_i;// 错误，k 是整数变量 o是一个整数常量引用 类型不一致
        auto &m =c_i, *p = &c_i;//正确 m是整数常量引用，p是指向整数常量的指针
        auto n = &c_i, *p = &c_i;// 正确 n是整数常量 引用(底层const，不被忽略) p是指向整数常量的指针
        
  ### decltype 类型指示符  用表达式推断类型 但是不用表达式的值初始化变量   
  
        decltype(f()) sum = x; // sum 的类型为 函数 f() 的返回值类型 初始化为 x 就像 int sum = x; 一样
        const int ci = 0, &cj = ci;// ci 整形常量  cj 整形常量的引用
        decltype(ci) x = 0; // x的类型 同ci 是 整形常量 const int 不忽略 顶层 const
        decltype(cj) y = x; // y的类型 同cj 是 整形常量的引用 const int &  y绑定到x上
        decltype(cj) z;     // 错误 z是一个 引用 ，必须初始化

        // decltype和引用 指针
        int i =42, j=12, *p = &i, &r = i;
        decltype(r) a = j;// a的类型和r的类型一致，为整数的引用 int& a绑定到j上
        decltype(r + 0) b;//正确 加法的结果为 整形 int 因此 b为整形， 这里只定义 没有初始化
        decltype(*p) c; // 错误 其实*p 解引用指针 相当于i的别名 也就是引用 ，所有这里 c的类型为 整数引用 int &，是个引用，必须初始化

        // decltype 的表达式如果 加上括号，得到的结果将是引用
        decltype(i) e; // e 是一个未初始化的 整形变量
        decltype((i)) d = e; // d是一个 整数的引用 绑定到 整形变量 e上  双层引用括号 的结果 永远都是 

        // 赋值表达式 也会产生 引用 引用的类型就是 等式左边变量的类型 如果 i是int i = x的类型是 int&
 
 
