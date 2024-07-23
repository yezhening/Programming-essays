#include<iostream>
using namespace std;
int length = 0;
char t[100], path[100];

typedef struct BiTNode
{
	char data;
	struct BiTNode* lchild, * rchild;
}BiTNode,*BiTree;

void CreateBiTree(BiTree& T, char S[],int& depth)
{
	if (S[depth] == '0')
		T = NULL;
	else
	{
		T = new BiTNode;
		T->data = S[depth];
		CreateBiTree(T->lchild, S, ++depth);
		CreateBiTree(T->rchild, S, ++depth);
	}
}

void LongestPath(BiTree T,int n)
{
	if (T)
	{
		t[n] = T->data;
		if (!T->lchild && !T->rchild)
		{
			if (n+1 > length)//初始时不知道有没有结点，长度length设置为0，实际长度应+1；输出的是第一条最长路径，只有>时才交换赋予数值
			{
				length = n + 1;
				for (int i = 0; i <= n; i++)
					path[i] = t[i];
			}
		}
		else
		{
			LongestPath(T->lchild, n + 1);
			LongestPath(T->rchild, n + 1);
		}
	}
}

int main()
{
	char S[100];//输入的字符
	while (cin >> S && S[0] != '0')
	{
		int depth = -1;//进入树的深度
		BiTree T;
		CreateBiTree(T, S, ++depth);
		length = 0;//每一次都置0
		LongestPath(T, length);
		cout << length<<endl;
		for (int i = 0; i < length; i++)
			cout << path[i];
		cout << endl;
	}
}