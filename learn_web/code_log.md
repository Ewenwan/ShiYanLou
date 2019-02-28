# java代码日记

[在线运行](http://www.runoob.com/try/runcode.php?filename=HelloWorld&type=java)

[本地运行环境配置](http://www.runoob.com/java/java-environment-setup.html)

[Java 实例 实战](http://www.runoob.com/java/java-examples.html)

[java8 ](https://houbb.github.io/2019/02/27/java8-00-intro)

## Java 基础语法
一个Java程序可以认为是一系列对象的集合，而这些对象通过调用彼此的方法来协同工作。

下面简要介绍下类、对象、方法和实例变量的概念。

	对象：对象是类的一个实例，有状态和行为。
	      例如，一条狗是一个对象。
	      它的状态有：颜色、名字、品种；
	      行为有：摇尾巴、叫、吃等。
	类： 类是一个模板，它描述一类对象的行为和状态，一类总称。
	方法：方法就是行为，一个类可以有很多方法。
	      逻辑运算、数据修改以及所有动作都是在方法中完成的。
	实例变量：每个对象都有独特的实例变量，对象的状态由这些实例变量的值决定。
```java
public class Dog{
  String breed; // 狗 品种
  int age;      // 年龄
  String color; // 颜色
  void barking(){ // 行为1: 叫  方法 ====需要返回类型
  }
 
  void hungry(){  // 行为2: 饿  方法
  }
 
  void sleeping(){// 行为3: 睡觉  方法
  }
}

// 构造方法的名称必须与类同名，一个类可以有多个构造方法。
// 每个类都有构造方法。如果没有显式地为类定义构造方法，Java编译器将会为该类提供一个默认构造方法。
public class Puppy{
    public Puppy(){ // 构造函数 不需要 返回类型====
    }
    public Puppy(String name){ // 有参构造函数
      //这个构造器仅有一个参数：name
      System.out.println("小狗的名字是 : " + name ); 
    }
   
   // 类的主方法入口 main 函数
   public static void main(String []args){
      // 下面的语句将创建一个Puppy对象
      Puppy myPuppy = new Puppy( "tommy" );
   }
}

public class XiaoBai{
 public XiaoBai(String name){
	System.out.println("小狗的名字叫："+name);
  }
 public static void main(String[] args){
	XiaoBai myXiaoBai= new XiaoBai("pony");
  }
}

```
> 输出示例
```java
public class MyHelloW{ // MyHelloW类 
// 每个类都有构造方法。如果没有显式地为类定义构造方法，Java编译器将会为该类提供一个默认构造方法。
	public static void main(String []args){ // 主方法入口
  // 注：String args[] 与 String[] args 都可以执行，但推荐使用 String[] args，这样可以避免歧义和误读。
	    System.out.println("Hello World XW"); // 输出 带换行
		  System.out.print("NI HAO");// 输出 不带换行
	}
}
/*
运行以上实例，输出结果如下：

$ javac HelloWorld.java  // javac 该命令用于将 java 源文件xx.jave 编译为 class 字节码文件xx.class
$ java HelloWorld
Hello World
*/
```
### 基本语法
编写Java程序时，应注意以下几点：

	1. 大小写敏感：Java是大小写敏感的，这就意味着标识符Hello与hello是不同的。
	2. 类名：对于所有的类来说，类名的首字母应该大写。如果类名由若干单词组成，
	         那么每个单词的首字母应该大写，例如 MyFirstJavaClass 。
	3. 方法名：所有的方法名都应该以小写字母开头。如果方法名含有若干单词，
	         则后面的每个单词首字母大写，例如countDays。
	4. 源文件名：源文件名必须和类名相同。当保存文件的时候，
	         你应该使用类名作为文件名保存（切记Java是大小写敏感的），
		 文件名的后缀为.java。（如果文件名和类名不相同则会导致编译错误）。
	5. 主方法入口：所有的Java 程序由public static void main(String[] args)方法开始执行。
	
> Java标识符

	Java所有的组成部分都需要名字。类名、变量名以及方法名都被称为标识符。

	关于Java标识符，有以下几点需要注意：

	所有的标识符都应该以字母（A-Z或者a-z）,美元符（$）、或者下划线（_）开始
	首字符之后可以是字母（A-Z或者a-z）,美元符（$）、下划线（_）或数字的任何字符组合
	关键字不能用作标识符
	标识符是大小写敏感的
	合法标识符举例：age、$salary、_value、__1_value  // 注意可以用$美元符号开头
	非法标识符举例：123abc、-salary

> Java修饰符

	像其他语言一样，Java可以使用修饰符来修饰类中方法和属性。
	主要有两类修饰符：

	访问控制修饰符 : default默认, public公开, protected保护, private私有
		public--都可访问(公有)   
		private--类内可访问（私有）
		protected--包内和子类可访问（保护）
		不写(default)--包内可访问 （默认）
	
	非访问控制修饰符 : final终结者, abstract抽象, static静态, synchronized同步
		static创建的类成员独立于该类的任何对象。不需要通过对象访问。
		使用static修饰符的变量或方法属于类本身，而不是类的实例对象。

		final修饰一个类时，表明这个类不能被继承。

		synchronized java线程同步中的同步锁。
                synchronized 关键字声明的方法同一时间只能被一个线程访问。
		
		abstract 修饰类和方法时，表示抽象类/抽象方法。

> Java枚举
Java 5.0引入了枚举，枚举限制变量只能是预先设定好的值。使用枚举可以减少代码中的bug。

例如，我们为果汁店设计一个程序，它将限制果汁为小杯、中杯、大杯。这就意味着它不允许顾客点除了这三种尺寸外的果汁。
```java
// 枚举可以单独声明或者声明在类里面。方法、变量、构造函数也可以在枚举中定义。
class FreshJuice {
   enum FreshJuiceSize{ SMALL, MEDIUM , LARGE }// 果汁尺寸：小杯、中杯、大杯
   FreshJuiceSize size;
}
 
public class FreshJuiceTest {
   public static void main(String []args){
      FreshJuice juice = new FreshJuice();
      juice.size = FreshJuice.FreshJuiceSize.MEDIUM  ;// 中杯
   }
}
```

> 访问实例变量和方法
```java
public class Puppy{ // class 类 Puppy
   int puppyAge; // 成员变量 属性:年龄
   // 有参构造函数
   public Puppy(String name){
      // 这个构造器仅有一个参数：name
      System.out.println("小狗的名字是 : " + name ); 
   }
   // 设置 年龄的 方法 有返回类型
   public void setAge( int age ){
       puppyAge = age;
   }
   // 获取年龄 的 方法 有返回类型
   public int getAge( ){
       System.out.println("小狗的年龄为 : " + puppyAge ); 
       return puppyAge;
   }
  
  // 主方法入口
   public static void main(String []args){
      /* 创建对象 myPuppy*/
      Puppy myPuppy = new Puppy( "tommy" );
      /* 通过方法来设定 年龄 age */
      myPuppy.setAge( 2 );
      /* 调用另一个方法获取 年龄 age */
      myPuppy.getAge( );
      /*你也可以像下面这样访问成员变量 */
      System.out.println("变量值 : " + myPuppy.puppyAge ); 
   }
}

```

> 源文件声明规则

当在一个源文件中定义多个类，并且还有import语句和package语句时，要特别注意这些规则。

* 一个源文件中只能有一个 public类
*  一个源文件可以有多个 非public类
* 源文件的名称应该 和 public类的类名保持一致。
  例如：源文件中 public类的类名是Employee，那么源文件应该命名为Employee.java。
* 如果一个类定义在某个包中，那么package语句 应该在 源文件的首行。
* 如果源文件包含import语句，那么应该放在package语句和类定义之间。
* 如果没有package语句，那么import语句应该在源文件中最前面。
* import语句和package语句对源文件中定义的所有类都有效。在同一源文件中，不能给不同的类不同的包声明。

```java
import java.io.*; // 会载入java_installation/java/io路径下的所有类

// 一个简单的例子 我们创建两个类：Employee 和 EmployeeTest。
// EmployeeTest 调用 Employee   

// Employee.java 文件代码： 员工类
import java.io.*; // 会载入java_installation/java/io路径下的所有类
public class Employee{
// 属性
   String name;  // 名字
   int age;      // 年龄
   String designation; // 职位
   double salary;      // 薪水
   
   // Employee 类的构造器 构造函数 无返回类型
   public Employee(String name){
      this.name = name;
   }
   // 设置age的值 方法
   public void empAge(int empAge){
      age =  empAge;
   }
   // 设置designation的值
   public void empDesignation(String empDesig){
      designation = empDesig;
   }
   // 设置salary的值 
   public void empSalary(double empSalary){
      salary = empSalary;
   }
   
   // 打印类属性信息 员工信息
   public void printEmployee(){
      System.out.println("名字:"+ name );
      System.out.println("年龄:" + age );
      System.out.println("职位:" + designation );
      System.out.println("薪水:" + salary);
   }
}

// 程序都是从main方法开始执行。为了能运行这个程序，必须包含main方法并且创建一个实例对象。
// EmployeeTest.java 文件代码：
import java.io.*;

public class EmployeeTest{
   
   // 仅包含一个 主方法
   public static void main(String []args)
   {
      /* 使用构造器创建两个对象 */
      Employee empOne = new Employee("xiaoming");
      Employee empTwo = new Employee("xiaohong");
 
      // 调用这两个对象的成员方法
      empOne.empAge(26);
      empOne.empDesignation("高级程序员");
      empOne.empSalary(1000);
      empOne.printEmployee();
 
      empTwo.empAge(21);
      empTwo.empDesignation("菜鸟程序员");
      empTwo.empSalary(500);
      empTwo.printEmployee();
   }
}

/*
编译这两个文件并且运行 EmployeeTest 类，可以看到如下结果：

$ javac EmployeeTest.java
$ java EmployeeTest 
名字:xiaoming
年龄:26
职位:高级程序员
薪水:1000.0
名字:xiaohong
年龄:21
职位:菜鸟程序员
薪水:500.0

*/
```

> 基本数据类型 


```java
public class PrimitiveTypeTest {  
    public static void main(String[] args) {  
        // byte  
        System.out.println("基本类型：byte 二进制位数：" + Byte.SIZE);  
        System.out.println("包装类：java.lang.Byte");  
        System.out.println("最小值：Byte.MIN_VALUE=" + Byte.MIN_VALUE);  
        System.out.println("最大值：Byte.MAX_VALUE=" + Byte.MAX_VALUE);  
        System.out.println();  
  
        // short  
        System.out.println("基本类型：short 二进制位数：" + Short.SIZE);  
        System.out.println("包装类：java.lang.Short");  
        System.out.println("最小值：Short.MIN_VALUE=" + Short.MIN_VALUE);  
        System.out.println("最大值：Short.MAX_VALUE=" + Short.MAX_VALUE);  
        System.out.println();  
  
        // int  
        System.out.println("基本类型：int 二进制位数：" + Integer.SIZE);  
        System.out.println("包装类：java.lang.Integer");  
        System.out.println("最小值：Integer.MIN_VALUE=" + Integer.MIN_VALUE);  
        System.out.println("最大值：Integer.MAX_VALUE=" + Integer.MAX_VALUE);  
        System.out.println();  
  
        // long  
        System.out.println("基本类型：long 二进制位数：" + Long.SIZE);  
        System.out.println("包装类：java.lang.Long");  
        System.out.println("最小值：Long.MIN_VALUE=" + Long.MIN_VALUE);  
        System.out.println("最大值：Long.MAX_VALUE=" + Long.MAX_VALUE);  
        System.out.println();  
  
        // float  
        System.out.println("基本类型：float 二进制位数：" + Float.SIZE);  
        System.out.println("包装类：java.lang.Float");  
        System.out.println("最小值：Float.MIN_VALUE=" + Float.MIN_VALUE);  
        System.out.println("最大值：Float.MAX_VALUE=" + Float.MAX_VALUE);  
        System.out.println();  
  
        // double  
        System.out.println("基本类型：double 二进制位数：" + Double.SIZE);  
        System.out.println("包装类：java.lang.Double");  
        System.out.println("最小值：Double.MIN_VALUE=" + Double.MIN_VALUE);  
        System.out.println("最大值：Double.MAX_VALUE=" + Double.MAX_VALUE);  
        System.out.println();  
  
        // char  
        System.out.println("基本类型：char 二进制位数：" + Character.SIZE);  
        System.out.println("包装类：java.lang.Character");  
        // 以数值形式而不是字符形式将Character.MIN_VALUE输出到控制台  
        System.out.println("最小值：Character.MIN_VALUE="  
                + (int) Character.MIN_VALUE);  
        // 以数值形式而不是字符形式将Character.MAX_VALUE输出到控制台  
        System.out.println("最大值：Character.MAX_VALUE="  
                + (int) Character.MAX_VALUE); 
		
		
	// 在 Java 中使用 final 关键字来修饰常量，声明方式和变量类似：
        final double PI = 3.1415927;
	
// 转换从低级到高级。
// 低  ------------------------------------>  高
// byte,short,char—> int —> long—> float —> double 

int a, b, c;         // 声明三个int型整数：a、 b、c
int d = 3, e = 4, f = 5; // 声明三个整数并赋予初值
byte z = 22;         // 声明并初始化 z
String s = "runoob";  // 声明并初始化字符串 s
double pi = 3.14159; // 声明了双精度浮点型变量 pi
char x = 'x';        // 声明变量 x 的值是字符 'x'。 
  
    static int allClicks=0;    // 类变量（静态变量）, 独立于方法之外的变量，用 static 修饰。
 // 无论一个类创建了多少个对象，类只拥有类变量的一份拷贝
 // 静态变量在第一次被访问时创建，在程序结束时销毁。
 
 
    String str="hello world";  // 实例变量, 独立于方法之外的变量，不过没有 static 修饰。
    // 实例变量在对象创建的时候创建，在对象被销毁的时候销毁
    // 实例变量可以声明在使用前或者使用后；
    // 访问修饰符可以修饰实例变量
    // 实例变量对于类中的方法、构造方法或者语句块是可见的
    
   // 这个实例变量对子类可见
   public String name;
   // 私有 实例变量，仅在该类可见
   private double salary;
    
    
    public void method(){
        int i =0;  // 局部变量,类的方法中的变量。
// 当它们执行完成后，局部变量 将会被销毁；
// 访问修饰符不能用于局部变量
// 局部变量只在声明它的方法、构造方法或者语句块中可见；
// 局部变量是在栈上分配的。
// 局部变量没有默认值，所以局部变量被声明后，必须经过初始化，才可以使用。
// int age; // 错误，局部变量被声明后，必须经过初始化
    }
    
// instanceof 运算符，检查该对象是否是一个特定类型
String name = "James";
boolean result = name instanceof String; // 由于 name 是 String 类型，所以返回真

    }  
}

```

> while for if else switch 语句 和 c++语言差不多

## Java Number 基本数据类型 类 & Math 类

所有的包装类（Integer、Long、Byte、Double、Float、Short）都是抽象类 Number 的子类。

![](http://www.runoob.com/wp-content/uploads/2013/12/number1.png)

装箱---装包---当内置数据类型被当作对象使用的时候，编译器会把内置类型装箱为包装类。

拆箱---拆包---相似的，编译器也可以把一个对象拆箱为内置类型。

Number 类属于 java.lang 包

```java
public class Test{
 
   public static void main(String args[]){
      Integer x = 5; // 由于x是一个对象，所以编译器要对x 进行 装箱
      x =  x + 10;   // 为了使x能进行加运算，所以要对x 进行 拆箱。
      System.out.println(x); 
   }
}

```
> Java 的 Math 包

Java 的 Math 包含了用于执行基本数学运算的属性和方法，如初等指数、对数、平方根和三角函数。

Math 的方法都被定义为 static 形式，通过 Math 类可以在主函数中直接调用。
```java
public class Test {  
    public static void main (String []args)  
    {  
        System.out.println("90 度的正弦值：" + Math.sin(Math.PI/2));  
        System.out.println("0度的余弦值：" + Math.cos(0));  
        System.out.println("60度的正切值：" + Math.tan(Math.PI/3));  
        System.out.println("1的反正切值： " + Math.atan(1));  
        System.out.println("π/2的角度值：" + Math.toDegrees(Math.PI/2));  
        System.out.println(Math.PI);  
    }  
}
/*
以上实例编译运行结果如下：
90 度的正弦值：1.0
0度的余弦值：1.0
60度的正切值：1.7320508075688767
1的反正切值： 0.7853981633974483
π/2的角度值：90.0
3.141592653589793
*/

// Number & Math 类方法
xxxValue()  // 将 Number 对象转换为xxx数据类型的值并返回。 拆包
compareTo() // 将number对象与参数比较。
equals()    // 判断number对象是否与参数相等。
toString()  // 以字符串形式返回值。
parseInt()  // 将字符串解析为int类型。
abs()  ceil() floor() 

rint()     // 返回与参数最接近的整数。返回类型为double。
round()    // 它表示四舍五入

min() max() exp() log() pow() sqrt() sin() cos() tan() 
toDegrees() // 将参数转化为角度。
toRadians() // 将角度转换为弧度。
random()    // 返回一个随机数。

```


## Java Character 类
	Character 类用于对单个字符进行操作。

	Character 类在对象中包装一个基本类型 char 的值
```java
// 原始字符 'a' 装箱到 Character 对象 ch 中
Character ch = 'a';
```
## Java String 类
	String 类来创建和操作字符串。
	String 类是不可改变的，所以你一旦创建了 String 对象，那它的值就无法改变了。
	
```java
String greeting = "菜鸟教程"; // 直接创建字符串
// 字符数组创建 字符串
char[] helloArray = { 'r', 'u', 'n', 'o', 'o', 'b'};
String helloString = new String(helloArray); 	

String site = "www.runoob.com";
int len = site.length(); // 字符串长度
System.out.println( "菜鸟教程网址长度 : " + len );

// 连接字符串 String 类提供了连接两个字符串的方法：
string1.concat(string2);
"1" + string1 + "www.runoob.com" // 直接使用 + 号进行连接


// 输出格式化字符串
System.out.printf("浮点型变量的值为 " +
                  "%f, 整型变量的值为 " +
                  " %d, 字符串变量的值为 " +
                  "is %s", floatVar, intVar, stringVar);

// 创建格式化字符串
String fs;
fs = String.format("浮点型变量的值为 " +
                   "%f, 整型变量的值为 " +
                   " %d, 字符串变量的值为 " +
                   " %s", floatVar, intVar, stringVar);
```
	
## Java StringBuffer 和 StringBuilder 类
	当对字符串进行修改的时候，需要使用 StringBuffer 和 StringBuilder 类。
	和 String 类不同的是，
	StringBuffer 和 StringBuilder 类的对象能够被多次的修改，
	并且不产生新的未使用对象。
	
	StringBuilder 类在 Java 5 中被提出，
	它和 StringBuffer 之间的最大不同在于 StringBuilder 的方法不是线程安全的（不能同步访问）。

	由于 StringBuilder 相较于 StringBuffer 有速度优势，
	所以多数情况下建议使用 StringBuilder 类。
	然而在应用程序要求线程安全的情况下，则必须使用 StringBuffer 类。
	
```java
public class Test{
  public static void main(String args[]){
    StringBuffer sBuffer = new StringBuffer("菜鸟教程官网：");
    sBuffer.append("www");
    sBuffer.append(".runoob");
    sBuffer.append(".com");
    System.out.println(sBuffer);  
  }
}
// 以上实例编译运行结果如下：
// 菜鸟教程官网：www.runoob.com

```
	

## Java 数组
	数组对于每一门编程语言来说都是重要的数据结构之一，当然不同语言对数组的实现及处理也不尽相同。

	Java 语言中提供的数组是用来存储固定大小的同类型元素。

	你可以声明一个数组变量，如 numbers[100] 
	来代替直接声明 100 个独立变量 number0，number1，....，number99。
```java	
声明数组变量

首先必须声明数组变量，才能在程序中使用数组。

下面是声明数组变量的语法：

dataType[] arrayRefVar;   // 首选的方法
或
dataType arrayRefVar[];  // 效果相同，但不是首选方法,风格是来自 C/C++ 语言
	
double[] myList;         // 首选的方法
或
double myList[];         //  效果相同，但不是首选方法

创建数组
Java语言使用new操作符来创建数组，语法如下：

arrayRefVar = new dataType[arraySize];
```
> 示例1

```java
public class TestArray {
   public static void main(String[] args) {
      // 数组大小
      int size = 10;
      // 定义数组
      double[] myList = new double[size];
      myList[0] = 5.6;
      myList[1] = 4.5;
      myList[2] = 3.3;
      myList[3] = 13.2;
      myList[4] = 4.0;
      myList[5] = 34.33;
      myList[6] = 34.0;
      myList[7] = 45.45;
      myList[8] = 99.993;
      myList[9] = 11123;
      // 计算所有元素的总和
      double total = 0;
      for (int i = 0; i < size; i++) {
         total += myList[i];
      }
      System.out.println("总和为： " + total);
   }
}

```
> 示例2
```java
public class TestArray {
   public static void main(String[] args) {
      double[] myList = {1.9, 2.9, 3.4, 3.5};
 
      // 打印所有数组元素
      for (int i = 0; i < myList.length; i++) {
         System.out.println(myList[i] + " ");
      }
/*
// for-each 循环或者加强型循环，它能在不使用下标的情况下遍历数组。
// 打印所有数组元素
for (double element : myList) {
     System.out.println(element);
}
*/
      
      // 计算所有元素的总和
      double total = 0;
      for (int i = 0; i < myList.length; i++) {
         total += myList[i];
      }
      System.out.println("Total is " + total);
      // 查找最大元素
      double max = myList[0];
      for (int i = 1; i < myList.length; i++) {
         if (myList[i] > max) max = myList[i];
      }
      System.out.println("Max is " + max);
   }
}
/*
以上实例编译运行结果如下：

1.9
2.9
3.4
3.5
Total is 11.7
Max is 3.5
*/
```
> 数组其他用法

```java
// 数组作为函数的参数
public static void printArray(int[] array) {
  for (int i = 0; i < array.length; i++) {
  // 也可用 for-each循环
    System.out.print(array[i] + " ");
  }
}
printArray(new int[]{3, 1, 2, 6, 4, 2});// 调用

// 数组作为函数的返回值
public static int[] reverse(int[] list) {
  int[] result = new int[list.length];
 
  for (int i = 0, j = result.length - 1; i < list.length; i++, j--) {
    result[j] = list[i]; // 数组元素逆序=================
  }
  return result;
}


// 多维数组
type arrayName = new type[arraylenght1][arraylenght2];
String str[][] = new String[3][4];
int a[][]      = new int[2][3];// 可以看成一个两行三列的数组

String s[][] = new String[2][];
s[0] = new String[2];
s[1] = new String[3];  // 每一行不同尺寸
s[0][0] = new String("Good");
s[0][1] = new String("Luck");
s[1][0] = new String("to");
s[1][1] = new String("you");
s[1][2] = new String("!");


// Arrays 类
// java.util.Arrays 类能方便地操作数组，它提供的所有方法都是静态的。
/*
具有以下功能：

给数组赋值：通过 fill 方法。
      public static void fill(int[] a, int val)
对数组排序：通过 sort 方法,按升序。
      public static void sort(Object[] a)
比较数组：通过 equals 方法比较数组中元素值是否相等。
      public static boolean equals(long[] a, long[] a2)
查找数组元素：通过 binarySearch 方法能对排序好的数组进行二分查找法操作。
      public static int binarySearch(Object[] a, Object key) 
*/
```

# Java 日期时间  Date  java.util 包  
```java
// 获取当前日期时间
import java.util.Date;
  
public class DateDemo {
   public static void main(String args[]) {
       // 初始化 Date 对象
       Date date = new Date();
        
       // 使用 toString() 函数显示日期时间
       System.out.println(date.toString());
       
       // 使用 SimpleDateFormat 格式化日期
       SimpleDateFormat ft = new SimpleDateFormat ("yyyy-MM-dd hh:mm:ss");
/*
其中 yyyy 是完整的公元年，MM 是月份，dd 是日期，HH:mm:ss 是时、分、秒。

注意:有的格式大写，有的格式小写，例如 MM 是月份，mm 是分；HH 是 24 小时制，而 hh 是 12 小时制。
*/
       System.out.println("当前时间为: " + ft.format(dNow));
    
    
// 使用printf格式化日期============= 
    //c的使用  
    System.out.printf("全部日期和时间信息：%tc%n",date);          
    //f的使用  
    System.out.printf("年-月-日格式：%tF%n",date);  
    //d的使用  
    System.out.printf("月/日/年格式：%tD%n",date);  
    //r的使用  
    System.out.printf("HH:MM:SS PM格式（12时制）：%tr%n",date);  
    //t的使用  
    System.out.printf("HH:MM:SS格式（24时制）：%tT%n",date);  
    //R的使用  
    System.out.printf("HH:MM格式（24时制）：%tR",date);  
    
   }
}
// 运行结果
// Wed Feb 20 11:26:06 UTC 2019


// 解析字符串为时间===================
// SimpleDateFormat 类有一些附加的方法，特别是parse()，
// 它试图按照给定的SimpleDateFormat 对象的格式化存储来解析字符串。
import java.util.*;
import java.text.*;
  
public class DateDemo {
 
   public static void main(String args[]) {
      SimpleDateFormat ft = new SimpleDateFormat ("yyyy-MM-dd"); 
 
      String input = args.length == 0 ? "1818-11-11" : args[0]; 
 
      System.out.print(input + " Parses as "); 
 
      Date t; 
 
      try { 
          t = ft.parse(input); 
          System.out.println(t); 
      } catch (ParseException e) { 
          System.out.println("Unparseable using " + ft); 
      }
   }
}

```
 
 
## Java 正则表达式
http://www.runoob.com/java/java-regular-expressions.html
```java
import java.util.regex.*;
 
class RegexExample1{
   public static void main(String args[]){
      String content = "I am noob " +
        "from runoob.com.";
 
      String pattern = ".*runoob.*";// 匹配模式
 
      boolean isMatch = Pattern.matches(pattern, content);// 匹配器按模式进行匹配
      
      System.out.println("字符串中是否包含了 'runoob' 子字符串? " + isMatch);
   }
}


```


## java方法
```java
public class TestMax {
   /** 主方法 */
   public static void main(String[] args) {
   
   
      int i = 5;
      int j = 2;
      int k = max(i, j);
      System.out.println( i + " 和 " + j + " 比较，最大值是：" + k);
      
      
    int num1 = 1;
    int num2 = 2;
    System.out.println("交换前 num1 的值为：" +
                        num1 + " ，num2 的值为：" + num2);
    // 调用swap方法
    swap(num1, num2);
    System.out.println("交换后 num1 的值为：" +
                       num1 + " ，num2 的值为：" + num2);
      
   }
 
   /** 返回两个整数变量较大的值 */
   public static int max(int num1, int num2) {
      int result;
      if (num1 > num2)
         result = num1;
      else
         result = num2;
 
      return result; 
   }
   // 函数重载===== 重载(Overload) 
public static double max(double num1, double num2) {
  if (num1 > num2)
    return num1;
  else
    return num2;
}
   
  /** 交换两个变量的方法 */
  public static void swap(int n1, int n2) {
    System.out.println("\t进入 swap 方法");
    System.out.println("\t\t交换前 n1 的值为：" + n1
                         + "，n2 的值：" + n2);
    // 交换 n1 与 n2的值
    int temp = n1;
    n1 = n2;
    n2 = temp;
 
    System.out.println("\t\t交换后 n1 的值为 " + n1
                         + "，n2 的值：" + n2);
  }
   
}


// 命令行参数的使用=============================
public class CommandLine {
   public static void main(String args[]){ 
      for(int i=0; i<args.length; i++){
         System.out.println("args[" + i + "]: " + args[i]);
      }
   }
}
/*
如下所示，运行这个程序：
$ javac CommandLine.java 
$ java CommandLine this is a command line 200 -100
args[0]: this
args[1]: is
args[2]: a
args[3]: command
args[4]: line
args[5]: 200
args[6]: -100
*/


// 可变参数===============
public class VarargsDemo {
    public static void main(String args[]) {
        // 调用可变参数的方法
        printMax(34, 3, 3, 2, 56.5);
        printMax(new double[]{1, 2, 3});
    }
    // 可变参数 typeName... parameterName ===========
    public static void printMax( double... numbers) {
        if (numbers.length == 0) {
            System.out.println("No argument passed");
            return;
        }
        double result = numbers[0];// 初始最大值
        for (int i = 1; i <  numbers.length; i++){
            if (numbers[i] >  result) {
                result = numbers[i];  // 最大值
            }
        }
        System.out.println("The max value is " + result);
    }
}


// 类似c++里面的 类析构函数=======
protected void finalize()
{
   // 在这里终结代码
}

```


## Java 流(Stream)、文件(File)和IO
http://www.runoob.com/java/java-files-io.html
```java
// 从控制台读取多字符输入
//使用 BufferedReader 在控制台读取字符 
import java.io.*;
public class BRRead {
    public static void main(String args[]) throws IOException {
        char c;
        // 使用 System.in 创建 BufferedReader
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("输入字符, 按下 'q' 键退出。");
        // 读取字符
        do {
            c = (char) br.read();
            System.out.println(c);
        } while (c != 'q');
    }
}
/*
以上实例编译运行结果如下:

输入字符, 按下 'q' 键退出。
runoob
r
u
n
o
o
b


q
q
*/


// 从控制台读取字符串=========
//使用 BufferedReader 在控制台读取字符
import java.io.*;
 
public class BRReadLines {
    public static void main(String args[]) throws IOException {
        // 使用 System.in 创建 BufferedReader
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String str;
        System.out.println("Enter lines of text.");
        System.out.println("Enter 'end' to quit.");
        do {
            str = br.readLine();
            System.out.println(str);
        } while (!str.equals("end"));
    }
}
/*
以上实例编译运行结果如下:

Enter lines of text.
Enter 'end' to quit.
This is line one
This is line one
This is line two
This is line two
end
end
*/

// JDK 5 后的版本我们也可以使用 Java Scanner 类来获取控制台的输入。
import java.util.Scanner; 
 
public class ScannerDemo {
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        // 从键盘接收数据
 
        // next方式接收字符串
        System.out.println("next方式接收：");
        // 判断是否还有输入
        if (scan.hasNext()) {
            String str1 = scan.next();
	    // String str2 = scan.nextLine();
            System.out.println("输入的数据为：" + str1);
        }
        scan.close();
    }
}


```

## Java 异常处理
```java
// 文件名 : ExcepTest.java
import java.io.*;
public class ExcepTest{
 
   public static void main(String args[]){
      try
      {
         int a[] = new int[2];
         System.out.println("Access element three :" + a[3]);
      }
      catch(ArrayIndexOutOfBoundsException e)
      {
         System.out.println("Exception thrown  :" + e);
      }
      System.out.println("Out of the block");
   }
}
/*
以上代码编译运行输出结果如下：
Exception thrown  :java.lang.ArrayIndexOutOfBoundsException: 3
Out of the block
*/


//  多重捕获块
// 一个 try 代码块后面跟随多个 catch 代码块的情况就叫多重捕获。
try {
    file = new FileInputStream(fileName);
    x = (byte) file.read();
} catch(FileNotFoundException f) { // Not valid!
    f.printStackTrace();
    return -1;
} catch(IOException i) {
    i.printStackTrace();
    return -1;
}
finally{
  // 程序代码, 可以运行清理类型等收尾善后性质的语句。
  // 无论是否发生异常，finally 代码块中的代码总会被执行。
}

```



## Java 继承
```java
// 类的继承格式
class 父类 {
}

class 子类 extends 父类 {
}
```
![](http://www.runoob.com/wp-content/uploads/2013/12/types_of_inheritance.png)

继承的特性

	1. 子类拥有父类非 private 的属性、方法。
	2. 子类可以拥有自己的属性和方法，即子类可以对父类进行扩展。
	3. 子类可以用自己的方式实现父类的方法。 覆盖
	4. Java 的继承是单继承，但是可以多重继承，单继承就是一个子类只能继承一个父类，
	   多重继承就是，例如 A 类继承 B 类，B 类继承 C 类，
	   所以按照关系就是 C 类是 B 类的父类，B 类是 A 类的父类，
	   这是 Java 继承区别于 C++ 继承的一个特性。
	5. 提高了类之间的耦合性（继承的缺点，耦合度高就会造成代码之间的联系越紧密，代码独立性越差）。

## Java 重写(Override)与重载(Overload)
```java

// 重写(Override)
// 重写是子类对父类的允许访问的方法的实现过程进行重新编写, 返回值和形参都不能改变。
// 即外壳不变，核心重写！

class Animal{
   public void move(){
      System.out.println("动物可以移动");
   }
}
 
class Dog extends Animal{
   public void move(){
      System.out.println("狗可以跑和走"); // 重写(Override)
   }
}
 
public class TestDog{
   public static void main(String args[]){
      Animal a = new Animal(); // Animal 对象
      Animal b = new Dog(); // Dog 对象
 
      a.move();// 执行 Animal 类的方法
 
      b.move();//执行 Dog 类的方法
   }
}

/*
以上实例编译运行结果如下：

动物可以移动
狗可以跑和走

*/

/*
重载(Overload)
重载(overloading) 是在一个类里面，方法名字相同，而参数不同。返回类型可以相同也可以不同。
每个重载的方法（或者构造函数）都必须有一个独一无二的参数类型列表。
最常用的地方就是构造器的重载。

区别点     重载方法     重写方法
参数列表   必须修改     一定不能修改
返回类型   可以修改     一定不能修改
异常       可以修改    可以减少或删除，一定不能抛出新的或者更广的异常
访问       可以修改    一定不能做更严格的限制（可以降低限制）
*/
```

## Java 多态 多态性是对象多种表现形式的体现 
多态是同一个行为具有多个不同表现形式或形态的能力。

多态就是同一个接口，使用不同的实例而执行不同操作，如图所示：

![](http://www.runoob.com/wp-content/uploads/2013/12/dt-java.png)

多态存在的三个必要条件

	继承
	重写
	父类引用 指向 子类对象！！！！  Parent p = new Child();
	
当使用多态方式调用方法时，首先检查父类中是否有该方法，如果没有，则编译错误；如果有，再去调用子类的同名方法。

多态的好处：可以使程序有良好的扩展，并可以对所有类的对象进行通用处理。

```java

public class Test {
    public static void main(String[] args) {
      show(new Cat());  // 以 Cat 对象调用 show 方法
      show(new Dog());  // 以 Dog 对象调用 show 方法
                
      Animal a = new Cat();  // 向上转型  
      a.eat();               // 调用的是 Cat 的 eat
      Cat c = (Cat)a;        // 向下转型  
      c.work();        // 调用的是 Cat 的 work
  }  
            
    public static void show(Animal a)  {
      a.eat();  
        // 类型判断
        if (a instanceof Cat)  {  // 猫做的事情 
            Cat c = (Cat)a;  
            c.work();  
        } else if (a instanceof Dog) { // 狗做的事情 
            Dog c = (Dog)a;  
            c.work();  
        }  
    }  
}
 
abstract class Animal {  
    abstract void eat();  
}  
  
class Cat extends Animal {  
    public void eat() {  
        System.out.println("吃鱼");  
    }  
    public void work() {  
        System.out.println("抓老鼠");  
    }  
}  
  
class Dog extends Animal {  
    public void eat() {  
        System.out.println("吃骨头");  
    }  
    public void work() {  
        System.out.println("看家");  
    }  
}
/*
执行以上程序，输出结果为：

吃鱼
抓老鼠
吃骨头
看家
吃鱼
抓老鼠

*/


// 虚方法
/*
我们将介绍在Java中，当设计类时，被重写的方法的行为怎样影响多态性。

我们已经讨论了方法的重写，也就是子类能够重写父类的方法。

当子类对象调用重写的方法时，调用的是子类的方法，而不是父类中被重写的方法。

要想调用父类中被重写的方法，则必须使用关键字super。
*/


```


## Java 抽象类
在面向对象的概念中，所有的对象都是通过类来描绘的，但是反过来，并不是所有的类都是用来描绘对象的，如果一个类中没有包含足够的信息来描绘一个具体的对象，这样的类就是抽象类。

抽象类除了不能实例化对象之外，类的其它功能依然存在，成员变量、成员方法和构造方法的访问方式和普通类一样。

由于抽象类不能实例化对象，所以抽象类必须被继承，才能被使用。也是因为这个原因，通常在设计阶段决定要不要设计抽象类。

父类包含了子类集合的常见的方法，但是由于父类本身是抽象的，所以不能使用这些方法。

在Java中抽象类表示的是一种继承关系，一个类只能继承一个抽象类，而一个类却可以实现多个接口。

http://www.runoob.com/java/java-abstraction.html


## Java 封装
在面向对象程式设计方法中，封装（英语：Encapsulation）是指一种将抽象性函式接口的实现细节部份包装、隐藏起来的方法。

封装可以被认为是一个保护屏障，防止该类的代码和数据被外部类定义的代码随机访问。

要访问该类的代码和数据，必须通过严格的接口控制。

封装最主要的功能在于我们能修改自己的实现代码，而不用修改那些调用我们代码的程序片段。

适当的封装可以让程式码更容易理解与维护，也加强了程式码的安全性。

封装的优点

1. 良好的封装能够减少耦合。

2. 类内部的结构可以自由修改。

3. 可以对成员变量进行更精确的控制。

4. 隐藏信息，实现细节。

```java
public class Person{
    private String name;
    private int age;
    public int getAge(){
      return age;
    }
    public String getName(){
      return name;
    }
    public void setAge(int age){
      this.age = age;
    }
    public void setName(String name){
      this.name = name;
    }
}
// 采用 this 关键字是为了
// 解决实例变量（private String name）和 局部变量（setName(String name)中的name变量）之间发生的同名的冲突。

```

## Java 接口

接口（英文：Interface），在JAVA编程语言中是一个抽象类型，是抽象方法的集合，接口通常以interface来声明。一个类通过继承接口的方式，从而来继承接口的抽象方法。

接口并不是类，编写接口的方式和类很相似，但是它们属于不同的概念。类描述对象的属性和方法。接口则包含类要实现的方法。

除非实现接口的类是抽象类，否则该类要定义接口中的所有方法。

接口无法被实例化，但是可以被实现。一个实现接口的类，必须实现接口内所描述的所有方法，否则就必须声明为抽象类。另外，在 Java 中，接口类型可用来声明一个变量，他们可以成为一个空指针，或是被绑定在一个以此接口实现的对象。

接口与类相似点：

	一个接口可以有多个方法。
	接口文件保存在 .java 结尾的文件中，文件名使用接口名。
	接口的字节码文件保存在 .class 结尾的文件中。
	接口相应的字节码文件必须在与包名称相匹配的目录结构中。
	接口与类的区别：
	接口不能用于实例化对象。
	接口没有构造方法。
	接口中所有的方法必须是抽象方法。
	接口不能包含成员变量，除了 static 和 final 变量。
	接口不是被类继承了，而是要被类实现。
	接口支持多继承。
接口特性
接口中每一个方法也是隐式抽象的,接口中的方法会被隐式的指定为 public abstract（只能是 public abstract，其他修饰符都会报错）。
接口中可以含有变量，但是接口中的变量会被隐式的指定为 public static final 变量（并且只能是 public，用 private 修饰会报编译错误）。
接口中的方法是不能在接口中实现的，只能由实现接口的类来实现接口中的方法。
抽象类和接口的区别

	1. 抽象类中的方法可以有方法体，就是能实现方法的具体功能，但是接口中的方法不行。
	2. 抽象类中的成员变量可以是各种类型的，
	   而接口中的成员变量只能是 public static final 类型的。
	3. 接口中不能含有静态代码块以及静态方法(用 static 修饰的方法)，
	   而抽象类是可以有静态代码块和静态方法。
	4. 一个类只能继承一个抽象类，而一个类却可以实现多个接口。

http://www.runoob.com/java/java-interfaces.html

## Java 包(package)
为了更好地组织类，Java 提供了包机制，用于区别类名的命名空间。

包的作用
1、把功能相似或相关的类或接口组织在同一个包中，方便类的查找和使用。

2、如同文件夹一样，包也采用了树形目录的存储方式。同一个包中的类名字是不同的，不同的包中的类的名字是可以相同的，当同时调用两个不同包中相同类名的类时，应该加上包名加以区别。因此，包可以避免名字冲突。

3、包也限定了访问权限，拥有包访问权限的类才能访问某个包中的类。

Java 使用包（package）这种机制是为了防止命名冲突，访问控制，提供搜索和定位类（class）、接口、枚举（enumerations）和注释（annotation）等。

http://www.runoob.com/java/java-package.html



## Java 数据结构
Java工具包提供了强大的数据结构。在Java中的数据结构主要包括以下几种接口和类：

	1. 枚举（Enumeration）
	   枚举（Enumeration）接口虽然它本身不属于数据结构,但它在其他数据结构的范畴里应用很广。
	   http://www.runoob.com/java/java-enumeration-interface.html
	   
	2. 位集合（BitSet）
	   位集合类实现了一组可以单独设置和清除的位或标志。
	   http://www.runoob.com/java/java-bitset-class.html
	   
	3. 向量（Vector）
	   向量（Vector）类和传统数组非常相似，但是Vector的大小能根据需要动态的变化。
	   http://www.runoob.com/java/java-vector-class.html
	   
	4. 栈（Stack）
	   栈（Stack）实现了一个后进先出（LIFO）的数据结构。
	   http://www.runoob.com/java/java-stack-class.html
	   
	5. 字典（Dictionary）
	   字典（Dictionary） 类是一个抽象类，它定义了键映射到值的数据结构。
	   Dictionary 类是一个抽象类，用来存储键/值对，作用和Map类相似。
	   http://www.runoob.com/java/java-dictionary-class.html
	   
	6. 哈希表（Hashtable）
	   Hashtable类提供了一种在用户定义键结构的基础上来组织数据的手段。
	   http://www.runoob.com/java/java-hashTable-class.html
	   
	7. 属性（Properties）
           Properties 继承于 Hashtable.Properties 类表示了一个持久的属性集.
	   属性列表中每个键及其对应值都是一个字符串。
	   http://www.runoob.com/java/java-properties-class.html
	   
	   
## Java 集合框架  Collection 
	   
![](http://www.runoob.com/wp-content/uploads/2014/01/2243690-9cd9c896e0d512ed.gif)   

集合框架是一个用来代表和操纵集合的统一架构。所有的集合框架都包含如下内容：

接口：是代表集合的抽象数据类型。例如 Collection、List、Set、Map 等。之所以定义多个接口，是为了以不同的方式操作集合对象

实现（类）：是集合接口的具体实现。从本质上讲，它们是可重复使用的数据结构，例如：ArrayList、LinkedList、HashSet、HashMap。

算法：是实现集合接口的对象里的方法执行的一些有用的计算，例如：搜索和排序。这些算法被称为多态，那是因为相同的方法可以在相似的接口上有着不同的实现。

除了集合，该框架也定义了几个 Map 接口和类。Map 里存储的是键/值对。尽管 Map 不是集合，但是它们完全整合在集合中。

![](http://www.runoob.com/wp-content/uploads/2014/01/java-coll.png)

http://www.runoob.com/java/java-collections.html




## Java 泛型 类似c++的 template  参数化类型 
	泛型的本质是参数化类型，也就是说所操作的数据类型被指定为一个参数。
	http://www.runoob.com/java/java-generics.html
```java
public class GenericMethodTest
{
   // 泛型方法 printArray                         
   public static < E > void printArray( E[] inputArray )
   {
      // 输出数组元素            
         for ( E element : inputArray ){        
            System.out.printf( "%s ", element );
         }
         System.out.println();
    }
 
    public static void main( String args[] )
    {
        // 创建不同类型数组： Integer, Double 和 Character
        Integer[] intArray = { 1, 2, 3, 4, 5 };
        Double[] doubleArray = { 1.1, 2.2, 3.3, 4.4 };
        Character[] charArray = { 'H', 'E', 'L', 'L', 'O' };
 
        System.out.println( "整型数组元素为:" );
        printArray( intArray  ); // 传递一个整型数组
 
        System.out.println( "\n双精度型数组元素为:" );
        printArray( doubleArray ); // 传递一个双精度型数组
 
        System.out.println( "\n字符型数组元素为:" );
        printArray( charArray ); // 传递一个字符型数组
    } 
}

/*
编译以上代码，运行结果如下所示：

整型数组元素为:
1 2 3 4 5 

双精度型数组元素为:
1.1 2.2 3.3 4.4 

字符型数组元素为:
H E L L O 
*/
```
	
# Java 网络编程
	http://www.runoob.com/java/java-networking.html
	
网络编程是指编写运行在多个设备（计算机）的程序，这些设备都通过网络连接起来。

java.net 包中 J2SE 的 API 包含有类和接口，它们提供低层次的通信细节。你可以直接使用这些类和接口，来专注于解决问题，而不用关注通信细节。

java.net 包中提供了两种常见的网络协议的支持：

TCP：TCP 是传输控制协议的缩写，它保障了两个应用程序之间的可靠通信。通常用于互联网协议，被称 TCP / IP。

UDP：UDP 是用户数据报协议的缩写，一个无连接的协议。提供了应用程序之间要发送的数据的数据包。


> Socket 编程

套接字使用TCP提供了两台计算机之间的通信机制。 客户端程序创建一个套接字，并尝试连接服务器的套接字。

当连接建立时，服务器会创建一个 Socket 对象。客户端和服务器现在可以通过对 Socket 对象的写入和读取来进行通信。

java.net.Socket 类代表一个套接字，并且 java.net.ServerSocket 类为服务器程序提供了一种来监听客户端，并与他们建立连接的机制。

以下步骤在两台计算机之间使用套接字建立TCP连接时会出现：

服务器实例化一个 ServerSocket 对象，表示通过服务器上的端口通信。

服务器调用 ServerSocket 类的 accept() 方法，该方法将一直等待，直到客户端连接到服务器上给定的端口。

服务器正在等待时，一个客户端实例化一个 Socket 对象，指定服务器名称和端口号来请求连接。

Socket 类的构造函数试图将客户端连接到指定的服务器和端口号。如果通信被建立，则在客户端创建一个 Socket 对象能够与服务器进行通信。

在服务器端，accept() 方法返回服务器上一个新的 socket 引用，该 socket 连接到客户端的 socket。


> Socket 客户端实例

如下的 GreetingClient 是一个客户端程序，该程序通过 socket 连接到服务器并发送一个请求，然后等待一个响应。

GreetingClient.java 文件代码：

```java
// 文件名 GreetingClient.java
 
import java.net.*;
import java.io.*;
 
public class GreetingClient
{
   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      try
      {
         System.out.println("连接到主机：" + serverName + " ，端口号：" + port);
         Socket client = new Socket(serverName, port);
         System.out.println("远程主机地址：" + client.getRemoteSocketAddress());
         OutputStream outToServer = client.getOutputStream();
         DataOutputStream out = new DataOutputStream(outToServer);
 
         out.writeUTF("Hello from " + client.getLocalSocketAddress());
         InputStream inFromServer = client.getInputStream();
         DataInputStream in = new DataInputStream(inFromServer);
         System.out.println("服务器响应： " + in.readUTF());
         client.close();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}
```


> Socket 服务端实例

如下的GreetingServer 程序是一个服务器端应用程序，使用 Socket 来监听一个指定的端口。

GreetingServer.java 文件代码：
```java
// 文件名 GreetingServer.java
 
import java.net.*;
import java.io.*;
 
public class GreetingServer extends Thread
{
   private ServerSocket serverSocket;
   
   public GreetingServer(int port) throws IOException
   {
      serverSocket = new ServerSocket(port);
      serverSocket.setSoTimeout(10000);
   }
 
   public void run()
   {
      while(true)
      {
         try
         {
            System.out.println("等待远程连接，端口号为：" + serverSocket.getLocalPort() + "...");
            Socket server = serverSocket.accept();
            System.out.println("远程主机地址：" + server.getRemoteSocketAddress());
            DataInputStream in = new DataInputStream(server.getInputStream());
            System.out.println(in.readUTF());
            DataOutputStream out = new DataOutputStream(server.getOutputStream());
            out.writeUTF("谢谢连接我：" + server.getLocalSocketAddress() + "\nGoodbye!");
            server.close();
         }catch(SocketTimeoutException s)
         {
            System.out.println("Socket timed out!");
            break;
         }catch(IOException e)
         {
            e.printStackTrace();
            break;
         }
      }
   }
   public static void main(String [] args)
   {
      int port = Integer.parseInt(args[0]);
      try
      {
         Thread t = new GreetingServer(port);
         t.run();
      }catch(IOException e)
      {
         e.printStackTrace();
      }
   }
}
```
编译以上两个 java 文件代码，并执行以下命令来启动服务，使用端口号为 6066：

	$ javac GreetingServer.java 
	$ java GreetingServer 6066
	等待远程连接，端口号为：6066...
	新开一个命令窗口，执行以上命令来开启客户端：

	$ javac GreetingClient.java 
	$ java GreetingClient localhost 6066
	连接到主机：localhost ，端口号：6066
	远程主机地址：localhost/127.0.0.1:6066
	服务器响应： 谢谢连接我：/127.0.0.1:6066
	Goodbye!

## Java 发送邮件
	http://www.runoob.com/java/java-sending-email.html
	
## Java 多线程编程
	http://www.runoob.com/java/java-multithreading.html
	
Java 给多线程编程提供了内置的支持。 一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。

多线程是多任务的一种特别的形式，但多线程使用了更小的资源开销。

这里定义和线程相关的另一个术语 - 进程：一个进程包括由操作系统分配的内存空间，包含一个或多个线程。一个线程不能独立的存在，它必须是进程的一部分。一个进程一直运行，直到所有的非守护线程都结束运行后才能结束。

多线程能满足程序员编写高效率的程序来达到充分利用 CPU 的目的。


> 一个线程的生命周期

线程是一个动态执行的过程，它也有一个从产生到死亡的过程。

![](http://www.runoob.com/wp-content/uploads/2014/01/java-thread.jpg)






