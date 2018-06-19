# 双向循环链表
    在“数据结构”课程中，
    如果创建某种数据结构的双循环链表，
    通常采用的办法是在这个数据结构的类型定义中有专门的成员变量 data, 
    并且加入两个指向该类型的指针next和prev。
    例如：
```C
typedef struct foo {
    ElemType data;／//节点存储的数据
    struct foo *prev;//指向前一个节点的　节点类型指针
    struct foo *next;//指向后一个节点的节点类型指针
} foo_t;
// typedef 使得可以使用 foo_t 代替　struct foo来使用
```
# 双向循环链表的特点
    双向循环链表的特点是尾节点的后继指向首节点，
    且从任意一个节点出发，沿两个方向的任何一个，
    都能找到链表中的任意一个节点的data数据。
    由双向循环列表形成的数据链如下所示：
    
![](https://chyyuu.gitbooks.io/ucore_os_docs/content/lab0_figs/image007.png)

