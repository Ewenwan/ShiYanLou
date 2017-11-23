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
	QElemType data;
	struct QNode *next;
}QNode, *QueuePtr;

typedef struct
{
	QueuePtr front;	//队头指针
	QueuePtr rear;	//队尾指针
}LinkQueue;

/*
 * 初始化队列
 */
Status InitQueue(LinkQueue *Q)
{
	Q->front = Q->rear = (QueuePtr) malloc(sizeof(QNode));
	if (!Q->front)
	{
		exit(OVERFLOW);
	}
	Q->front->next = NULL;
	return OK;
}

/*
 * 销毁队列
 */
Status DestroyQueue(LinkQueue *Q)
{
	while (Q->front)
	{
		Q->rear = Q->front->next;
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
