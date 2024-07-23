#include <iostream>
#include <stdlib.h>
#include<string.h>
#include <iomanip>
using namespace std;
#define OK 1
#define ERROR  0
#define OVERFLOW  -1
#define MAXSIZE 100
typedef int Status;

typedef struct
{
	char no[20];
	char name[50];
	float price;
}Book;
typedef struct LNode
{
	Book data;
	struct LNode* next;
}LNode, * LinkList;

Status InitList(LinkList& L)
{
	L = new LNode;
	L->next = NULL;
	return OK;
}
Status CreatList(LinkList& L, char* no, char* name, float& price)
{
	LinkList  p;//用后插法r=L每次循环都会重置
	p = new LNode;
	strcpy_s(p->data.no, no);//_s
	strcpy_s(p->data.name, name);
	p->data.price = price;
	p->next = NULL;
	L->next = p;
	L = p;
	return OK;
}
Status ListEmpty(LinkList L)
{
	if (L->next)
		return ERROR;
	else
		return OK;
}
int ListLength(LinkList L)
{
	LinkList p;
	int i = 0;
	p = L->next;
	while (p)
	{
		i++;
		p = p->next;
	}
	return i;
}
Status ChangeList(LinkList& L)
{
	LinkList p;
	p = L->next;
	float sum = 0.0, average = 0.0;
	while(p)
	{
		sum = sum+p->data.price;
		p=p->next;//忘记
	}
	average = sum / ListLength(L);
	cout << fixed << setprecision(2) << average << endl;
	p = L->next;//指针回到头
	while(p)
	{
		if (p->data.price <= average)
			p->data.price = p->data.price * 1.2;
		else
			p->data.price = p->data.price * 1.1;
		p = p->next;
	}
	return OK;
}
Status PrintList(LinkList& L)
{
	if (!ListEmpty(L))
	{
		L = L->next;
		while (L)
		{
			cout << (*L).data.no << " " << (*L).data.name << " " << fixed << setprecision(2) << (*L).data.price << endl;
			L = L->next;
		}
	}
	return OK;
}

int main()
{
	LinkList L,head;//因为头结点后面在插入时会移动到尾部
	InitList(L);
	head = L;
	char no[20], name[50];
	float price;
	//int length = 0;
	while (1)
	{
		cin >> no >> name >> price;
		if (*no == '0' && *name == '0' && price == 0)
			break;
		else
		{
			CreatList(L, no, name, price);
			//length++;
		}
	}
	ChangeList(head);
	PrintList(head);
	return 0;
}