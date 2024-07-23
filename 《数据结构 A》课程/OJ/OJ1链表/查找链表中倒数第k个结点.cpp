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
void Lookup(LinkList& L,int n,int m)
{
	LinkList p;
	int i;
	p = L->next;
	for (i=1;i<=n-m;i++)
	{
		p= p->next;
	}
	if (p != NULL)
	{
		cout << p->data << endl;
	}
}

int main()
{
	while (1)
	{
		LinkList L;
		int length;
		int k;
		cin >> length;
		CreateList(L, length);
		cin >> k;
		Lookup(L, length, k);
	}
	return 0;
}