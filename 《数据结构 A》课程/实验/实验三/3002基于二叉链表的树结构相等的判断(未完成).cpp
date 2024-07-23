#include<iostream>
using namespace std;

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

void CreateBiTree2(BiTree& T,char ch)
{
	/*char ch;
	cin >> ch;*///不能再放在这里了
	if (ch == '0')
		T = NULL;
	T = new BiTNode;
	T->data = ch;
	CreateBiTree(T->lchild);
	CreateBiTree(T->rchild);
}

int Compare(BiTree T1, BiTree T2)
{
	if (T1&&T2)
	{
		if (T1->data != T2->data)
			return -1;//表示该函数失败，但不退出程序:exit(0);返回值也可进行输出判断
		else
		{
			Compare(T1->lchild, T2->lchild);
			Compare(T1->rchild, T2->rchild);
		}
	}
	//cout << "Yes" << endl;一堆循环
}

int main()
{
	while (1)
	{
		char ch;
		cin >> ch;
		if (ch == '0')
			break;//输入放这里才能终止
		BiTree T1,T2;
		CreateBiTree2(T1,ch);
		CreateBiTree(T2);
		if (T1 == NULL||T2==NULL)
			break;
		if (Compare(T1, T2)!=-1)
			cout << "Yes" << endl;
		else
			cout << "No" << endl;
	}
}



