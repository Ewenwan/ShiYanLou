# 学习 GO语言
[参考](https://blog.csdn.net/column/details/18320.html?&page=2)

# 介绍
    Go语言是Google开发的一种静态强类型、编译型、并发型，并具有垃圾回收功能的编程语言。为了方便搜索和识别，有时会将其称为Golang。

    Go语言于2009年11月正式宣布推出，成为开放源代码项目，并在Linux及Mac OS X平台上进行了实现，后来追加了Windows系统下的实现。

# 特点
    Go语言是一门强类型的通用编程语言。它的基础语法与C语言很类似，但对于变量的声明有所不同，也对其他的一些优秀编程语言有所借鉴。

    另外，Go语言支持垃圾回收。

    与C++相比，Go语言并不包括如异常处理、继承、泛型、断言、虚函数等功能，

    但增加了 Slice 型、并发、管道、垃圾回收、接口（Interface）等特性的语言级支持。

    Go语言对并发编程的支持是天生的、自然的和高效的。Go语言为此专门创造出了一个关键字“go”。

    使用这个关键字，我们就可以很容易的使一个函数被并发的执行。
    
# 撰写风格
    在Go语言中有几项规定，而且这些是强制的，当不匹配以下规定时编译将会产生错误。

    1.每行程序结束后不需要撰写分号; 
    2.大括号{不能够换行放置。 
    3.if 判断式和 for 循环不需要以小括号包覆起来。

代码示例

所有语言的第一个程序都是“hello world”，我们也不例外，在这里用Go语言写一个“hello world”程序。
```GO
package main

import "fmt"

func main(){

    fmt.Printf("Hello World!");

}
```
这段代码运行，会在控制台下打印出“hello world!”。

如果你把fmt.Printf("Hello World!")改为fmt.Println("Hello World!")，在打印“Hello World”后换行。 LINE

还有值得注意的一点是：Golang官方的约定是每行程序结束后不需要撰写分号;，但是如果你不小心加了，编译器也不会报错


# 跨平台特性
Go语言和Java，Python一样，也是支持跨平台的。另外，Go语言还有黑科技：支持跨平台编译。比如你手头只有Mac系统，而你的用户有Linux和Windows的，他们也想使用你的Go语言程序，你可以通过交叉编译出Linux和Windows上的可执行文件给他们。另外，你甚至可以在32位平台的机器上以编译生成64位平台上的可执行程序。

## Windows下安装
    go语言官方下载地址：https://golang.org/dl。 
    然后在这里找到对应的windows安装包，
    一路Next安装即可。
    安装完毕后重启电脑，在cmd控制台下输入：

    go version
    如果打印出：

    go version go1.9.2 windows/amd64
    则证明安装成功！

## Linux下安装
    go语言官方下载地址：https://golang.org/dl。 
    然后在这里找到对应的Linux安装包，
    然后执行以下命令解压：

        mkdir /home/go
        mkdir /home/gopath
        tar -C /home -xzvf go1.9.2.linux-amd64.tar.gz

    接下来修改系统配置文件（环境变量），执行命令：
        vi ~/.bashrc
    加入以下内容：

        export GOROOT=/home/go
        export PATH=$PATH:$GOROOT/bin
        export GOPATH=/home/gopath

    然后执行：
        source ~/.bashrc

    让配置文件生效。

    然后打开Terminal，输入：
        go version

    如果打印出：
        go version go1.9.2 linux/amd64
    则证明安装成功！


## GOROOT和GOPATH区别
    GOROOT是Go语言的安装目录，类似于JAVA中的JAVA_HOME。

    GOPATH是你代码中的引用的包所在的位置，可以看成是工程目录，可以设置多个。

## IDE推荐
    主流的文本编辑器，例如sublime，vscode，atom，vim和emacs等，都有Go语言插件，读者可以自行配置下。

    这里推荐两款IDE，第一款是开源免费的liteide，
    地址：https://www.golangtc.com/download/liteide。 
    这是一个国人开发的 Google Go 语言的一个开发工具，支持单步调试，而且跨平台。

    第二个是Jetbrain在2017年11月发布的Go语言IDE——GoLand，功能方面不弱，但貌似是收费的。

## Golang中国社区
    因为某些原因，Go语言官网在国内是没法直接访问的，用户无法从Go语言官网下载安装文件。
    如果还需要下载一些第三方的依赖包，速度更是慢的不行。
    Golang中国社区的出现就解决了这个问题：
    不仅提供最新版的Go语言安装包和第三方包的下载，还提供liteide的下载！

    地址：https://www.golangtc.com

    这是一个高质量的语言社区，上面有丰富的教程和学习资料，还有各种Go语言问题的讨论。
## 代码示例
接下来我们看一个简单的程序，可以打印出当前运行环境中的Go语言版本号。
```go
package main
import (
    "fmt"
    "runtime"
)
func main() {
    fmt.Println(runtime.Version())
}
```


