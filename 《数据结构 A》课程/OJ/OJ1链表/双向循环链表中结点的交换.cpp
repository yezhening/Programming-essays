#include <iostream>
using namespace std;
typedef int ElemType;
typedef struct Lnode
{
	ElemType data;
	struct Lnode* front;
	struct Lnode* next;
}Lnode,*LinkList;
void InitList(LinkList& L)
{
	L = new Lnode;
	L->front = NULL;
	L->next = NULL;
}
void CreateList(LinkList& L,int n)
{
	LinkList r,p;
	int i;
	r = L;
	for (i = 0; i < n;i++)
	{
		p = new Lnode;
		cin >> p->data;
		p->front = r;
		p->next = NULL;
		r->next = p;
		r = p;
	}
}
int ListLength(LinkList L)
{
	LinkList p;
	p = L->next;
	int i = 0;
	while (p)
	{
		i++;
		p = p->next;
	}
	return i;
}
void  Exchange(LinkList& L)
{
	int n;
	cin >> n;
	if (ListLength(L) != n)
	{
		int i = 1;
		LinkList p;
		p = L->next;
		while (p)
		{
			p = p->next;
			i++;
			if (i == n)
				break;
		}
		LinkList Q, R, S;
		Q = p->front;
		R = p->next;
		S = Q->front;
		Q->front = p;
		p->next = Q;
		p->front = S;
		S->next = p;
		Q->next = R;
		R->front = Q;
	}
	else
	{
		int i = 1;
		LinkList p;
		p = L->next;
		while (p)
		{
			p = p->next;
			i++;
			if (i == n)
				break;
		}
		LinkList Q, S;
		Q = p->front;
		S = Q->front;
		Q->front = p;
		p->next = Q;
		p->front = S;
		S->next = p;
		Q->next = NULL;
	}
}
void Print(LinkList& L)
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
	int length;
	cin >> length;
	while (length!=0)
	{
		LinkList L;
		InitList(L);
		CreateList(L, length);
		Exchange(L);
		Print(L);
		cin >> length;
	}
	return 0;
}