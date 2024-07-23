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

int Depth(BiTree T)
{
	if (T == NULL)
		return 0;
	int m, n;
	m = Depth(T->lchild);
	n = Depth(T->rchild);
	if (m > n)
		return (m + 1);
	else
		return (n + 1);
}

int main()
{
	while (1)
	{
		BiTree T;
		CreateBiTree(T);
		if (T == NULL)
			break;
		cout << Depth(T);
		cout << endl;
	}
}