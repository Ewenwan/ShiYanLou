# 语言学习记录

# 复合类型
## 引用 &   左值引用lvalue reference  别名; int val; &refval = val; 右值引用 rvalue reference 

	int val = 1024;//整形变量
	int &refVal = val;// 整形引用 refVal 指向 val ，是val的另一个名字，引用必须被初始化，引用不是对象，只是为所存在的对象起的一个别名。
	refVal = 2;       // 把2 赋值给refVal 也就是 赋值给了 val
	int i = refVal;   // 相当于 i = val; 
	int &refVal2 = 10;// 错误，非常量引用只能绑定(bind)在对象向,不能与字面值或某个表达式的计算结果绑定在一起, 且引用的类型 必须和 对象的类型一致
	const int &rci = 10;// 常量引用可以绑定到 字面值 非常量 一般表达式
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

> void* 类型的指针  可一指向 任意类型 的 地址 ， 可一存放任意类型的 地址，

> 用作函数参数，函数体内使用时需要，转化成实际传入类型的指针

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
	int *&r_p = p;//引用r_p的类型为 整形的指针引用int*&，r_p 是指针p的一个别名 
	 //从右向左 离变量最近的符号为 & 即为 引用，* 引用的为 指针类型
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
 
 # 标准库 std
 ## 字符串类 string 类  #include <string> 存储一个可变长度的 字符串          使用内置数组类型实现
 
 ### 范围for  访问所有元素
 
	string str("some string");
	for (auto c : str)// auto自动推断类型
	cout << c << endl; 
	// 范围for 引用 改变 内容
	string s("Hello World!!!");
	for (auto &c : s)// c是 字符串中每个元素的一个别名
	c = toupper(c);//变成大写
	cout << s << endl;
     
 ### 下标运算符(索引)[] 和 迭代器 访问单个元素
 > string的下标 类型为 string::size_type 无符号数 可用 decltype(s.size())获取 s[0] 是第一个字符 s[s.size()-1]是最后一个字符
 
		// 第一个字符改为大写
		string s("some string");
		if(!s.empty())
			s[0] = toupper(s[0]);//第一个字符改为大写  在 cctype头文件中
		// 第一个单词改为大写
		for(decltype(s.size()) index = 0;
		    index != s.size() && !isspace(s[index]; ++index))
			s[index] = toupper(s[index]);
	 
## 向量 模板  容器container  vector 模板  #include <vector> 存储一个可变长度的 对象 集合 使用内置数组类型实现
> 因为 vector 可以存放任意类型 所以事先需要知道 存放的对象是什么类型  vector<int> ivec; vector<string>; vector<vector<string> >;
    
	// 初始化方式
	vector<int> ivec(10,-1);// 直接初始化 10个元素 全为 -1
	vector<int> ivec2 = ivec;//拷贝初始化
	vector<int> ivec3{10};//一个元素 10 
	vector<int> ivec3{10，1};//两个元素 10  和 1
	vector<string> svec{"a","an","the"};//列表初始化 直接方式
	vector<string> svec2 = {"a","an","the"};//列表初始化 拷贝方式
	
	// 默认初始化 
	vector<int> ivec(10);    // 10个元素，每个值都是0 
	vector<string> svec(10); // 10个元素，每个值都是空 string 对象
	vector<string> svec2{10};// 10个元素，每个值都是空 string 对象
	vector<string> svec3{10, "hi"};// 10个 "hi"元素
	vector<string> svec3(10, "hi");// 10个 "hi"元素
	
	// 使用 .push_back() 添加元素
	vector<int> ivec2; //空vec对象
	for(int i = 0; i != 100; ++i)
	ivec2.push_back(i);// 一次把整数值 放到 ivec2尾部 结束后 ivec2有100个元素 0~99
	
	// 实时添加 string 元素
	string word;
	vector<string> text;//空对象
	while (cin >> word) 
	text.push_back(word);// 把word添加到 text 后面
	
	//使用范围for  + 引用 访问 并改变vector元素
	vector<int> iv{1,2,3,4,5,6,7,8,9};// 列表直接初始化
	for (auto &i : v)// 对于v中每个 元素的 引用 需要改变其值
	i *= i;      // 变成原来值 的 平方
	for (auto i : v) // 仅读取其中的变量
	cout << i << " ";
	cout << endl;
	
	// vector 对象大小 类型为size_type
	vector<int>::size_type se = iv.size();
	
	// 使用索引[] 访问 计算vector对象元素索引   统计各个分数段上 出现的 成绩个数
	// 索引不能添加元素
	vector<unsigned> scores(11,0);//11个分数段， 0~9，10~19，...,90~99，100 计数值全部初始化为0
	unsigned grade;
	while (cin >> grade){
	if(grade <= 100) ++scores[grade/10];
	}    
	
	// cin 读入一组词 改成大写 存入 vector中  #include <cctype> 使用toupper()
	vector<string> sv;
	string word1 = "qwe";
	cout << word1 << endl; 
	while(cin >> word1){
	        for (auto &c : word1) c = toupper(c);
		sv.push_back(word1); 	
		cout << word1 << endl; vector  
	}

## 迭代器 访问容器中的 元素 auto b = v.begin(), e = v.end(); b表示v的第一个元素 e表示v尾元素 的下一个位置  类似 指针

	// 修改 字符串 第一个元素为大小字符
	string s("some string");
	if (s.begin() != s.end()){//确保 s非空
	  auto it = s.begin();// it 指向 s的第一个字符 类似指针 的作用
	  *it = toupper(*it);// 将当前字符改写成大写形式  *it 解引用迭代器 得到其所指向的 对象  是其指向对象的别名 引用
	}

	// 字符串的第一个单词 改写成大写
	for (auto it = s.begin(); it != s.end() && !isspace(*it); ++it)
		*it = toupper(*it);

	// 迭代器类型  iterator (具有读写功能)  const_iterator 具有 读功能  不具有写功能
	// 对象为常量 只具有常量类型的迭代器 const_iterator  对象为变量具有 iterator 和 const_iterator
	vector<int>::iterator it;// 迭代器 it 可以读写 vector<int> 类型容器 的元素
	 string::iterator it2;   // it2 可以读写  string对象 中的字符
	 vector<int>::const_iterator it3;//it3只能读元素，不能写元素
	 string::const_iterator it4;     //it4只能读字符，不能写字符

	 // cbegin()   cend() 返回 常量 迭代器 仅能读取 容器元素 不能修改
	 vector<int> iv;        // 变量
	 const vector<int> civ; // 常量
	 auto it1 = iv.begin(); //  it1的类型为 vector<int>::iterator  能读写iv的元素
	 auto it2 = iv.cbegin(); // it2的类型为 vector<int>::const_iterator  能读iv的元素 不能修改 iv的元素
	 auto it3 = civ.begin(); // it3的类型为 vector<int>::const_iterator  能读civ的元素 不能修改 civ的元素

	  // 访问 容器元素对象的 成员函数   (*it).empty  等同于 it->empty()  解引用 和成员访问
	  vector<string> text;
	  for (auto it = text.cbegin(); it !=text.cend() && !it->empty(); ++it)
		cout << *it << endl;

	  // 迭代器 运算
	  auto mid = iv.begin() + iv.size()/2; //指向容器的中间位置
	  if (it < mid) // 处理前半部分元素
	  
	// 两个迭代器相减 得到的类型为 带符号整数  difference_type

	// 常规二分查找算法
	// 升序数组 查找的元素  范围开始  结束 
	int BinarySearch(int *array, int key, int low, int high)
	{
	    int mid;
	    while (low <= high)//  缩小范围 
	    {
		mid = (low + high) / 2;//更新中间元素的 下标 
		if (key == array[mid])//中间元素是否 等于所查找的元素 
		    return mid+1;//相等 返回元素下标  
		else if (key < array[mid])//所查元素 比 中间元素小  则在 前区间查找 
		    high = mid - 1;//将区间 右侧 退后 到 中间元素下标前一个元素  搜索 范围为  low，mid-1
		else//所查元素 比 中间元素大 则 在后区间查找 
		    low = mid + 1;//将区间  左测 提至  中间元素下标后一个元素    搜索 范围 mid+1，high
	    }
	    return 0;
	}
	
	// 使用迭代器完成二分查找
	 vector<int> text// 升序容器
	 auto b = text.begin(), e = text.end();//起始 结束位置
	 auto mid = b + (e - b)/2;//中间位置
	 while(low <= end && *mid != key){
	 	if(key < *mid) e = mid - 1;//所查元素 比 中间元素小  则在 前区间查找
		else b = mid + 1;// 所查元素 比 中间元素大 则 在后区间查找
		mid =  b + (e - b)/2;//更新 中间位置
	 }
	
	// 使用索引[] 访问 计算vector对象元素索引   统计各个分数段上 出现的 成绩个数
	// 索引不能添加元素
	vector<unsigned> scores(11,0);//11个分数段， 0~9，10~19，...,90~99，100 计数值全部初始化为0
	unsigned grade;
	while (cin >> grade){
	if(grade <= 100) ++scores[grade/10];
	} 
	// 使用迭代器 访问 计算vector对象元素索引   统计各个分数段上 出现的 成绩个数
	vector<unsigned> scores(11,0);//11个分数段， 0~9，10~19，...,90~99，100 计数值全部初始化为0
	unsigned grade;
	auto it0 = scores.begin();
	while (cin >> grade){
		auto it = it0 +  grade/10;
		if(grade <= 100) ++*it;
	} 
	
	
	
# 数组 
>  数组与标准库 类型 vector 类似，都存放类型相同对象的容器。
>  需要通过其所在的位置访问对象。
>  与vector不同的是，数组大小确定不变，不能随意向数组中增加元素, 数组不允许拷贝，vector允许拷贝。
>  注意数组名 相当于数组首元素的地址   ia[10]  ia === &ia[0]

## 【1】定义

	constexpr unsigned sz = 42;//constexpr修饰，常量表达式
	int arr[10];  //字面值常量初始化  含有10个整数的数组
	int arr2[sz]; //常量表达式初始化 含有42个个整数的数组
	int *parr[sz];//常量表达式初始化 含有42个指向整数的指针的数组

	定义时必须指定数组类型，不能由auto来推断
	不存在引用的数组，引用不是对象！！！
	string nums[] = {"one", "two", "three"};// 数组元素是string对象
	string *sp = &nums[0];// p指向nums的第一个元素
	string *sp2 = nums;   // 等价于 string *sp2 = &nums[0]

	auto sp3(nums); //sp3 是一个string指针，指向nums的第一个元素
	// 而decltype关键字 声明的 不发生上述转换
	 decltype(nums) sa = {"two", "three", "four"};//sa 是一个 含有3个string对象的 数组


##【2】显式初始化数组元素

	const unsigned sz = 3;//
	int ia1[sz] = {0, 1, 2};//列表初始化 含有3个元素
	int ia2[] = {0, 1, 2};//维度为3
	int ia3[5] = {0, 1, 2};//等价于 {0, 1, 2, 0, 0}

	// 字符数组
	char ca1[] = {'C', 'P', 'P'};//列表初始化
	char ca2[] = {'C', 'P', 'P', '\0'};//含有显式的 空字符
	char ca3[] = "CPP";//字符串字面值初始化 自动添加表示字符串结束的空字符

	// string 对象初始化 字符数组
	string s("Hello World");
	const char *str = s.c_str();// 用string对象初始化 字符数组 需要使用 c_str() 方法 最好再重新拷贝一份

	// 数组  初始化 vector 对象
	int i_arr[] = {1, 2, 3, 4, 5, 6};
	vector<int> ivec(begin(i_arr), end(i_arr));//全部副本
	vector<int> sub_ivec(i_arr + 1, i_arr + 4);// 包含 {2, 3, 4, 5}四个元素



	// 不允许拷贝和赋值
	char ca4 = ca3;// 错误

	// 复杂的数组声明定义
	int *parr[10];// 是数组，包含10个整形指针的数组
	int &rarr[10]=?;//错误，不存在 引用数组，引用不是对象
	int (*Parray)[10] = &arr;//是指针，指向一个含有10个整数的数组
	int (&Rarray)[10] = arr;//是引用，引用一个含有10个整数的数组	
	
## 【3】访问数组元素
### 与标准库类型vector 和 string 一样，数组元素也可以使用 范围for语句
### 或下标运算符 访问，元素下标从0开始，下标通常定义为 size_t类型,unsigned类型。
### 标准库类型vector 和 string 下标运算符索引必须为正值 unsigned类型，数组下标运算符索引 为signed类型，内置类型，可以为负值

	//下标访问修改元素
	unsigned score[11];//11个分数段
	unsigned grade;
	while(cin >> grade){
	    if(grade <= 100) ++score[grade/10];//对应段 计数+1
	}
	// 范围for 访问修改所有元素
	for( auto i:score)//可以设置为 引用就可以修改元素了
	    cout << i << " ";
	cout << endl;

	// 指针访问数组
	int iarr[] = {0,1,2,3,4};//含有5个元素
	int *pi = iarr;//指向第一个元素的指针 iarr[0]
	int *pi2 = iarr + 2;//指向第三个元素的指针 iarr[2]
	auto num = end(iarr) - begin(iarr); // num的值是5 就是iarr包含元素的数量 ptrdiff_t 类型 是signde类型 结果可能为负
	++pi;//指向第二个元素 iarr[1]
	j = pi[1];   // 等价于 *(p+1),就是 iarr[2], 就是 2
	k = pi[-1];  // 等价于 *(p-1),就是 iarr[0], 就是 0
	pi + 3;//指向最后一个元素
	*pi;//第二个元素 4
	int last =  *(iarr + 4);// 等价于 last = iarr[4];
	int *end = &iarr[6];//指向尾后的位置 到达不了    不能执行解引用运算！！！！！
	// 使用 for 
	fot(int *begin = arr; begin != end; ++begin)
		cout << *begin  << endl;//输出每一个元素
	// 使用 while
	while(begin<end){//指针指向相关的对象 可以比较大小（单位（间隔）一样大）
		cout << *begin  << endl;//输出每一个元素
		++begin;
	}

	//标准库函数 begin() end() 函数得到指针
	int iarr = {0,1,2,3,4};//函数5个元素
	int *beg = begin(iarr);//首元素指针
	int *end = end(iarr);//尾后指针                 不能执行解引用运算！！！！！
	while(beg != end && *beg >=0) ++beg;//找第一个负值元素

## 多维数组 数组的数组

### 初始化
	constexpr size_t row =3, col = 4;
	int iarr[row][col];//定义未初始化

	//下标运算符（size_t 类型） for 循环初始化
	for(size_t i = 0; i != row; ++i){
		for(size_t j = 0; j != col; ++j){
			iarr[i][j] = i * col + j;//元素索引为其 值
			cout << iarr[i][j] << ' '; // 访问输出  数组元素
			}
		 cout << endl;	
	}

	// 范围for 初始化
	size_t cnt = 0;
	for (auto &row1 : iarr)//每一行 引用  int (&row1)[4] 是引用 引用一个含有4个整数的数组
	 {	
		for (auto &col1 : row1){// int &col1 每一行的每一个元素 引用  可以读写 除去最内层外 其它必须使用 引用
		col1 = cnt;//赋值
		cout << col1 << ' ';// 访问 输出数组元素
		++cnt;
		}
		cout << endl;
	 }	
		
### 访问 
	// 指针访问
	int (*p)[4] = iarr;//p 指向一个含有4个整数的数组 iarr的 第一行   圆括号必不可少
	p = &iarr[2]; // iarr的 第3行
	// for 循环访问
	for( auto p = ia; p != ia + row; ++p){// 相当于定义 int (*p)[4] = iarr; p 指向 含有4个整数的数组
		 for( auto q = *p; q != *p + col; ++q) // *p 为含有4个元素的数组 q指向数组的首元素，即指向一个整数 int *q = *p
			cout << *q << ' ';
		 cout << endl;
	}
	// 使用 标准库函数 begin() 和 end()
	for( auto p = begin(ia); p != end(ia); ++p){// 相当于定义 int (*p)[4] = iarr; p 指向 含有4个整数的数组
		 for( auto q = begin(*p); q != end(*p); ++q) // *p 为含有4个元素的数组 q指向数组的首元素，即指向一个整数 int *q = *p
			cout << *q << ' ';
		 cout << endl;
	}	
	// 使用类型别名
	using int_arr_tpye = int[4];// int_arr_tpye为包含4个元素的整形数组
	typedef int int_arr_tpye[4];// 效果同上
	for( int_arr_tpye *p = ia; p != ia + row; ++p){// 相当于定义 int (*p)[4] = iarr; p 指向 含有4个整数的数组
		 for( int *q = *p; q != *p + col; ++q) // *p 为含有4个元素的数组 q指向数组的首元素，即指向一个整数
			cout << *q << ' ';
		 cout << endl;
	}



