#include <iostream>
using namespace std;
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
void CreateList(LinkList& L,int n)
{
	LinkList r,p;
	int i;
	L = new Lnode;
	L->next = NULL;
	r = L;
	for (i = 0; i < n;i++)
	{
		p = new Lnode;
		cin >> p->data;
		p->next = NULL;
		r->next = p;
		r = p;
	}
}
void MergeList_L(LinkList& LA, LinkList& LB, LinkList& LC)
{
	LinkList pa,pb,pc;
	pa = LA->next;
	pb = LB->next;
	LC = LA;
	pc = LC;
	while (pa && pb)
	{
		if (pa->data <= pb->data)
		{
			pc->next = pa;
			pc = pa;
			pa = pa->next;
		}
		else
		{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}	
	}
	pc->next = pa ?  pa: pb;
	delete LB;
}
void Inverse(LinkList& L)
{
	LinkList p, q;
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
	int lengtha,lengthb;
	cin >> lengtha>>lengthb;
	while (lengtha != 0 && lengthb != 0)
	{
		LinkList LA, LB, LC;
		CreateList(LA, lengtha);
		CreateList(LB, lengthb);
		InitList(LC);
		MergeList_L(LA, LB, LC);
		Inverse(LC);
		Print(LC);
		cin >> lengtha >> lengthb;
	}
	return 0;
}