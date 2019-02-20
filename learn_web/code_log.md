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
