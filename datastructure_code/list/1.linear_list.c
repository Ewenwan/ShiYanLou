// 性表的顺序存储结构是逻辑位置和物理位置都相邻
/*
有且仅有一个开始结点没有前驱但有一个后继结点，
有且仅有一个终端结点没有后继但有一个前驱结点，
其它的结点都有且仅有一个前驱和一个后继结点。
一般地，一个线性表可以表示成一个线性序列：k1，k2，…，kn，其中k1是开始结点，kn是终端结点。
顺序存储结构读取元素的效率比较高，链式存储结构添加和删除元素的效率比较高。
*/

#include <stdio.h>  //标准库 输入输出 
#include <stdlib.h> //标准库  

// 宏定义 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INIT_SIZE 10        //初始化表长度 节点数量 
#define INCREMENT_SIZE 5    //分配增量  内存不够后 新增加节点数量增量 

//  等价 数据类型 定义 
typedef int Status;  //状态 
typedef int Elemtype;//表 元素类型 

/*
 * 存储结构  结构体  SqList *list;  list ->  length
 */
typedef struct
{
    Elemtype *elem;    //存储空间基址  指针 基地址 
    int length;        //当前长度(实际长度)
    int size;          //当前分配的表长大小(分配大小) 
}SqList;

/*
 * 初始化一个空的线性表
 */
Status InitList(SqList *L)
{
    L->elem = (Elemtype *) malloc(INIT_SIZE * sizeof(Elemtype));//初始大小*元素类型存储长度  开辟一段内存空间 malloc(size) 
    if (!L->elem)//基地址 
    {
        return ERROR;
    }
    L->length = 0;// 实际长度 
    L->size = INIT_SIZE;//初始分配大小 
    return OK;//返回状态 
}

/*
 * 销毁线性表
 */
Status DestroyList(SqList *L)
{
    free(L->elem);//删除内存空间 
    L->length = 0;
    L->size = 0;
    return OK;
}

/*
 * 清空线性表
 */
Status ClearList(SqList *L)
{
    L->length = 0;// 实际长度 
    return OK;
}

/*
 * 判断线性表是否为空
 */
Status isEmpty(const SqList L)
{
    if (0 == L.length)
    {
        return TRUE;//1 为空 
    }
    else
    {
        return FALSE;// 0 为非空 
    }
}

/*
 * 获取长度
 */
Status getLength(const SqList L)
{
    return L.length;// 实际长度 
}

/*
 * 根据位置获取元素  表 L  位置 i  返回的元素值  GetElem(L, 1, &e)
 */
Status GetElem(const SqList L, int i, Elemtype *e)
{
    if (i < 1 || i > L.length)
    {
        return ERROR;
    }
    *e = L.elem[i-1];
    return OK;
}

/*
 * 比较两个元素是否相等
 */
Status compare(Elemtype e1, Elemtype e2)
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
 * 查找元素           表 L       元素值          返回状态 
 */
Status FindElem(const SqList L, Elemtype e, Status (*compare)(Elemtype, Elemtype))
{
    int i;
    for (i = 0; i < L.length; i++)
    {
        if (!(*compare)(L.elem[i], e))// (*compare)(L.elem[i], e) 相等 为0 
        {
            return i + 1;// 返回元素位置 
        }
    }
    if (i >= L.length)
    {
        return ERROR;
    }
}

/*
 * 查找前驱元素       表 L      当前元素值      前面的元素值    PreElem(L, 6, &e);  &e取地址 
 */
Status PreElem(const SqList L, Elemtype cur_e, Elemtype *pre_e)
{
    int i;
    for (i = 0; i < L.length; i++)
    {
        if (cur_e == L.elem[i])//查找当前元素位置 
        {
            if (i != 0)//确保  i - 1不越界 
            {
                *pre_e = L.elem[i - 1];//返回前一个元素值  
            }
            else
            {
                return ERROR;
            }
        }
    }
    if (i >= L.length)
    {
        return ERROR;
    }
}

/*
 * 查找后继元素       表 L      当前元素值      后面的元素值    NextElem(L, 6, &e);  &e取地址 
 */
Status NextElem(const SqList L, Elemtype cur_e, Elemtype *next_e)
{
    int i;
    for (i = 0; i < L.length; i++)
    {
        if (cur_e == L.elem[i])//查找当前元素位置 
        {
            if (i < L.length - 1)//确保  i + 1不越界 
            {
                *next_e = L.elem[i + 1];
                return OK;
            }
            else
            {
                return ERROR;
            }
        }
    }
    if (i >= L.length)
    {
        return ERROR;
    }
}

/*
 * 插入元素        表 L     位置   插入的值    InsertElem(&L, i + 1, i);
 */
Status InsertElem(SqList *L, int i, Elemtype e)
{
    Elemtype *new;//指针 
    if (i < 1 || i > L->length + 1)//位置越界 
    {
        return ERROR;
    }
    if (L->length >= L->size)//实际大小 大于等于 开辟的空间大小 
    {
        new = (Elemtype*) realloc(L->elem, (L->size + INCREMENT_SIZE) * sizeof(Elemtype));//在原来内存空间上 新 开闭一段存储空间 
        if (!new)//开辟出错  可能到了 系统空间 
        {
            return ERROR;
        }
        L->elem = new;//新的存储空间  元素 
        L->size += INCREMENT_SIZE;//新的 分配大小 
    }
    Elemtype *p = &L->elem[i - 1];//目标位置元素地址 指针 
    Elemtype *q = &L->elem[L->length - 1];//最后元素地址指针  
    for (; q >= p; q--)
    {
        *(q + 1) = *q;//从最后元素开始 一次向后 移动一个位置 
    }
    *p = e;//将增添的元素存入 
    ++L->length;//实际长度+1 
    return OK;
}

/*
 * 删除元素并返回其值   表L   需要删除的位置  返回被删除的元素   DeleteElem(&L, 1, &e)  &e 取地址 指向这个地址的指针 
 */
Status DeleteElem(SqList *L, int i, Elemtype *e)
{
    if (i < 1 || i > L->length)//位置越界 
    {
        return ERROR;
    }
    Elemtype *p = &L->elem[i - 1];// 目标位置元素地址 指针 p指向的地址 
    *e = *p;//目标元素值 
    for (; p < &L->elem[L->length]; p++)//元素地址 范围 
    {
        *(p) = *(p + 1);//向前 移动一个位置 
    }
    --L->length;//实际长度-1 
    return OK;
}

/*
 * 访问元素
 */
void visit(Elemtype e)
{
    printf("%d ", e);
}

/*
 * 遍历线性表
 */
Status TraverseList(const SqList L, void (*visit)(Elemtype))
{
    int i;
    for(i = 0; i < L.length; i++)
    {
        visit(L.elem[i]);
    }
    return OK;
}

/*
 * 主函数测试
 */
int main()
{
    SqList L;// 结构体变量 
    if (InitList(&L))//初始化表 
    {
        Elemtype e;//元素变量 
        printf("init_success\n");
        int i;
        for (i = 0; i < 10; i++)
        {
            InsertElem(&L, i + 1, i);//从1位置添加元素  10次 
        }
        printf("length is %d\n", getLength(L));//打印 表 长度 
        if (GetElem(L, 1, &e)) {
            printf("The first element is %d\n", e);
        }
        else
        {
            printf("element is not exist\n");        
        }
        if (isEmpty(L))//表为空？ 
        {
            printf("list is empty\n");
        }
        else
        {
            printf("list is not empty\n");
        }
        printf("The 5 at %d\n", FindElem(L, 5, *compare));//查找元素 
        PreElem(L, 6, &e);//前驱元素 
        printf("The 6's previous element is %d\n", e);
        NextElem(L, 6, &e);//后驱元素 
        printf("The 6's next element is %d\n", e);
        DeleteElem(&L, 1, &e);//删除元素 
        printf("delete first element is %d\n", e);
        printf("list:");
        TraverseList(L,visit);//遍历 表 
        if (DestroyList(&L))//删除 表 
        {
            printf("\ndestroy_success\n");
        }
    }
}
