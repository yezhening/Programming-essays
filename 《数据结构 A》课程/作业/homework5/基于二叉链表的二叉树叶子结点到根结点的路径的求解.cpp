#include<iostream>
using namespace std;
#define MaxSize 100

typedef struct BiTNode
{
	char data;
	struct BiTNode* lchild, * rchild;
}BiTNode, * BiTree;

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

void Path(BiTree& T, char path[], int length)
{
	if (T)
	{
		if (T->lchild == NULL && T->rchild == NULL)
		{
			path[length] = T->data;
			for (int i = length; i >= 0; i--)
				cout << path[i];
			cout << endl;
		}
		else
		{
			path[length++] = T->data;
			Path(T->lchild, path, length);
			Path(T->rchild, path, length);
			length--;
		}
	}
}

int main()
{
	while (1)
	{
		char path[MaxSize];
		BiTree T;
		CreateBiTree(T);
		if (T == NULL)
			break;
		int length = 0;
		Path(T, path, length);
	}
}



