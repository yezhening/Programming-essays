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
Status ListInsert(LinkList& L, int length)
{
	int insertlacationnum = 0, j=1;//1.
	char no[20], name[50];
	float price;
	cin >> insertlacationnum;
	cin >> no >> name >> price;
	if (insertlacationnum<1 || insertlacationnum>length + 1)
	{
		cout << "Sorry，the position to be inserted is invalid!" << endl;
		exit(0);
	}
	LinkList p, s;
	p = L->next;//2.应该让j=0，p=L，这样可以插入在第一个结点位置，否则不可
	s = new LNode;
	strcpy_s(s->data.no , no);
	strcpy_s(s->data.name, name);
	s->data.price = price;
	while(p)
	{
		if (j+1 == insertlacationnum)//3.注意取值会影响到查找，应使p指针置于所查找结点的前一节点
		{
			s->next = p->next;
			p->next = s;
			length++;//未让返回长度
		}
		p = p->next;
		j++;
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
	int length = 0;
	cin >> length;
	int i;
	for(i=0;i<length;i++)
	{
		cin >> no >> name >> price;
		CreatList(L, no, name, price);
	}
	ListInsert(head,length);
	PrintList(head);
	return 0;
}