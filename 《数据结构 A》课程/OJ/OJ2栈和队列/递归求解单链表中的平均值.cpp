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
Status Push(LinkStack& S, SElemType e)
{
	LinkStack p;
	p = new StackNode;
	p->data = e;
	p->next = S;
	S = p;
	return OK;
}
double GetAverage(LinkStack S, int n)
{
	if (S->next == NULL)
		return S->data;
	else
		return(GetAverage(S->next, n - 1) * (n - 1) + S->data) / n;
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
		cout << fixed << setprecision(2) << GetAverage(S, length) << endl;
	}
	return 0;
}
