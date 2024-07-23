#include <iostream>
using namespace std;
#include <stdlib.h>
typedef int ElemType;
typedef struct Lnode
{
	ElemType data;
	struct Lnode* next;
}Lnode,*LinkList;
void InitList(LinkList& L)
{
	L = new Lnode;
	L->next = NULL;
}
void CreateList(LinkList& L, int n)
{
	LinkList r,p;
	int i;
	L = new Lnode;
	L->next = NULL;
	r = L;
	if (n == 0)
		exit(0);
	for (i = 0; i < n;i++)
	{
		p = new Lnode;
		cin >> p->data;
		p->next = NULL;
		r->next = p;
		r = p;
	}
}
void Inverse(LinkList& L)
{
	LinkList p,q;
	p = L->next;
	L->next = NULL;
	while (p != NULL)
	{
		q = p->next;
		p->next = L->next;
		L->next = p;
		p = q;
	}
}
void Print(LinkList &L)
{
	LinkList p;
	p = L->next;
	while (p)
	{
		cout << p->data;
		p = p->next;
		if (p != NULL)
			cout << " ";
	}
	cout << endl;
}
int main()
{
	while (1)
	{
		LinkList L;
		int length;
		cin >> length;
		CreateList(L, length);
		Inverse(L);
		Print(L);
	}
	return 0;
}