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

int NodeCountZeroChild(BiTree T)
{
	if (T == NULL)
		return 0;
	else
	{
		if (!T->lchild && !T->rchild)
			return 1 + NodeCountZeroChild(T->lchild) + NodeCountZeroChild(T->rchild);
		else
			return 0 + NodeCountZeroChild(T->lchild) + NodeCountZeroChild(T->rchild);
	}
}

int NodeCountOneChild(BiTree T)
{
	if (T == NULL)
		return 0;
	else
	{
		if ((T->lchild && !T->rchild) || (!T->lchild && T->rchild))
			return 1 + NodeCountOneChild(T->lchild) + NodeCountOneChild(T->rchild);
		else
			return 0 + NodeCountOneChild(T->lchild) + NodeCountOneChild(T->rchild);
	}
}

int NodeCountTwoChild(BiTree T)
{
	if (T == NULL)
		return 0;
	else
	{
		if (T->lchild && T->rchild)
			return 1 + NodeCountTwoChild(T->lchild) + NodeCountTwoChild(T->rchild);
		else
			return 0 + NodeCountTwoChild(T->lchild) + NodeCountTwoChild(T->rchild);
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
		cout << NodeCountZeroChild(T) << " ";
		cout << NodeCountOneChild(T) << " ";
		cout << NodeCountTwoChild(T);
		cout << endl;
	}
}