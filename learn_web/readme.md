# Web开发

# 前端攻城狮的自我修养 – 零基础入门学习Web开发
[鱼C](http://blog.fishc.com/category/web)

[Java 知识结构](https://github.com/Ewenwan/lifelong-learning/blob/master/knowledge-structure/cs/java.md)

[Java 学习笔记 ](https://www.cnblogs.com/xiaoYu3328/p/5224724.html)

[Java资源大全中文版，包括开发库、开发工具、网站、博客、微信、微博等](https://github.com/biezhi/awesome-java-cn#awesome-java-cn)

[【Java学习+面试指南】 一份涵盖大部分Java程序员所需要掌握的核心知识](https://github.com/Snailclimb/JavaGuide)

[Java 编程技巧 lambda 中如何处理异常？
从开源框架聊聊责任链模式的应用
不做 BUG 仔，必须 GET 的 IDEA 调试技巧
异常处理最佳实践](https://github.com/biezhi/java-tips)

[《跟上 Java 8》视频课程源码](https://github.com/biezhi/learn-java8)

[Java 8 新特性](http://www.runoob.com/java/java8-new-features.html)

[Java 9 新特性](http://www.runoob.com/java/java9-new-features.html)

# js 书籍
[Javascript高级程序设计(第3版)](https://github.com/devphc/Javascript-Book)
  
    与网页开发有关的所有问题基本上都涉及了，写得很清楚明白，相当实用。

JavaScript语言精粹

    链接: https://pan.baidu.com/s/1boP8EFzAyHF9gOKKmIu1-Q 提取码: dcw9 复制这段内容后打开百度网盘手机App，操作更方便哦

[JavaScript权威指南(第6版)中文版](https://github.com/Ewenwan/JavaScriptNote)

    犀牛书/『JavaScript第一书』，基本上所有人都会推荐它。特点是权威、全面、详尽，可以当手册用。
    
[JavaScript DOM编程艺术 （第2版）](https://github.com/Ewenwan/notes-for-javascript-dom-2nd)
  
    循序渐进，适合初学者，非常容易上手，系统的介绍了如何使用JavaScript、HTML及CSS创建可用的Web页面。
    学习JavaScript从HTML、DOM开始，是JavaScript设计的初衷。


# Java三大平台
    Java SE、Java EE、Java ME
    
## 1. Java SE  （J2SE）（Java2 Platform Standard Edition，java平台标准版）
     
       可以分为四个主要的部分：JVM、JRE、JDK和Java语言。
       为了能够运行Java撰写好的程序，必须有Java虚拟机（Java Virtual Machine，JVM）。
       JVM包括在Java执行环境（Java SE Runtime Environment，JRE）中，
       所以为了要运行Java程序，必须安装JRE。
       如果要开发Java程序，必须取得JDK（Java SE Development Kits），
       JDK包括JRE及开发过程中需要的一些工具程序，像是javac、java、appletviewer等工具程序。
       
       javap 是一个Java类文件反汇编程序，可以查看Java编译器生成的字节码，是分析代码的一个好工具。
       jvisualvm 是一个Java虚拟机监控和分析工具，
           该工具提供了一个图形界面窗口，并且可以直观的了解Java应用程序的运行时信息。
       jcmd主要用来把诊断命令请求发送到Java JVM中，当JVM进程中没有jcmd参数列表时，jcmd就会立即运行。
       jhat的全称是Java heap analysis tool。它主要是用来解析和浏览堆文件，jhat有时更像是一个可视化工具。
       
       JVM会架构在Windows、Linux、Mac OS等各种操作系统平台上，从而让Java实现跨平台。
       对Java程序而言，只认识一种操作系统，这个系统叫JVM，JVM就负责了Java程序的各种资源管理。
       
       在 Java 中，JVM可以理解的代码就叫做字节码（即扩展名为 .class 的文件），
       它不面向任何特定的处理器，只面向虚拟机。
       Java 语言通过字节码的方式，在一定程度上解决了传统解释型语言执行效率低的问题，
       同时又保留了解释型语言可移植的特点。
       所以 Java 程序运行时比较高效，而且，由于字节码并不专对一种特定的机器，
       因此，Java程序无须重新编译便可在多种不同的计算机上运行。
       
       JDK包括了Java程序语言、工具程序与JRE，JRE则包括了部署技术、JavaSE API与JVM。
       更简单点，要撰写Java程序就需要JDK，如果只是想运行，那么只要安装JRE就好了。
       （当然现在JDK安装包都是包括了JRE的）
       
       Java 程序从源代码到运行一般有下面3步：
![](https://camo.githubusercontent.com/8f6eceddf64b5948c69a398d1a0e777c9c7f8e5b/68747470733a2f2f6d792d626c6f672d746f2d7573652e6f73732d636e2d6265696a696e672e616c6979756e63732e636f6d2f4a6176612532302545372541382538422545352542412538462545382542462539302545382541312538432545382542462538372545372541382538422e706e67)


##  2. Java EE (J2EE)(Java 2 Platform,Enterprise Edition，java平台企业版)
     
       以Java SE为基础，定义了一系列的服务、API、协议等，
       适用于开发分布式、多层次、已组建为基础、以Web为基础的应用程序，整个Java EE的体系是相当庞大的，
       比较为人熟悉的技术像是JSP、Servlet、JavaMail、Enterprise JavaBeans（EJB）等。
       必须在JavaSE上奠定良好的基础，再来学习Java EE的开发。  
     
##   3. Java ME (J2ME)(Java 2 Platform Micro Edition，java平台微型版)。

       是Java平台上版本中最小的一个，目的是作为小型数字设备上开发及部署应用程序的平台，
       像是消费型电子产品或嵌入式系统等，最为人熟悉的设备如手机、PDA、股票机等，
       可以使用Java ME来开发这些设备上的应用程序，如Java游戏、记事程序等。
       
       Android的优势在于开发性和开源，是一个优秀的移动设备操作系统。 
       Java Me 是一个移动设备软件开发包，跨平台是其主要特点。
       
       Android应用程序不能在JavaME环境下运行，JavaME程序也不能直接在Android里运行，因为它们都有自己的API。
       
     
# 建议的学习路径
    1. 深入了解JVM、JRE、JDK
    2. 理解封装、继承、多态
    3. 掌握常用JavaSE API架构
    4. 学习容器概念
    5. 研究开放源码项目
    6. 学习设计模式与重构
    7. 熟悉相关开发工具（推荐Eclipse和IntelliJ IDEA,网上均有下载资源）
   
    Java 1.6 在线API（http://www.javaweb.cc/help/JavaAPI1.6/）经常查阅API是非常好的习惯
    
    ImportNew -专注Java & Android技术分享（http://www.importnew.com/）很好的Java文章咨询网站
    
    菜鸟教程--学的不仅是技术，更是梦想！（http://www.runoob.com/）
        提供诸多方面的基础知识，包括Java以及JavaWeb、数据库等。
    在线运行工具 http://www.runoob.com/try/runcode.php?filename=HelloWorld&type=java
        
    
    极客学院(http://www.jikexueyuan.com/)，丰富的视频教学资源，需要办理会员才能观看所有视频资源。
    
    

# js语法 Javascript 语法

## 准备工作
- 方法1：将 Javascript 代码放到文档`<head>`标签中`<script>`的标签之间
- 方法2：将 Javascript 代码存为一个扩展名为`.js`的独立文件，并把`<script>`的`src属性`指向该文件。
- 把`<script>`标签放到HTML文档的最后，`</body>`标签之前，这样能使浏览器更快地加载页面。

## 语法

### 语句 Statement
- 如果把多条语句放在同一行，必须用**分号;** 来分隔他们。`first statement; second statement;`
- 建议在每一条语句的末尾都加上分号，这是一种良好的编程习惯。

> 好习惯：在每条语句的末尾都加上分号

### 注释 Comment
- 单行注释：`// 自我提醒：有注释是好事`
- 多行注释：

```
/* 自我提醒：
   有注释是好事 */
```

- 也可以使用HTML风格的注释：`<!-- 这是HTML中的注释`, 这种用法等同于同单行注释，建议不要使用

### 变量 Variable

> 好习惯：提前声明变量是一种良好的编程习惯

- 变量声明 declare：`var mood;` ，也可以一条语句声明多个变量： `var mood, age;`
- 声明和赋值一次完成：`var mood = "happy";`
- 一次声明并赋值多个变量：`var mood = "happy", age = 33;`
- 变量和其他语法元素是 **区分大小写** 的
- 变量名中不允许包含空格或者标点符号（$除外）
- 变量名允许包含：字母、数字、$、下划线，但第一个字符不允许是数字
- 首选驼峰格式（camel case）：myMood

### 数据类型

- Javasript不需要进行类型声明，他是一种弱类型（weakly typed）语言，可以在任何阶段改变变量的数据类型
- 字符串：字符串必须包含在引号里，单引号或双引号都可以。转义字符（escaping），`\n`

> 好习惯：不管选择双引号还是单引号，请在整个脚本中保持一致

- 数值：不用限定他必须是一个整数，可以是浮点数、负数、负数浮点数
- 布尔值：true、false

### 数组 Array
- 数组 Array：变量的集合：
  - `var beatles = Array()`
  - `var beatles = Array(4)`
  - `var beatles = Array("John", "Paul", "George", "Ringo");`
  - `var beatles = [ "John", "Paul", "George", "Ringo" ]`
  - 添加元素（填充populating）：`array[index] = element;`
  - 可以把不同数据类型混在一起存入一个数组中
  - 数组中的元素element也可以是一个数组
- 关联数组：下标不是数字，`lenno["name"] = "John"`，不推荐使用

### 对象 Object
- 对象的每一个值都是对象的属性；
- 创建对方及访问属性：

```js
var lenno = Object();
lenno.name = "John";
lenno.year = 1940;
lenno.living = false;
```

- 简易方法：`var lennon = { name:"John", year:1940, living:false };`

## 操作
### 算数操作符
- +、-、*、/、++、--
- `var total = (1+4)*5;`
- 拼接（concatenation): 把数字和字符串拼接在一起，其结果将是一个更长的字符串；
- `+=`

### 比较操作符
- < 、> 、>= 、<= 、== 、=== (不仅比较值，还比较变量类型）、!= 、!==

### 逻辑操作符
- && 、|| 、！

## 条件语句 Condition Statment
- if 条件语句的语法

```js
if (condition) {
  statements;
} else {
  statements;
}
```

- if 语句中的花括号本身并不是必不可少的，如果if 语句中花括号部分只包含着一条语句的话，可以不适用花括号：`if (1 > 2) alert("The world has gone mad!");`

> 好习惯：在if语句中总是使用花括号是个好习惯

## 循环语句

### while 循环
只要给定条件的求值结果是true，包含在花括号里的代码就将**反复地**执行下去。
while循环的内部**必须**发生一些会影响循环控制条件的事情。
语法如下：
```javascript
while (condition) {
  statements;
}
```

### do...while 循环

对循环控制条件的求值发生在每次循环结束之后，因此循环语句内的代码**至少执行一次**
语法如下：
```javascript
do {
  statements;
} while (condition);
```

### for 循环
for循环结构清晰，与循环有关的所有内容都包含在for语句的圆括号部分。
语法如下：
```javascript
for (initial condition; test condition; alter condition) {
  statements;
}
```

## 函数 function
函数（function）就是一组允许在代码里随时调用的语句。

> 好习惯：应该先对函数做出定义再调用他们。

语法如下：
```javascript
function name(arguments) {
  statements;
}
```
例子1：
```javascript
function multiply(num1, num2) {
  var total = num1 * num2;
  return total;
}
```

例子2:
```javascript
function convertToCelsius(temp) {
  var result = temp - 32;
  result = result / 1.8;
  return result;
}
```

> 函数的真正价值自在于我们可以把他们当做一种数据类型来使用，可以把一个函数的调用结果赋给一个变量；
例如：
```javascript
var temp_fahrenheit = 95;
var temp_celsius = convertToCelsius(temp_fahrenheit);
```

### 变量的作用域 scope

- 全局变量（global variable）：可以在脚本中的任何位置被引用，包括函数内，作用域为整个脚本
- 局部变量（local variable）：作用于声明他的那个函数内部，在那个函数的外部无法引用他。如果在函数中使用了`var`那么这个变量将被视为一个局部变量。反之如果没有使用`var`，那么这个变量将被视为一个全局变量，如果脚本里已经存在一个与之同名的全局变量，这个函数就会改变那个全局变量的值。
  
## 对象 Object
对象是自包含数据集合，包含在对象中的数据可以通过两种形式访问：
- 属性 property：隶属于某个特定对象的变量；
- 方法 method：只有某个特定对象才能调用的函数；
- 他们都用 `.` 的语法来访问：`Object.property`, `Object.method()`；

### 创建实例（instance）
- 包含属性和方法：`var jenny = new Person;`
- `jenny.age`
- `jenny.mood`

### 内建对象：
- Math(), Array(), Date()
- `var beatles = new Array();`
- `beatles.length`
- `var num = Math.round(7.561);`
- `var today = current_date.getDay();`

### 宿主对象
- 这些对象是有他的运行环境提供的，具体到web应用，这个环境就是浏览器。由浏览器提供的预定义对象被称为宿主对象（host object）；
- 包括：Form, Image, Element, document等

## 小结
- Javascript语言的基础知识：语句、变量、数组、函数等
- 对象的概念
- 作者[第一版的附录](http://www.ituring.com.cn/book/42)
