#include<iostream>
using namespace std;
#define MaxSize 100

typedef struct BiTNode
{
	char data;
	struct BiTNode* lchild, * rchild;
}BiTNode, * BiTree;

void CreateBiTree(BiTree& T, char S[], int& depth)
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

int MaxWidth(BiTree T)
{
	if (T == NULL)
		return 0;
	int front = 1, rear = 1;//front指向队首，rear指向队尾的后一个位置 
	int last = 1, count = 0, max = 0;//last指向当前层的最右节点,记录宽度,记录最大值
	BiTree Q[MaxSize];//创建队列
	Q[rear] = T;//根节点入队

	while (front <= last)//还没有到当前层的最右节点
	{
		count++;
		BiTree p;//遍历指针
		p = Q[front++];//出队
		if (p->lchild)
			Q[++rear] = p->lchild;//若子树则继续入队
		if (p->rchild)
			Q[++rear] = p->rchild;
		if (front>last)//刚出队的元素是否为当前层的最右节点，是
		{
			last = rear;//更新last
			max = max > count ? max : count;
			count = 0;
		}
	}
	return max;
}

int main()
{
	char S[MaxSize];//输入的字符
	while (cin >> S && S[0] != '0')
	{
		int depth = -1;//进入树的深度
		BiTree T;
		CreateBiTree(T, S, ++depth);
		cout << MaxWidth(T) << endl;
	}
}



