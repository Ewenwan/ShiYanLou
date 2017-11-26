//线性表的链式表示
//单链表
// 从头结点开始，指针域依次指向下一个数据的存储地址，尾节点的指针域为NULL
// 还有 循环链表 和 双向链表
/*
线性表的顺序存储结构是逻辑位置和物理位置都相邻，
而链式存储结构是逻辑位置相邻，但物理位置不一定相邻，
相比顺序存储结构，它不能随机存取，
但在插入和删除操作时不需要移动元素，
大大提高了增加和删除元素的效率。

顺序存储结构读取元素的效率比较高，链式存储结构添加和删除元素的效率比较高。
*/ 
#include <stdio.h>  //标准库 输入输出 
#include <stdlib.h> //标准库  

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

//  等价 数据类型 定义 
typedef int Status;  //状态 
typedef int ElemType;//表 元素类型 

/*
 * 存储结构
 */
typedef struct LNode
{
    ElemType data;//元素  这里的元素可以不只一个  类型也可以为 字符串数组等
	              // 例如 char cName[20];/*学生姓名*/  int iNumber;/*学生学号*/
    struct LNode *next;//后继指针 
}LNode, *LinkList;//链表 结构体变量  结构体指针 

/*
 * 初始化线性表
 */
void InitList(LinkList *L)
{
    *L = (LinkList) malloc(sizeof(LNode));// 一个元素 + 一个指针变量  的内存大小 
    if (!L)
    {
        exit(OVERFLOW);
    }
    (*L)->next = NULL;//初始化时  后继指针为NULL 
}

/*
 * 销毁线性表
 */
void DestroyList(LinkList *L)
{
    LinkList temp;
    while (*L)//未到表尾 表尾部为NULL 
    {
        temp = (*L)->next;//保存后继的节点 
        free(*L);//删除当前节点内存 
        *L = temp;//下一个节点 
    }
}

/*
 * 清空线性表
 */
void ClearList(LinkList L)
{
    LinkList p = L->next;//下一个节点 
    L->next = NULL;//后继指针为NULL 
    DestroyList(&p);//清空内存 
}

/*
 * 判断是否为空
 */
Status isEmpty(LinkList L)
{
    if (L->next)
    {
        return FALSE;//后继有内容 不为空 
    }
    else
    {
        return TRUE;//后继无内容  为空 
    }
}

/*
 * 获取长度
 */
int GetLength(LinkList L)
{
    int i = 0;
    LinkList p = L->next;//后继节点 
    while (p)
    {
        i++;//后继节点 计数 
        p = p->next;//依次遍历后继节点 
    }
    return i;
}

/*
 * 根据位置获取元素
 */
Status GetElem(LinkList L, int i, ElemType *e)
{
    int j = 1;
    LinkList p = L->next;//后继节点 
    while (p && j < i)//p 未到尾节点 且j<i 
    {
        j++;//位置计数 
        p = p->next;//下一个后级节点 
    }
    if (!p || j > i)//越界  p到了尾部 
    {
        return ERROR;
    }
    *e = p->data;
    return OK;
}

/*
 * 比较两个元素是否相等
 */
Status compare(ElemType e1, ElemType e2)
{
    if (e1 == e2)
    {
        return 0;
    }
    else if (e1 < e2)
    {
        return -1;//前面的小于后面的 
    }
    else
    {
        return 1;//前面的大于后面的 
    }
}

/*
 * 查找指定元素的位置
 */
int FindElem(LinkList L, ElemType e, Status (*compare)(ElemType, ElemType))
{
    int i = 0;
    LinkList p = L->next;//后继节点 
    while (p)
    {
        i++;//位置计数 +1 
        if (!compare(p->data, e))//比较当前节点元素值和 目标元素值 
        {
            return i;//相等返回 位置计数值 
        }
        p = p->next;//指向后继  
    }
    return 0;
}

/*
 * 获取前驱元素
 */
Status PreElem(LinkList L, ElemType cur_e, ElemType *pre_e)
{
    LinkList q, p = L->next;//后继节点 
    while (p->next)//后继 不为空 
    {
        q = p->next;//p节点的后继为q 
        if (q->data == cur_e)//q节点的元素与目标 元素相等
        {
            *pre_e = p->data;//q的前驱 p的元素为 目标元素的前驱 
            return OK;
        }
        p = q;//p指向下一个后继 
    }
    return ERROR;
}

/*
 * 获取后继元素
 */
Status NextElem(LinkList L, ElemType cur_e, ElemType *next_e)
{
    LinkList p = L->next;//后继 
    while (p->next)//节点后继不为空 
    {
        if (p->data == cur_e)//节点元素与目标元素相等 
        {
            *next_e = p->next->data;//当前节点的后继节点的元素为 目标元素的后继元素 
            return OK;
        }
        p = p->next;//指向下一个后继 
    }
    return ERROR;
}

/*
 * 插入元素
 */
Status InsertElem(LinkList L, int i, ElemType e)
{
    int j = 0;
    LinkList s, p = L;// 
    while (p && j < i - 1)
    {
        j++;//位置计数 +1 
        p = p->next;//指向后继 后移 
    }
    if (!p || j > i - 1)//位置越界 
    {
        return ERROR;
    }
    s = (LinkList) malloc(sizeof(LNode));//新开辟一段内存空间 
    s->data = e;//元素值 
    s->next = p->next;//后继指向p的后继 
    p->next = s;//p的后继指向新的元素节点 
    return OK;
}

/*
 * 删除元素并返回值
 */
Status DeleteElem(LinkList L, int i, ElemType *e)
{
    int j = 0;
    LinkList q, p = L;
    while (p->next && j < i - 1)
    {
        j++;
        p = p->next;//找到 目标位置节点 
    }
    if (!p->next || j > i - 1)
    {
        return ERROR;
    }
    q = p->next;//保存节点的后继 为目标位置节点 
    p->next = q->next;//节点的后继 指向后继的后继 
    *e = q->data;//节点后继 的值 目标位置节点的元素值 
    free(q);//清除空间 
    return OK;
}

/*
 * 访问元素
 */
void visit(ElemType e)
{
    printf("%d ", e);
}

/*
 * 遍历线性表
 */
void TraverseList(LinkList L, void (*visit)(ElemType))
{
    LinkList p = L->next;//后继 
    while (p)
    {
        visit(p->data);//访问 
        p = p->next;//指向后继 
    }
}

int main()
{
    LinkList L;
    InitList(&L);
    ElemType e;
    int i;
    if (L)
    {
        printf("init success\n");
    }

    if (isEmpty(L))
    {
        printf("list is empty\n");    
    }

    for (i = 0; i < 10; i++)
    {
        InsertElem(L, i + 1, i);
    }

    if (GetElem(L, 1, &e)) {
        printf("The first element is %d\n", e);
    }

    printf("length is %d\n", GetLength(L));

    printf("The 5 at %d\n", FindElem(L, 5, *compare));

    PreElem(L, 6, &e);
    printf("The 6's previous element is %d\n", e);

    NextElem(L, 6, &e);
    printf("The 6's next element is %d\n", e);

    DeleteElem(L, 1, &e);
    printf("delete first element is %d\n", e);

    printf("list:");
    TraverseList(L,visit);

    DestroyList(&L);
    if (!L) {
        printf("\ndestroy success\n");    
    }
}
