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
Status Push(LinkStack& S, SElemType e)//前插法，S是第一个元素结点而不是头结点，所以在递归结束时要+1
{
	LinkStack p;
	p = new StackNode;
	p->data = e;
	p->next = S;
	S = p;
	return OK;
}
int LnodeCount(LinkStack S, int lnodecount)//传递个数而不是传递长度
{
	if (S->next == NULL)
	{
		lnodecount = lnodecount+ 1;//因为是前插法
		return lnodecount;
	}
	else
	{
		lnodecount = lnodecount + 1;
		return(LnodeCount(S->next, lnodecount));
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
		cout << LnodeCount(S, lnodecount) << endl;
	}
	return 0;
}