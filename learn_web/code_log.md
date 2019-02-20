# java代码日记

[在线运行](http://www.runoob.com/try/runcode.php?filename=HelloWorld&type=java)

[本地运行环境配置](http://www.runoob.com/java/java-environment-setup.html)

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
示例

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
