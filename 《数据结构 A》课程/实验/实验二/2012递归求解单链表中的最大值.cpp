#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;
#define OK 1
#define ERROR 0
#define OVERFLOW -1
typedef int SElemType;
typedef int Status;

typedef struct StackNode
{
	SElemType data;
	struct StackNode* next;
}StackNode, * LinkStack;
Status InitStack(LinkStack& S)
{
	S = NULL;
	return OK;
}
Status Push(LinkStack& S, SElemType e)//前插法，S是第一个元素结点而不是头结点，在递归结束时要+1
{
	LinkStack p;
	p = new StackNode;
	p->data = e;
	p->next = S;
	S = p;
	return OK;
}
int FindMax(LinkStack S)
{
	int max;
	if (S->next == NULL)
	{
		return S->data;
	}
	else
	{
		max = FindMax(S->next);
		if (S->data >= max)//S是第一个元素
		{
			return S->data;
		}
		return max;
	}
}
int main()
{
	while (1)
	{
		int length;
		cin >> length;
		if (length == 0)
			exit(0);
		LinkStack S;
		InitStack(S);
		int i;
		for (i = 0; i < length; i++)
		{
			int t;
			cin >> t;
			Push(S, t);
		}
		int lnodecount = 0;
		cout << FindMax(S) << endl;
	}
	return 0;
}