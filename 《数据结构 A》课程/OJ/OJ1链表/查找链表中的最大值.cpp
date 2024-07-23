#include <iostream>
using namespace std;
typedef int ElemType;
typedef struct Lnode
{
	ElemType data;
	struct Lnode* next;
}Lnode,*LinkList;
void CreateList(LinkList& L,int n)
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
void Max(LinkList& L)
{
	LinkList p;
	int i;
	int e;
	p = L->next;
	e = p->data;
	while(p)
	{
		if (p->data > e)
			e = p->data;
		p = p->next;
	}
	cout<<e<<endl;
}

int main()
{
	while (1)
	{
		LinkList L;
		int length;
		cin >> length;
		CreateList(L, length);
		Max(L);
	}
	return 0;
}