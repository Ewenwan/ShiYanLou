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
#define INIT_SIZE 20//初始栈分配内存大小 
#define INCREMENT_SIZE 5//分配增长  

typedef int SElemType;
typedef int Status;

/*
 * 存储结构
 */
typedef struct
{
    SElemType *base;    //栈尾指针   表头 
    SElemType *top;     //栈顶指针  表尾 
    int size;           //栈的大小
}SqStack;

/*
 * 初始化栈
 */
Status InitStack(SqStack *S)
{
    S->base = (SElemType*) malloc(INIT_SIZE * sizeof(SElemType));//表头 
    if (!S->base)//分配出错 
    {
        exit(OVERFLOW);
    }
    S->top = S->base;//栈顶也是栈尾  表头也是表尾 
    S->size = INIT_SIZE;
    return OK;
}

/*
 * 销毁栈
 */
Status DestroyStack(SqStack *S)
{
    free(S->base);//栈尾 表头 
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
    S->top = S->base;//栈顶指向栈尾 
    return OK;
}

/*
 * 判断栈是否为空
 */
Status IsEmpty(SqStack S)
{
    if (S.top == S.base)//栈顶 = 栈尾 
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
    return S.top - S.base;//栈顶-栈尾 
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
    if ((S->top - S->base) / sizeof(SElemType) >= S->size)//内存不够了 
    {
        S->base = (SElemType*) realloc(S->base, (S->size + INCREMENT_SIZE) * sizeof(SElemType));
        if (!S->base)//分配出错 
        {
            exit(OVERFLOW);
        }
        S->top = S->base + S->size;//更新栈顶地址   向上生长 
        S->size += INCREMENT_SIZE;//更新大小 
    }
    *S->top = e;//*地址  取值  更新值 
    S->top++;//地址+1 
    return OK;
}

/*
 * 退栈
 */
Status Pop(SqStack *S, SElemType *e)
{
    if (S->top == S->base)//空 
    {
        return ERROR;
    }
    S->top--;//地址-1 
    *e = *S->top;//取栈顶地址里的值 
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
        S.base++;//栈尾地址++ 
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
