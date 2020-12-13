# c 示例代码

## 1.基础数据类型 打印相关 宏定义 变量

```c

#include <stdio.h>

//预定义 定义符号常量
#define NAME "fyf"
#define YEAR 1987
#define URL "https://www.qq.com"
/*	
 *	标识符identifier: 用于给变量，常量，函数，语句块等命名
 *
 *	常量：constant
 *	整数常量：250 , 12345
 *	实数常量：1.23 ， 35.99 ， 0.1 （float和double）
 *	字符常量：
 *		-普通字符：‘f’ , 'y' , 'F'
 *		-转义字符：'/n', '/t', '/b'
 *	字符串常量："FYFfyf"
 *
 *	定义符号常量：
 *		格式：#define 标识符 常量（宏定义）
 *		为与变量区分，符号常量通常用大写字母表示,非硬性规定
 *		#define YEAR 1987
 *		#define NAME "fyf"
 */

/*
 * 数据类型：
 * 	1.基本类型：
 * 		a.整数类型 int
 * 		b.浮点数类新 float double
 * 		c.字符类型 char
 * 		d.布尔类型 _Bool
 * 		e.枚举类型 enum
 * 	2.指针类型
 * 	3.构造类型：
 * 		a.数组类型
 * 		b.结构类型
 * 		c.联合类型
 * 	4.空类型
 *
 * 限定符：
 * 整数类型：short int , int , long int , long long int
 * short int <= int <=long int <=long long int
 * 
 * 浮点数类型：float , double , long double
 * 布尔类型：_Bool
 * 枚举类型：enum
 */


int main(void)
{
    printf("Name: %s,Year: %d年\n", NAME, YEAR);
// \n 输出换行 \ 同一行代码分行
/* \n 输出换行 \ 同一行代码分行 */
// printf print format
    printf("Hello World!\n\
		    ++++++\n\
		    +\n\
		    +\n\
		    ++++++\n\
   	            +\n\
   	            +\n\
   	            +\n\
   	            +\n");
//  定义变量 various variable
//  数据类型 变量名
    int a;
    char b;
    float c;
    double d;
//  变量 = 常量  （变量可改变，常量不可改变）
    a = 250;
    b = 'F';
    c = 3.14;
    d = 3.141592653;
// %d %c %f 转换说明
    printf("This is integer: %d\n" ,a);
    printf("This is charactor: %c\n" ,b);
    printf("This is float: %.2f\n" ,c);
    printf("This is double: %10.10f\n" ,d);
// sizeof()运算符：返回变量的大小
// 用法：sizeof(object对象)  sizeof(type_name类型)  sizeof object对象
    printf("The size of short int:%d\n", sizeof(short int));
    printf("The size of int:%d\n", sizeof(int));
    printf("The size of long int:%d\n", sizeof(long int));
    printf("The size of long long int:%d\n", sizeof(long long int));
    printf("The size of float:%d\n", sizeof(float));
    printf("The size of double:%d\n", sizeof(double));
    printf("The size of long double:%d\n", sizeof(long double));
    printf("The size of char:%d\n", sizeof(char));
    printf("The size of _Bool:%d\n", sizeof(_Bool));
    printf("The size of a:%d\n", sizeof(a));
    printf("The size of b:%d\n", sizeof(b));

    return 0;

}

````

## 2. 
```c


```


## 3. 
```c


```


## 4. 
```c


```




## 5. 
```c


```



## 6. 
```c


```

## 7. 
```c


```



## 8. 
```c


```


## 9. 
```c


```



## 10. 
```c


```


## 11. 
```c


```



## 12. 
```c


```


## 13. 
```c


```



## 14. 
```c


```

