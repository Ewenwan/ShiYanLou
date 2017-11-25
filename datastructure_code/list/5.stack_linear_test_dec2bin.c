#include<stdio.h>  
#include<malloc.h>  

#define ERROR 0  
#define OK 1  
#define STACK_INT_SIZE 10  /*存储空间初始分配量*/  
#define STACKINCREMENT 5  /*存储空间分配增量*/  
typedef  int ElemType; /*定义元素的类型 int 等价 数据类型 定义 */  

typedef struct{  
    ElemType *base;  
    ElemType *top;  
    int stacksize;     /*当前已分配的存储空间*/  
}SqStack;  
  
   
int push(SqStack *S,ElemType e); /*入栈*/  
int Pop(SqStack *S,ElemType *e);  /*出栈*/  
void PrintStack(SqStack *S);   /*出栈并输出栈中元素*/  
  
  
int Push(SqStack *S,ElemType e){  
    if(S->top-S->base==STACK_INT_SIZE)  
        return 0;  
    *(++S->top)=e;  
    return 1;  
}/*Push*/  
  
int Pop(SqStack *S,ElemType *e){  
    if(S->top==S->base)  
        return 0;  
    *e=*S->top;  
    S->top--;  
    return 1;  
}/*Pop*/  
  
  
void Conversion(SqStack *S,ElemType e)  
{  
    while(e/2)  
    {  
        Push(S,e%2);  
        e/=2;  
    }  
    Push(S,e);  
  
}  
  
void PrintStack(SqStack *S){  
    ElemType e;  
    while(Pop(S,&e))  
        printf("%d",e);  
    printf("\n");  
}/*Pop_and_Print*/  
  
int main(){  
    int num;  
    SqStack *S;  
    S=(SqStack *)malloc(sizeof(SqStack));  
    S->base=(ElemType *)malloc(STACK_INT_SIZE *sizeof(ElemType));  
    if(!S->base) return ERROR;  
    S->top=S->base;  
    S->stacksize=STACK_INT_SIZE;  
  
    printf("请输入要转换的数字：\n");  
    scanf("%d",&num);  
    Conversion(S,num);  
    PrintStack(S);  
    return 0;  
} 
