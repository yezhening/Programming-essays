#include <iostream>
#include <stdlib.h>
#include<string.h>
#include <iomanip>
using namespace std;
#define OK 1
#define ERROR  0
#define OVERFLOW  -1
#define MAXSIZE 100
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
Status CreatList(LinkList& L, int n)
{
	L = new LNode;
	L->next = NULL;
	LinkList  p,r;
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
Status ListInsert(LinkList& L)
{
	LinkList  head, newnode;
	head = L->next;
	newnode = new LNode;
	cin >> newnode->data;
	while (head)
	{
		if (head->data < newnode->data)
		{
			head = head->next;
		}
		else
		{
			newnode->next = head->next;
			head->next = newnode;
			break;
		}
	}
	return OK;
}

int main()
{
	LinkList L;
	InitList(L);
	CreatList(L,10);
	PrintList(L);
	ListInsert(L);
	PrintList(L);
	return 0;
}