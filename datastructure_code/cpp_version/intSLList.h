//************************  intSLList.h  **************************
// 单链表 实现类 

#ifndef INT_LINKED_LIST
#define INT_LINKED_LIST

// 单个节点
// 定义一个 节点指针
// IntLLNode* pt = new IntLLNode(10);//新建一个节点 其地址 赋给一个指向 IntLLNode 的指针 pt
//
//   [pt]  ----> |  10 | 也就是 pt->info_   也就是 (*pt).info_
//               |  \  | 也就是 pt->nextPtr_ 也就是 (*pt).nextPtr_  
class IntSLLNode {
public:
    IntSLLNode() {// 默认构造函数   没有info信息
        next = 0;// 空指针
    }
    IntSLLNode(int data, IntSLLNode *ptr = 0) {// 第二个构造函数
        info = data; 
		next = ptr;
    }
public:    
    int info;         // 包含一个信息          对用户很重要
    IntSLLNode *next; // 指向下一个 节点的指针  用于将节点连接起来组成链表
};

// 链表 类  保存了一个 头节点 和 一个尾节点 
class IntSLList {
public:
    IntSLList() {// 默认构造函数  这里定义和实现 
        head = tail = 0;// 节点 和 一个尾节点 指针赋值为 空 
    }
    ~IntSLList();// 默认析构函数  只有定义 实现在 cpp文件中 
    int isEmpty() {//为空链表是否 
        return head == 0;
    }
    void addToHead(int);   // 从头部添加 节点 
    void addToTail(int);   // 从尾部添加 节点 
    int  deleteFromHead(); // 从头部删除 节点 并返回该节点的信息 
    int  deleteFromTail(); // 从尾部删除 节点 并返回该节点的信息 
    void deleteNode(int);//删除 
    bool isInList(int) const;
    void printAll() const;//打印所有的节点的信息 
    
    
private:
    IntSLLNode *head, *tail;
};

#endif
