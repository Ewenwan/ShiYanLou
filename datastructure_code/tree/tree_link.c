#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define OK 1
#define ERROR 0
/*
常用的树结构--二叉树，它的特点是一个结点的直接子节点最多只能有两个，
并且有左右之分。在二叉树中有种常见的称为完全二叉树的结构，
它的特点是除最后一层外每一层的结点数为2^i-1，
最后一层的结点数若不满足2^i-1，那么最后一层的结点是自左向右排列的

*/
typedef int Status;
typedef int TElemType;

/*
 * 存储结构
 */
typedef struct BiTNode
{
    TElemType data;    //数据
    struct BiTNode *lchild, *rchild;//左孩子  右孩子 结构体指针
}BiTNode, *BiTree;//结构体变量  结构体指针

/*
 * 创建二叉树,输入0表示创建空树
 */
Status CreateBiTree(BiTree *T)
{
    TElemType e;
    scanf("%d", &e);//输入一个整形树
    if (e == 0)
    {
        *T = NULL;//0为空树
    }
    else
    {
        *T = (BiTree) malloc(sizeof(BiTNode));//初始化一个结构体的内存空间
        if (!T)//创建失败
        {
            exit(OVERFLOW);
        }
        (*T)->data = e;//初始数据  根数据
        CreateBiTree(&(*T)->lchild);    //创建左子树  迭代  0的话 左子树为空
        CreateBiTree(&(*T)->rchild);    //创建右子树        1的话又会迭代 创建左子树 又子树  0 左子树为空  1 右子树
		//又会创建左子树 右子树 0 左子树为空 0 右子树为空 结束 
    }
    return OK;
}

/*
 * 访问元素
 */
void visit(TElemType e)
{
    printf("%d ", e);
}

/*
 * 先序遍历二叉树：指先访问根，然后访问孩子的遍历方式
 */
Status PreOrderTraverse(BiTree T, void (*visit)(TElemType))
{
    if (T)
    {
        visit(T->data);//先访问父 元素
        PreOrderTraverse(T->lchild, visit);//访问左子树 元素
        PreOrderTraverse(T->rchild, visit);//访问右子树元素
    }
}

/*
 * 中序遍历二叉树：指先访问左（右）孩子，然后访问根，最后访问右（左）孩子的遍历方式
 */
Status InOrderTraverse(BiTree T, void (*visit)(TElemType))
{
    if (T)
    {
        InOrderTraverse(T->lchild, visit);//访问左子树 元素
        visit(T->data);//访问父 元素
        InOrderTraverse(T->rchild, visit);//访问右子树元素
    }
}

/*
 * 后序遍历二叉树：指先访问孩子，然后访问根的遍历方式
 */
Status PostOrderTraverse(BiTree T, void (*visit)(TElemType))
{
    if (T)
    {
        PostOrderTraverse(T->lchild, visit);//先访问左子树 元素
        PostOrderTraverse(T->rchild, visit);//访问右子树元素
        visit(T->data);//访问父 根 元素
    }
}

int main()
{
    BiTree T;
    printf("创建树，输入0为空树：\n");
    CreateBiTree(&T);
    printf("先序遍历：");
    PreOrderTraverse(T, *visit);
    printf("\n中序遍历：");
    InOrderTraverse(T, *visit);
    printf("\n后序遍历：");
    PostOrderTraverse(T, *visit);
    printf("\n");

    return 0;
}
