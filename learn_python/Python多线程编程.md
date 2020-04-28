# Python多线程编程

[Python多线程编程 ](https://www.ctolib.com/docs-Python-Multithreading-c-index.html)

[详解multiprocessing多进程](https://blog.csdn.net/brucewong0516/article/details/85788202)

[subprocess以及常用的封装函数](https://www.cnblogs.com/lincappu/p/8270709.html)

运行python的时候，我们都是在创建并运行一个进程。像Linux进程那样，一个进程可以fork一个子进程，并让这个子进程exec另外一个程序。在Python中，我们通过标准库中的subprocess包来fork一个子进程，并运行一个外部的程序。
subprocess包中定义有数个创建子进程的函数，这些函数分别以不同的方式创建子进程，所以我们可以根据需要来从中选取一个使用。另外subprocess还提供了一些管理标准流(standard stream)和管道(pipe)的工具，从而在进程间使用文本通信。
