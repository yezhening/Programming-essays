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
/*int ListLength(LinkList L)
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
}*/
/*float MaxPrice(LinkList& L)
{
	float max = 0.0;
	LinkList p;
	p = L->next;
	while(p)
	{
		if (p->data.price > max)
		{
			max = p->data.price;
		}
		p = p->next;
	}
	return max;
}
Status InvertList(LinkList& L)
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
	return OK;
}
Status PrintList(LinkList& L)
{
	float max = 0.0;
	max = MaxPrice(L);
	if (!ListEmpty(L))
	{
		LinkList p;
		p = L->next;
		int count = 0;
		while(p)
		{
			if (p->data.price == max)
			{
				count++;
			}
			p = p->next;
		}
		cout << count << endl;
		p = L->next;
		while (p)
		{
			if (p->data.price == max)
			{
				cout << (*p).data.no << " " << (*p).data.name << " " << fixed << setprecision(2) << (*p).data.price << endl;
			}
			p = p->next;
		}
	}
	return OK;
}*/
Status FindFavorite(LinkList& L)
{
	int findcount = 0, i, favoritenum = 0;
	cin >> findcount;
	LinkList p;
	char lovename[50];
	char flagname[50] = "c";
	for (i = 0; i < findcount; i++)
	{
		cin >> lovename;
		p = L->next;
		while(p)
		{
			if (!strcmp(p->data.name, lovename))//strcmp相等返回0
			{
				favoritenum++;
				strcpy_s(flagname, lovename);
			}
			p = p->next;//老是忘记
		}
		if (!strcmp(flagname, "c"))
		{
			cout << "Sorry，there is no your favourite!" << endl;
		}
	}
	cout << favoritenum << endl;
	p = L->next;
	while (p)
	{
		if (!strcmp(p->data.name, flagname))
		{
			cout << p->data.no << " " << p->data.name << " " << fixed << setprecision(2) << p->data.price << endl;
		}
		p = p->next;
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
	int length = 0;
	cin >> length;
	int i;
	for(i=0;i<length;i++)
	{
		cin >> no >> name >> price;
		CreatList(L, no, name, price);
	}
	//InvertList(head);
	//PrintList(head);
	FindFavorite(head);
	return 0;
}