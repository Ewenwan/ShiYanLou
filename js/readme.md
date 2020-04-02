# JavaScript
[前端技能汇总 Frontend Knowledge Structure](https://github.com/Ewenwan/fks)

[深入浅出Node.js 书籍pdf ](https://github.com/zxysilent/books/blob/master/%E6%B7%B1%E5%85%A5%E6%B5%85%E5%87%BANode.js.pdf)
[《深入浅出Node.js》的相关代码](https://github.com/JacksonTian/diveintonode_examples)


[Javascript&jQuery教程 pdf](https://github.com/zxysilent/books/blob/master/Javascript%26jQuery%E6%95%99%E7%A8%8B_20141027.pdf)

[html & css教程 pdf](https://github.com/zxysilent/books/blob/master/html%20%26%20css%E6%95%99%E7%A8%8B_20141002.pdf)


[高性能JavaScript_中英双语版 pdf](https://github.com/zxysilent/books/blob/master/%E9%AB%98%E6%80%A7%E8%83%BDJavaScript_%E4%B8%AD%E8%8B%B1%E5%8F%8C%E8%AF%AD%E7%89%88.pdf)


[跳坑之js调用另一个js文件中函数](https://www.jianshu.com/p/7dfd612693bc)

方法1;

在html文件中加入两个脚本程序，注意，加入的位置在</head>和<body>两个标签之间，（也有的在<body></body>两个标签之间加入的），代码如下：
```js
</head>
  <script src="hello.js"></script>
  <script src="renderer.js"></script>
  <body>
```


之后在hello.js中直接调用函数就行。

test('click', 'asynchronous message', 'ping');

方法2

renderer.js中使用exports导出函数：

```js
//在这里面写好函数的封装，然后在hello.js中调用
var test = function(struct, buttonId, msg){
    const asyncMsgBtn = document.getElementById(buttonId);
    asyncMsgBtn.addEventListener(struct, function(){
        switch(struct){
            case 'click':
                ipc.send('asynchronous-message', msg);
                console.log("调用成功");
                break;
            default:
                console.log('Error!!!')
        }
    })
}
//这种方式是成功的
exports.test = test;
//这种方式也是可以得
//module.exports.test = test;
```

而hello.js中对于代码的使用如下：

```js
//利用require加载模块
const renderer = require('./renderer')

renderer.test('click', 'asynchronous message', 'ping');
renderer.test('click', 'changeView', 'change');
```

可以说，这种方式完全符合我们程序封装的概念，思路统一，结构规整，个人最爱。
[]()



