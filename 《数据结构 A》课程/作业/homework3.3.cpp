#include <iostream>
#include <stdlib.h>
using namespace std;
#define OK 1
#define ERROR  0
#define OVERFLOW  -1
typedef int ElemType;
typedef int Status;

typedef struct LNode
{
	ElemType data;
	struct LNode* next;
}LNode, * LinkList;

Status InitList(LinkList& L)
{
	L = new LNode;
	L->next = NULL;
	return OK;
}
Status ListEmpty(LinkList L)
{
	if (L->next)
		return ERROR;
	else
		return OK;
}
Status PrintList(LinkList& L)
{
	if (!ListEmpty(L))
	{
		LinkList  p;
		p = L->next;
		while (p)
		{
			cout << p->data << " ";
			p = p->next;
		}
	}
	cout << endl;
	return OK;
}
Status CreateList(LinkList& L, int n)
{
	L = new LNode;
	L->next = NULL;
	LinkList  p, r;
	int i;
	r = L;
	for (i = 0; i < n; i++)
	{
		p = new LNode;
		cin >> p->data;
		p->next = NULL;
		r->next = p;
		r = p;
	}
	return OK;
}
LinkList CutList(LinkList L)
{
	LinkList slow;
	slow = new LNode;
	slow = L->next;
	LinkList fast;
	fast = new LNode;
	fast = L->next;
	LinkList mid;
	mid = new LNode;
	while (fast != NULL && fast->next != NULL)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	mid = slow->next;
	slow->next = NULL;
	return mid;
}

int main()
{
	LinkList L;
	InitList(L);
	CreateList(L, 10);
	PrintList(L);

	LinkList LL;
	LL = new LNode;
	LL->next = CutList(L);
	PrintList(L);
	PrintList(LL);
	return 0;
}