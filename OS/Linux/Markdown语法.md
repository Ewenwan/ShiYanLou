# Markdown基本操作技能

## 1.超链接的使用

在github中，我们使用了markdown文法来书写说明文档。不同于其他项目，weloveinterns仓库中，wiki 和 code 的代码我们人工合并在一起了。

在code中，使用超链接可以采用下面的方式：

```
[fork](images/fork.png)
```

在wiki中，使用超链接可以是以下形式：

```
[[Markdown基本操作技能|markdown-checklist]]
```

其中，“|”后面跟着的“markdown-checklist”就是需要跳转到的文件名。

其他不正确的使用方法：

1.这里如果不是使用文件名，而是文件地址，

如“https://github.com/lazyparser/weloveinterns/blob/master/capabilities/markdown-checklist.md”，

则会跳转到上面这个绝对地址，但是显示的效果不是我们预期的wiki方式。

也就是说，wiki使用的方式：“https://github.com/lazyparser/weloveinterns/wiki/markdown-checklist”

与“https://github.com/lazyparser/weloveinterns/blob/master/capabilities/markdown-checklist.md” 

处理方式是完全不同的。

2.如果我们在wiki中使用下面相对地址形式的超链接：

```
[Markdown基本操作技能](../capabilities/markdown-checklist.md)
```

则实际会跳转到：

https://raw.githubusercontent.com/wiki/lazyparser/weloveinterns/capabilities/markdown-checklist.md


3.如果使用下面相对地址形式的超链接：

```
[[Markdown基本操作技能|capabilities/markdown-checklist]]
```

则实际会跳转到一个新的地址：

“https://github.com/lazyparser/weloveinterns/wiki/capabilities-markdown-checklist”


总结：我们可以理解github在两个方面做了修改和取舍:

1. 像一般的wiki一样,将 '/' 和 ' ' 都作为 '-' 替换掉, 这样URL可以正确

2. 如果push上来的wiki, 本身页面在子目录下, 那也要能够找到啊~ 怎么办呢？于是就干脆压平, 每次搜索的时候, 就把整个项目中的所有的 md 文件都找一遍。

TODO：

那么问题来了,如果 a/xxx.md 和 b/xxx.md 都同时存在, 

```
[[a-or-b|xxx]] 
```

会如何链接呢?


关于wiki的更多介绍，可以参看：https://github.com/gollum/gollum/wiki

以及下图的介绍：

![wiki-page](../images/Link-internal-pages.png)

