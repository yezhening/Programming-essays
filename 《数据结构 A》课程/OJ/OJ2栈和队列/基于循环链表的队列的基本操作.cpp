#include<iostream>
#include<stdlib.h>
using namespace std;
#define OK 1
#define ERROR 0
#define OVERFLOW -1
typedef int QElemType;
typedef int Status;

typedef struct QNode
{
	QElemType data;
	struct QNode* next;
}QNode, * LinkQueue;
Status InitQueue(LinkQueue& Q)
{
	Q = new QNode;
	Q->next = Q;
	return OK;
}
Status Push(LinkQueue& Q, QElemType e)
{
	LinkQueue p;
	p= new QNode;
	p->data = e;
	p->next = Q->next;//画图
	Q->next = p;
	Q = p;//Q指向队尾
	return OK;
}
Status Pop(LinkQueue& Q)
{
	LinkQueue p;
	p= Q->next->next;//指向队头元素，跳过头结点
	cout << p->data << " ";
	Q->next->next = p->next;//两个next：跳过头结点
	if (p == Q)//最后一个元素被删
		Q=Q->next;//Q->next=Q错
	delete p;
	return OK;
}
Status IsEmpty(LinkQueue Q)
{
	if (Q->next == Q)
		return ERROR;
	else
		return OK;
}
int main()
{
	while (1)
	{
		int m, n;
		cin >> m >> n;
		if (m == 0 && n == 0)
			exit(0);
		LinkQueue Q;
		InitQueue(Q);
		int i;
		for(i=0;i<m;i++)
		{
			int e; 
			cin >> e;
			Push(Q, e);
		}
		for (i = 0; i < n; i++)
		{
			Pop(Q);
		}
		cout<<IsEmpty(Q);
		cout << endl;
	}
	return 0;
}