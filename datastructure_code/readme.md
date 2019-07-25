# 数据结构 learn  datastructure

[数据结构思维 中文翻译](https://github.com/Kivy-CN/think-dast-zh)

[大话数据结构](https://github.com/Ewenwan/Big-Data-Structure)

[数据结构与算法系列 目录 博客整理 强烈推荐](http://www.cnblogs.com/skywang12345/p/3603935.html)


[数据结构-算法基础](http://www.xuetangx.com/courses/course-v1:TsinghuaX+30240184_p1+sp/about)

[数据结构-向量](http://www.xuetangx.com/courses/course-v1:TsinghuaX+30240184_p2+sp/about)

[清华视频教程 数据结构(上)](http://www.xuetangx.com/courses/course-v1:TsinghuaX+30240184+sp/about)

[清华视频教程 数据结构(下)](http://www.xuetangx.com/courses/course-v1:TsinghuaX+30240184_2X+sp/about)

[计算几何](http://www.xuetangx.com/courses/course-v1:TsinghuaX+70240183x_1+sp/about)

[C++语言程序设计基础](http://www.xuetangx.com/courses/course-v1:TsinghuaX+00740043_1x+2018_T1/about)

[C++语言程序设计进阶](http://www.xuetangx.com/courses/course-v1:TsinghuaX+00740043_2x_2015_T2+sp/about)

[面向对象程序设计（C++）（2018春）](http://www.xuetangx.com/courses/course-v1:TsinghuaX+30240532X+2018_T1/about)

[基于Linux的C++](http://www.xuetangx.com/courses/course-v1:TsinghuaX+20740084X+sp/about)

[汇编语言程序设计](http://www.xuetangx.com/courses/course-v1:TsinghuaX+20240103X+sp/about)

[鱼C 数据结构和算法](http://blog.fishc.com/653.html)

## 向量 vector 基于数组实现 自动调整大小

### 0_类结构_用户-接口-类实现
![vector接口](Tsinghua/vector/picture/0_类结构_用户-接口-类实现.png)

### 01_复制构造
![vector接口](Tsinghua/vector/picture/01_复制构造.png)

### 1_扩容_动态内存管理_金蝉脱壳
![vector接口](Tsinghua/vector/picture/1_扩容_动态内存管理_金蝉脱壳.png)

### 12_扩容_金蝉脱壳_实现
![vector接口](Tsinghua/vector/picture/12_扩容_金蝉脱壳_实现.png)

### 13_扩容_容量递增策略复杂度
![vector接口](Tsinghua/vector/picture/13_扩容_容量递增策略复杂度.png)

### 14容量加倍策略扩容时间复杂度
![vector接口](Tsinghua/vector/picture/14容量加倍策略扩容时间复杂度.png)

### 15递增扩容和容量加倍策略对比
![vector接口](Tsinghua/vector/picture/15递增扩容和容量加倍策略对比.png)

### 20向量下标操作符重载访问
![vector接口](Tsinghua/vector/picture/20向量下标操作符重载访问.png)

### 30插入操作
![vector接口](Tsinghua/vector/picture/30插入操作.png)

### 31区间删除操作
![vector接口](Tsinghua/vector/picture/31删除操作.png)

### 32单元素删除操作 调用区间删除操作[r,r+1)
![vector接口](Tsinghua/vector/picture/32单个删除操作.png)

### 33查找元素操作 从后向前依次查找 判等 或大于小于
![](Tsinghua/vector/picture/33查找元素操作.png)

### 34唯一化操作 在前驱中查找相同 在后驱中删除重复出现的元素 类似插入排序思想 可以先排序提高效率
![](Tsinghua/vector/picture/34唯一化操作.png)

### 35唯一化操作 算法正确性证明
![](Tsinghua/vector/picture/35唯一化操作证明.png)

### 36遍历向量元素 传递一个函数对象
![](Tsinghua/vector/picture/36遍历向量元素-传递一个函数对象.png)

### 37遍历向量元素 传递一个函数对象
![](Tsinghua/vector/picture/36遍历向量对对象+1.png)

### 40有序向量唯一化
![](Tsinghua/vector/picture/40有序向量唯一化.PNG)

### 41有序向量二分查找
![](Tsinghua/vector/picture/41有序向量二分查找.PNG)

### 42_3个分支 有序向量二分查找 时间复杂度 分析
![](Tsinghua/vector/picture/43.3次比较二分查找时间复杂度.PNG)

### 43 黄金比例分割 0.618 查找 Fibonacci查找
![](Tsinghua/vector/picture/44.黄金比例分割查找.PNG)

### 44 黄金比例分割查找算法实现
![](Tsinghua/vector/picture/45.黄金比例分割查找算法实现_还是3个分支.PNG)

### 45 黄金比例分割查找算法时间复杂度分析
![](Tsinghua/vector/picture/46.黄金比例分割查找算法时间复杂度分析.PNG)

### 46 二分查找_版本b，两个分支，包含中心点
![](Tsinghua/vector/picture/47.二分查找_版本b，两个分支，包含中心点.PNG)

### 47 二分查找_版本c，两个分支，不包含中心点
![](Tsinghua/vector/picture/48.二分查找_版本c，两个分支，bu包含中心点.PNG)

### 48 线性分布数据的 线性插值查找算法
![](Tsinghua/vector/picture/49.线性分布数据的线性插值查找算法.PNG)

### 49 线性分布数据的线性插值查找实例
![](Tsinghua/vector/picture/50.线性分布数据的线性插值查找实例.PNG)

### 50 线性分布数据的线性插值查找时间复杂度分析
![](Tsinghua/vector/picture/51.线性分布数据的线性插值查找时间复杂度分析.PNG)

### 51 线性插值查找_二进制位宽_二分log(log(n))
![](Tsinghua/vector/picture/52线性插值查找_二进制位宽_二分log(log(n)).PNG)

### 52 无序变有序_冒泡起泡排序
![](Tsinghua/vector/picture/55.无序变有序_冒泡起泡排序.PNG)

### 53 无序变有序_冒泡起泡排序算法实现
![](Tsinghua/vector/picture/56.无序变有序_冒泡起泡排序算法实现.PNG)

### 54 无序变有序_冒泡起泡排序算法实现
![](Tsinghua/vector/picture/57.无序变有序_冒泡起泡排序算法实现.PNG)

### 57 冒泡起泡排序改进跳过有序段
![](Tsinghua/vector/picture/58.冒泡起泡排序改进跳过有序段.PNG)

### 60 归并排序_图解 分解到最小单位 一次两两排序后合并 
![](Tsinghua/vector/picture/60.归并排序_图解.PNG)

### 61 归并排序 主程序 分 分 合 合
![](Tsinghua/vector/picture/60.归并排序_序列合并图解.PNG)

### 62 2有序数组合并图解 一次比较最小的 最小的放入 总体有序数组内
![](Tsinghua/vector/picture/2有序数组合并图解.PNG)

### 63 归并排序_两个有序数组合并 算法实现
![](Tsinghua/vector/picture/60.归并排序_量有序数组合并.PNG)

### 64 归并排序_2有序数组合并_当其中一个为空的情况， 另外一个直接汇入总体数组
![](Tsinghua/vector/picture/61.归并排序_2有序数组合并_一个为空的情况.PNG)

### 65 两有序序列合并精简版 其中处于总体数组中的有序数组可以省去复制
![](Tsinghua/vector/picture/60.两有序序列合并精简版.bmp)

### 66 归并排序_时间复杂度分析
![](Tsinghua/vector/picture/62.归并排序_时间复杂度分析.PNG)
