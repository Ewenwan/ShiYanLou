#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define OK 1
#define ERROR 0

typedef int Status;
typedef int TElemType;

/*
 * 存储结构
 */
typedef struct BiTNode
{
	TElemType data;	//数据
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

/*
 * 创建二叉树,输入0表示创建空树
 */
Status CreateBiTree(BiTree *T)
{
	TElemType e;
	scanf("%d", &e);
	if (e == 0)
	{
		*T = NULL;
	}
	else
	{
		*T = (BiTree) malloc(sizeof(BiTNode));
		if (!T)
		{
			exit(OVERFLOW);
		}
		(*T)->data = e;
		CreateBiTree(&(*T)->lchild);	//创建左子树
		CreateBiTree(&(*T)->rchild);	//创建右子树
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
 * 先序遍历二叉树
 */
Status PreOrderTraverse(BiTree T, void (*visit)(TElemType))
{
	if (T)
	{
		visit(T->data);
		PreOrderTraverse(T->lchild, visit);
		PreOrderTraverse(T->rchild, visit);
	}
}

/*
 * 中序遍历二叉树
 */
Status InOrderTraverse(BiTree T, void (*visit)(TElemType))
{
	if (T)
	{
		InOrderTraverse(T->lchild, visit);
		visit(T->data);
		InOrderTraverse(T->rchild, visit);
	}
}

/*
 * 后序遍历二叉树
 */
Status PostOrderTraverse(BiTree T, void (*visit)(TElemType))
{
	if (T)
	{
		PostOrderTraverse(T->lchild, visit);
		PostOrderTraverse(T->rchild, visit);
		visit(T->data);
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
}
