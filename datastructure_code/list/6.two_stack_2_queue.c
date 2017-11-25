#include <stdio.h>  
#include <stdlib.h>  
#define STACK_INIT_SIZE 100  //存储空间初始分配量  
#define STACKINCREMENT 10    //存储空间分配增量  
typedef struct{  
int *base;  
int *top;  
int stacksize;} stack;  
int initStack(stack *s)//构造一个空栈  
{  
    s->base = (int *)malloc(sizeof(int)*STACK_INIT_SIZE);  
    if(s->base == NULL)  
    {  
        printf("存储空间分配失败！");  
        exit(-1);  
    }  
    s->top = s->base;  
    s->stacksize = STACK_INIT_SIZE;  
    return 1;  
}  
//压栈的方法，i为需要压入栈顶的元素  
int push(stack *s,int i)  
{  
    //判断栈容量是否已经满了，如果已满，重新分配存储空间  
    if(s->top - s->base >= s->stacksize)  
    {  
        s->base = (int *)realloc(s->base,(s->stacksize + STACKINCREMENT)*sizeof(int));  
    }  
    if(s->base == NULL)  
    {  
        printf("内存分配失败！");  
        exit(-1);  
    }  
    *s->top++ = i; //把元素压栈  
    return 1;  
}  
//弹栈的方法  
int pop(stack *s)  
{  
    if(s->base == s->top)  
        return -1;  
        //本例中默认压入栈中的元素都是正整数，所以返回-1表示栈空  
    //如果栈不空，则删除栈顶元素并返回其值  
    return *--s->top;  
}  
//利用两个栈模拟一个队列  
int main()  
{  
    stack st1,st2;  
    initStack(&st1);  
    initStack(&st2);  
    //假设传入的队列为 1，2，3，4，5，6  
    //则出队列的时候顺序为 1，2，3，4，5，6  
    //先在栈1中压入这几个数  
    int i;  
    printf("进入队列的序列为:\n");  
    for(i = 1; i < 7 ;i++)  
    {  
        push(&st1,i);  
        printf("%d ",i);  
  
    }  
    printf("\n");  
    //出栈的时候每次先把1栈中除栈底元素之外的所有元素弹到2栈中  
    //然后再弹出1栈栈底元素  
    //最后再把2栈中的元素压入1栈中  
    //重复以上动作，直到1栈为空  
    printf("出队列的顺序为:\n");  
    while(st1.base!= st1.top)  
    {  
        while((st1.top - st1.base) > 1)  
            push(&st2,pop(&st1));  
        printf("%d ",pop(&st1));  
        while((st2.top - st2.base) > 0)  
            push(&st1,pop(&st2));  
    }  
    return 0;  
}
