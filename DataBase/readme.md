# 数据库

[PyMongo模块的使用-MongoDB的Python接口](http://blog.topspeedsnail.com/archives/10790)

MapReduce将数据分解成子集，在不同机器上分开处理，并把结果集合起来，从而处理大数据的泛化框架。

Hadoop是MapReduce的一种实现，类似于C++是面向对象编程的实现一样。

NoSQL-Not Only SQL，技能能更新颖，更高效地访问(如MapReduce)数据的数据库或数据库管理系统

Spark 由一个驱动程序构成，它运行用户的 main 函数并在聚类上执行多个并行操作。Spark 最吸引人的地方在于它提供的弹性分布数据集（RDD），那是一个按照聚类的节点进行分区的元素的集合，它可以在并行计算中使用。RDDs 可以从一个 Hadoop 文件系统中的文件（或者其他的 Hadoop 支持的文件系统的文件）来创建，或者是驱动程序中其他的已经存在的标量数据集合，把它进行变换。用户也许想要 Spark 在内存中永久保存 RDD，来通过并行操作有效地对 RDD 进行复用。最终，RDDs 无法从节点中自动复原。

* 离线数据处理: hadoop / hbase / cassandra
* 在线数据处理：storm / s4
* 跨语言通信：thrift / proobuf / avro, elephant bird
* 一致性：zoo keeper / chubby
* 数据查看：hive / pig
* 数据传输：scribe

## 大数据 

    按照马云的说法，现在已经从IT时代进入了DT时代，这个地方的DT强调的就是大数据。
    目前大数据的应用非常广泛，从资讯分发到视频娱乐，大数据无处不在。
    目前大数据已经成为了热门投资方向之一，在资本的大力支持下，
    大数据产业快速发展，预计到2020年中国大数据产业规模或超过万亿。
    在重视数据的今天，大数据行业必定会成为2019年的创业风口。

[Data_analysis 数据分析处理](https://github.com/Choven-Meng/Data_analysis)

[用户行为定向](https://github.com/Shanshan-IC/ComputationalAdvertising/blob/master/3-shou-zhong-ding-xiang.md)

[computational advertising reading list (计算广告阅读）](https://github.com/Ewenwan/ad-reading)

[Big Data 大数据/数据挖掘/推荐系统/机器学习相关资源 ](https://github.com/Ewenwan/Big-Data-Resources/blob/master/README.md)

![](https://github.com/SsLen/blog-backup/raw/5dab2851c5ec8da8d733d5e6c20776f9935ecc7b/source/_posts/%E5%A6%82%E4%BD%95%E6%88%90%E4%B8%BA%E6%95%B0%E6%8D%AE%E7%A7%91%E5%AD%A6%E5%AE%B6live-%E8%AE%B0%E5%BD%95/4.jpg)

* 收集 -> 加工 -> 变现 -> 交易
* 收集：不同行业不同的收集方式，传感器自动收集
* 加工：用户画像，将原始数据转化成业务所需要的数据
* 变现：已有成熟的互联网解决方案，流量和数据变现

> 现在大数据--5V特征

* 速度V(velocity)(实时，快速流动)、
* 多样化V(variety)(结构化，非结构化如微博内容)、
* 大数据量(volume)、
* 真实性(veracity)、
* 价值(value)

> 数据科学家=数据+科学+艺术家=用数据和科学从事艺术创作的人

> 一些数据科学应用的领域

* 分析NASA的照片，寻找新的行星和小行星 -- 图像处理?
* 自动报价系统 ?
* 自动驾驶
* Amazon上或FB推荐书，朋友 -- 推荐系统
* 针对所有酒店客房的客户定制(实时)定价系统 ???
* 流行病的早期检测
* (实时)估计美国所有房屋价值(Zillow.com)
* 高频交易
* 税收欺诈监测，恐怖行为检测
* 信用卡交易评分(欺诈检测)
* 计算广告 [计算广告-在线广告市场与背景](https://github.com/Nianchese/bloop_blog_article/blob/e20332ec9ff19539cc1da78471d03947718daf35/ComputationalAdvertising1.md) -- [计算广告-在线广告商品逻辑](https://github.com/Nianchese/bloop_blog_article/blob/e20332ec9ff19539cc1da78471d03947718daf35/ComputationalAdvertising2.md)-- [重读计算广告](https://github.com/SummerBuger/mblog-source/blob/5919575ae37711ac429a99da1ce44614af1c70d1/source/_posts/%E8%AE%A1%E7%AE%97%E5%B9%BF%E5%91%8A/2018-05-14-%E9%87%8D%E8%AF%BB%E8%AE%A1%E7%AE%97%E5%B9%BF%E5%91%8A.md)--[计算广告笔记](https://github.com/csuncs89/learn-machine-learning/tree/8ceb2467cb2a2def0adb047fa17e4b3c8f5a123e/misc/computational_advertising)



> 必备技能
* 1、 机器学习，统计学技能是基础
* 2、 领域知识
* 3、 分布式计算
> 核心能力：
* 1、 统计学基本准则
* 2、 机器学习建模能力
* 3、 模型最优化
* 4、 大数据工具、编程能力
* 5、 领域知识学习能力

![](https://camo.githubusercontent.com/380896fc351b85f6ec63fae8fc53f06df5ee3705/687474703a2f2f736369656e74697374636166652e636f6d2f626f6f6b2f4669677572652f536b696c6c2e706e67)



