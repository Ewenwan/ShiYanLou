/** 
 * 假设输入的中缀表达式为：  
 * （a+b)*((c+d)*e+f*h*g)  
 *
 * 1）以二叉链表的存储结构创建一棵。
 * 2）先根遍历为：此中缀表达式的前缀表达式  
 * 3）后根遍历为：此中缀表达式的后缀表达式   
 *
 * 2005/04/28
 */
#include<stack>  
#include<iostream>  
#include<stdio.h> 
#include<stdlib.h> 
#include<ctype.h>  
#include<string.h> 
 
using   namespace   std;  

//////////////////////////////////////////////////////////////////////////  
//   数据类型定义区  
//  
typedef   struct nodeTag{ /*   表达式二叉树结点类型   */  
    union{  
        int opnd;  
        char optr;  
    }val;  
    struct   nodeTag *left;  
    struct   nodeTag *right;  
}treeNode;  

typedef   struct pTag{ /*   优先表结点类型   */  
    char op;  
    int f;  
    int g;  
}Prior;  

//////////////////////////////////////////////////////////////////////////  
//   全局变量定义区  
//  
Prior pList[]   =   { /*   优先表   */  
    '+',   2,   1,  
    '-',   2,   1,  
    '*',   4,   3,  
    '/',   4,   3,  
    '^',   4,   5,  
    '(',   0,   5,  
    ')',   6,   0,  
    '$',   0,   0  
};  
stack<char> OptrStack; /*   操作符栈   */  
stack<treeNode*> ExprStack; /*   表达式栈   */  
const   int   NUM =   256;  
const   int   OPTR =   257;  
int tokenval; /*   下一输入值   */  

/**************************************************************************  
*   descr     :比较栈顶运算符与下一输入运算符优先关系  
*   param     :opf   栈顶运算符  
*   param     :opg   下一输入运算符  
*   return   :关系'>',   '=',   '<'  
**************************************************************************/  
char   Precede(char   opf,   char   opg)  
{  
    int   op1=-1,op2=-1;  
    for   (int   i=0;   i   <   8;   i++)  
    {  
        if   (pList[i].op   ==   opf)  
            op1 =   pList[i].f;  
        if   (pList[i].op   ==   opg)  
            op2   =   pList[i].g;  
    }  
    if   (op1   ==   -1   ||   op2   ==   -1)  
    {  
        cout<<"operator   error!"<<endl;  
        exit(1);  
    }  
    if   (op1   >   op2)  
        return   '>';  
    else   if   (op1   ==   op2)  
        return   '=';  
    else  
        return   '<';  
}  

/**************************************************************************  
*   descr     :  
*   return   :  
**************************************************************************/  
int   lexan()  
{  
    int   t;  
    while(1)  
    {  
        t   =   getchar();  
        if   ( (t ==' ')||(t=='\t')||(t=='\n')); //去掉空白字符  
        else   if   (isdigit(t))  
        {  
            ungetc(t,   stdin);  
            cin>>tokenval;  
            return   NUM;  
        }  
        else  
        {  
            return   t;  
        }    
    }  
}  
/**************************************************************************  
*   descr     :   建立二叉树数结点(叶结点)  
*   param     :   num   操作数  
*   return   :   二叉树叶结点指针   treeNode*  
**************************************************************************/  
treeNode*   mkleaf(int   num)  
{  
    treeNode   *tmpTreeNode   =   new   treeNode;  
    if   (tmpTreeNode   ==   NULL)  
    {  
        cout<<"Memory   allot   failed!"<<endl;  
        exit(1);  
    }  
    tmpTreeNode->left =   NULL;  
    tmpTreeNode->right =   NULL;  
    tmpTreeNode->val.opnd   =   num;  
    return   tmpTreeNode;  
}  

/**************************************************************************  
*   descr     :   建立二叉树运算符结点(内结点)  
*   param     :   op运算符  
*   param     :   left左子树指针  
*   param     :   right右子树指针  
*   return   :   二叉树内结点指针   treeNode*  
**************************************************************************/  
treeNode*   mknode(char   op,   treeNode*   left,treeNode*   right)  
{  
    treeNode   *tmpTreeNode   =   new   treeNode;  
    if   (tmpTreeNode   ==   NULL)  
    {  
        cout<<"Memory   allot   failed!"<<endl;  
        exit(1);  
    }  
    if   (left   ==   NULL   ||   right   ==   NULL)  
    {  
        cout<<"Lossing   operand!"<<endl;  
        exit(1);  
    }  
    tmpTreeNode->left =   left;  
    tmpTreeNode->right =   right;  
    tmpTreeNode->val.optr   =   op;  
    return   tmpTreeNode;  
}  

/**************************************************************************  
*   descr     :   建立表达式二叉树(参考严蔚敏，吴伟民的《数据结构》P/53)  
*   return   :   二叉树跟结点指针  
**************************************************************************/  
treeNode*   CreateBinaryTree()  
{  
    int     lookahead;  
    char   op;  
    treeNode   *opnd1,   *opnd2;  
    OptrStack.push('$');  
    lookahead   =   lexan();  
    while   (   lookahead   !=   '$'   ||   OptrStack.top()   !=   '$')  
    {  
        if   (lookahead   ==   NUM   )  
        {  
            ExprStack.push(   mkleaf(tokenval));  
            lookahead   =   lexan();  
        }  
        else  
        {  
            switch   (Precede(OptrStack.top(),   lookahead))  
            {  
            case   '<':  
                OptrStack.push(lookahead);  
                lookahead   =   lexan();  
                break;  
            case   '=':  
                OptrStack.pop();  
                lookahead   =   lexan();  
                break;  
            case   '>':  
                opnd2 =   ExprStack.top();ExprStack.pop();  
                opnd1 =   ExprStack.top();ExprStack.pop();  
                op =   OptrStack.top();OptrStack.pop();  
                ExprStack.push(   mknode(op,   opnd1,   opnd2));  
                break;  
            }  
        }  
    }  
    return   ExprStack.top();  
}  

/**************************************************************************  
*   descr     :   输出前缀表达式  
*   param     :  
*   return   :  
**************************************************************************/  
int   PreOrderTraverse(treeNode*   T)  
{  
    if   (   T   ==   NULL)  
        return   1;  
    if(T->left   !=   NULL)  
    {  
        cout<<T->val.optr<<"   ";  
        if   (PreOrderTraverse(T->left))  
            if   (PreOrderTraverse(T->right))  
                return   1;  
        return   0;  
    }  
    else  
    {  
        cout<<T->val.opnd<<"   ";  
        return   1;  
    }  
}  

/**************************************************************************  
*   descr     :   输出后缀表达式  
*   param     :  
*   return   :  
**************************************************************************/  
int   FollowOrderTraverse(treeNode*   T)  
{  
    if   (   T   ==   NULL)  
        return   1;  
    if   (   T->left   !=NULL)  
    {  
        if   (FollowOrderTraverse(T->left))  
            if   (FollowOrderTraverse(T->right))  
            {  
                cout<<T->val.optr<<"   ";  
                return   1;  
            }  
            return   0;  

    }  
    else  
    {  
        cout<<T->val.opnd<<"   ";  
        return   1;  
    }  
}  

//////////////////////////////////////////////////////////////////////////  
//   主程序  
//  
int   main()  
{  
    treeNode   *ExprTree;  
    ExprTree   =   CreateBinaryTree();  
    PreOrderTraverse(ExprTree);  
    cout<<endl;  
    FollowOrderTraverse(ExprTree);  
    cout<<endl;  
    return 0; 
}   
