# java代码日记

[在线运行](http://www.runoob.com/try/runcode.php?filename=HelloWorld&type=java)

## 基础

```java
public class MyHelloW{ // MyHelloW类 
	public static void main(String []args){ // 类构造函数??
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
