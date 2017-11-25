/*
线性结构-栈——井结构 
栈是先进后出，而队列是先进先出。

栈是只能在表尾进行插入或删除操作的线性表，
通常我们称表尾端为栈顶
，表头端为栈底，
它是一种先进后出的线性表，
既只能在表尾栈顶端插入元素，称为入栈，
也只能在表尾端栈底删除元素，称为退栈。

栈既然也是线性表，那么它也有
顺序存储结构和链式存储结构两种表示方法，
这两种表示方法实现类似，
*/

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define INIT_SIZE 20
#define INCREMENT_SIZE 5

typedef int SElemType;
typedef int Status;

/*
 * 存储结构
 */
typedef struct
{
    SElemType *base;    //栈尾指针
    SElemType *top;        //栈顶指针
    int size;            //栈的大小
}SqStack;

/*
 * 初始化栈
 */
Status InitStack(SqStack *S)
{
    S->base = (SElemType*) malloc(INIT_SIZE * sizeof(SElemType));
    if (!S->base)
    {
        exit(OVERFLOW);
    }
    S->top = S->base;
    S->size = INIT_SIZE;
    return OK;
}

/*
 * 销毁栈
 */
Status DestroyStack(SqStack *S)
{
    free(S->base);
    S->base = NULL;
    S->top = NULL;
    S->size = 0;
    return OK;
}

/*
 * 清空栈
 */
Status ClearStack(SqStack *S)
{
    S->top = S->base;
    return OK;
}

/*
 * 判断栈是否为空
 */
Status IsEmpty(SqStack S)
{
    if (S.top == S.base)
    {
        return TRUE;
    }
    else
        return FALSE;
}

/*
 * 获取栈的长度
 */
int GetLength(SqStack S)
{
    return S.top - S.base;
}


/*
 * 获取栈顶元素
 */
Status GetTop(SqStack S, SElemType *e)
{
    if (S.top > S.base)
    {
        *e = *(--S.top);
        return OK;
    }
    else
    {
        return ERROR;
    }
}

/*
 * 压栈
 */
Status Push(SqStack *S, SElemType e)
{
    if ((S->top - S->base) / sizeof(SElemType) >= S->size)
    {
        S->base = (SElemType*) realloc(S->base, (S->size + INCREMENT_SIZE) * sizeof(SElemType));
        if (!S->base)
        {
            exit(OVERFLOW);
        }
        S->top = S->base + S->size;
        S->size += INCREMENT_SIZE;
    }
    *S->top = e;
    S->top++;
    return OK;
}

/*
 * 退栈
 */
Status Pop(SqStack *S, SElemType *e)
{
    if (S->top == S->base)
    {
        return ERROR;
    }
    S->top--;
    *e = *S->top;
    return OK;
}

/*
 * 访问元素
 */
void visit(SElemType e)
{
    printf("%d ", e);
}

/*
 * 遍历栈
 */
Status TraverseStack(SqStack S, void (*visit)(SElemType))
{
    while (S.top > S.base)
    {
        visit(*S.base);
        S.base++;
    }
    return OK;
}

int main()
{
    SqStack S;
    if (InitStack(&S))
    {
        SElemType e;
        int i;

        printf("init_success\n");

        if (IsEmpty(S))
        {
            printf("Stack is empty\n");
        }

        for (i = 0; i < 10; i++)
        {
            Push(&S, i);
        }

        GetTop(S, &e);
        printf("The first element is %d\n", e);

        printf("length is %d\n", GetLength(S));

        Pop(&S, &e);
        printf("Pop element is %d\n", e);

        TraverseStack(S, *visit);

        if (DestroyStack(&S))
        {
            printf("\ndestroy_success\n");
        }
    }
}
