//线性表的链式表示
// 循环链表
// 从头结点开始，指针域依次指向下一个数据的存储地址，尾节点的指针域指向头结点存储地址 构成环 
/*
线性表的顺序存储结构是逻辑位置和物理位置都相邻，
而链式存储结构是逻辑位置相邻，但物理位置不一定相邻，
相比顺序存储结构，它不能随机存取，
但在插入和删除操作时不需要移动元素，
大大提高了增加和删除元素的效率。

顺序存储结构读取元素的效率比较高，链式存储结构添加和删除元素的效率比较高。
参考 https://www.cnblogs.com/rookiefly/p/3451310.html

还有问题 
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
    ElemType data;//元素  
    
    struct LNode *next;//后继指针 
}LNode, *LinkList;//链表 结构体变量  结构体指针 

/*
 * 初始化线性表
 */
void InitList(LinkList *L)
{
    *L = (LinkList) malloc(sizeof(LNode));// 分配头结点  一个元素 + 一个指针变量  的内存大小 
    if (*L == NULL) //分配失败
    {
        exit(OVERFLOW);
    }
    (*L)->next = *L;  //指针域指向它本身
}

/*
 * 销毁线性表
 */
void DestroyList(LinkList *L)
{
    LinkList temp,p;
    *L = (*L)->next;  //先令尾指针指向头结点，不然释放最后一个结点时尾指针，无法指向头结点 
    p = (*L)->next;  //p指向第一个结点
    while (p!=*L)   //p未到表头时 表尾部指向表头 
    {
        temp = p->next;//保存后继的节点 
        free(p);//删除当前节点内存 
        p = temp;//下一个节点 
    }
    (*L)->next = *L; //头结点指针域指向其本身
    
}

/*
 * 清空线性表
 */
void ClearList(LinkList L)
{
    DestroyList(&L);//清空内存 
    free(L); //释放头结点
    L->next = NULL;//后继指针为NULL 
}

/*
 * 判断是否为空
 */
Status isEmpty(LinkList L)
{
    if (L->next==L)//指针域指向它本身，肯定就是空了，该结点即为头结点
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 * 获取长度
 */
int GetLength(LinkList L)
{
    int i = 0;
    LinkList p = L->next->next;  //p指向第一个结点
    while (p!= L->next)//p未到表头时
    {
        ++i;//后继节点 计数 
        p = p->next;//依次遍历后继节点 
    }
    return i;
}

/*
 * 根据位置获取元素
 */
Status GetElem(LinkList L, int i, ElemType *e)
{
    int j = 0;
    LinkList p = L->next;//头结点 
   if (i<1 || i>GetLength(L))//越界 
    {
        return ERROR;
    }
    
    while (j < i)
    {
        ++j;//位置计数 
        p = p->next;//下一个后级节点 
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
    LinkList p = L->next->next;//p指向链表第一个元素
    while(p != L->next) //p未到表头时
    {
        ++i;//位置计数 +1 
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
    LinkList q, p = L->next->next;//p指向链表第一个元素
    while (p != L->next) //p未到表头时
    {
        q = p->next;//p节点的后继为q 
        if ( q!=L->next && q->data == cur_e)//q节点的元素与目标 元素相等且q未到尾 
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
    LinkList p = L->next->next;//p指向链表第一个元素
    LinkList q; 
    while (p != L->next) //p未到表头时
    {   
        q=p->next;//q指向p的后继 
        if (q!=L->next && p->data == cur_e)//节点元素与目标元素相等  且其后继为指向头 
        {
            *next_e = q->data;//当前节点的后继节点的元素为 目标元素的后继元素 
            return OK;
        }
        p = q;//指向下一个后继 
    }
    return ERROR;
}

/*
 * 插入元素
 */
Status InsertElem(LinkList L, int i, ElemType e)
{
    int j = 0;
    LinkList s, p = L->next;//p指向表头	
    if (i<1 || i>GetLength(L)+1)//位置越界 
    {
        return ERROR;
    }       
    while ( j < i - 1)//位置合理，找到第i-1个结点
    {
        ++j;//位置计数 +1 
        p = p->next;//指向后继 后移 
    }

    s = (LinkList) malloc(sizeof(LNode));//新开辟一段内存空间 
    s->data = e;//元素值 
    s->next = p->next;//后继指向p的后继 
    p->next = s;//p的后继指向新的元素节点 
    
    if (p == L)//p 表尾 指向表在表尾插入 
      L  = s;

    return OK;
}

/*
 * 删除元素并返回值
 */
Status DeleteElem(LinkList L, int i, ElemType *e)
{
    int j = 0;
    LinkList q, p = L->next;//p指向表头	
   if (i<1 || i>GetLength(L)+1)//位置越界 
    {
        return ERROR;
    }
        
    while (j < i - 1)
    {
        j++;
        p = p->next;//找到 目标位置节点 
    }

    q = p->next;//保存节点的后继 为目标位置节点 
    p->next = q->next;//节点的后继 指向后继的后继 
    *e = q->data;//节点后继 的值 目标位置节点的元素值 
    if (q == L)   //删除的是表尾元素，表尾指针发生改变
      L = p;
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
    LinkList p = L->next->next;//p指向链表第一个元素
    while (p != L->next)  //p未到表头
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
