#include<iostream>
using namespace std;

typedef struct BiTNode
{
	char data;
	struct BiTNode* lchild, * rchild;
}BiTNode,*BiTree;

void CreateBiTree(BiTree& T)
{
	char ch;
	cin >> ch;
	if (ch == '0')
		T = NULL;
	else
	{
		T = new BiTNode;
		T->data = ch;
		CreateBiTree(T->lchild);
		CreateBiTree(T->rchild);
	}
}

void PreOrderTraverse(BiTree T)
{
	if (T)
	{
		cout << T->data;
		PreOrderTraverse(T->lchild);
		PreOrderTraverse(T->rchild);
	}
}

void InOrderTraverse(BiTree T)
{
	if (T)
	{
		InOrderTraverse(T->lchild);
		cout << T->data;
		InOrderTraverse(T->rchild);
	}
}

void PostOrderTraverse(BiTree T)
{
	if (T)
	{
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		cout << T->data;
	}
}

int main()
{
	while (1)
	{
		BiTree T;
		CreateBiTree(T);
		if (T == NULL)
			break;
		PreOrderTraverse(T);
		cout << endl;
		InOrderTraverse(T);
		cout << endl;
		PostOrderTraverse(T);
		cout << endl;
	}
}