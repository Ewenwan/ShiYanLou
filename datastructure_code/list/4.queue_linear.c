/*
链式 队列 
栈是先进后出，而队列是先进先出。
队列刚好和栈相反，它是一种先进先出的线性表，
只能在一端插入元素，在另一端删除元素，
允许插入元素（入队）的一端称为队尾，
允许删除元素（出队）的一端称为队头。

队列也一样有顺序和链式存储结构两种表示方法，
前面的栈我们实现了顺序存储结构，
*/ 
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int QElemType;
typedef int Status;

/*
 * 存储结构
 */
typedef struct QNode
{
    QElemType data;//元素 
    struct QNode *next;//后继指针   链式队列 
}QNode, *QueuePtr;

typedef struct
{
    QueuePtr front;   //队头指针
    QueuePtr rear;    //队尾指针
}LinkQueue;

/*
 * 初始化队列
 */
Status InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QueuePtr) malloc(sizeof(QNode));//队尾 队头 
    if (!Q->front)//分配出错 
    {
        exit(OVERFLOW);
    }
    Q->front->next = NULL;//为空指针 
    return OK;
}

/*
 * 销毁队列
 */
Status DestroyQueue(LinkQueue *Q)
{
    while (Q->front)//
    {
        Q->rear = Q->front->next;//下一个 
        free(Q->front);
        Q->front = Q->rear;
    }
    return OK;
}

/*
 * 清空队列
 */
Status ClearQueue(LinkQueue *Q)
{
    DestroyQueue(Q);
    InitQueue(Q);
    return OK;
}

/*
 * 判断队列是否为空
 */
Status IsEmpty(LinkQueue Q)
{
    if (Q.front->next == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 * 获取队列的长度
 */
int GetLength(LinkQueue Q)
{
    int i = 0;
    QueuePtr p = Q.front;
    while (Q.rear != p)
    {
        i++;
        p = p->next;
    }
    return i;
}

/*
 * 获取队头元素
 */
Status GetHead(LinkQueue Q, QElemType *e)
{
    QueuePtr p;
    if (Q.front == Q.rear)
    {
        return ERROR;
    }
    p = Q.front->next;
    *e = p->data;
    return OK;
}

/*
 * 入队
 */
Status EnQueue(LinkQueue *Q, QElemType e)
{
    QueuePtr p = (QueuePtr) malloc(sizeof(QNode));
    if (!p)
    {
        exit(OVERFLOW);
    }
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    return OK;
}

/*
 * 出队
 */
Status DeQueue(LinkQueue *Q, QElemType *e)
{
    QueuePtr p;
    if (Q->front == Q->rear)
    {
        return ERROR;
    }
    p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)
    {
        Q->rear = Q->front;
    }
    free(p);
    return OK;
}

/*
 * 访问元素
 */
void visit(QElemType e)
{
    printf("%d ", e);
}

/*
 * 遍历队列
 */
Status TraverseQueue(LinkQueue Q, void (*visit)(QElemType))
{
    QueuePtr p = Q.front->next;
    while (p)
    {
        visit(p->data);
        p = p->next;
    }
    return OK;
}

int main()
{
    LinkQueue Q;
    if (InitQueue(&Q))
    {
        QElemType e;
        int i;

        printf("init_success\n");

        if (IsEmpty(Q))
        {
            printf("queue is empty\n");
        }

        for (i = 0; i < 10; i++)
        {
            EnQueue(&Q, i);
        }

        GetHead(Q, &e);
        printf("The first element is %d\n", e);

        printf("length is %d\n", GetLength(Q));

        DeQueue(&Q, &e);
        printf("delete element is %d\n", e);

        TraverseQueue(Q, *visit);

        if (DestroyQueue(&Q))
        {
            printf("\ndestroy_success\n");
        }
    }
}
