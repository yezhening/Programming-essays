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

void DoubleTraverse(BiTree& T)
{
	if (T)
	{
		cout << T->data;
		DoubleTraverse(T->lchild);
		cout << T->data;
		DoubleTraverse(T->rchild);
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
		DoubleTraverse(T);
		cout << endl;
	}
}



