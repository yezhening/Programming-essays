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
Status PrintList(LinkList& L)//改进：用p指向结点去循环，见下个输出函数
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
Status PrintList2(LinkList& L)//改进的
{
	if (!ListEmpty(L))
	{
		LinkList p;
		p = L->next;
		while (p)
		{
			cout << (*p).data.no << " " << (*p).data.name << " " << fixed << setprecision(2) << (*p).data.price << endl;
			p = p->next;
		}
	}
	return OK;
}
Status SortList(LinkList& L)
{
	LinkList p;
	int i, j;
	for (i = 0; i < ListLength(L) - 1; i++)
	{
		p = L->next;
		if (p)
		{
			for (j = 0; j < ListLength(L) - 1 - i; j++)
			{
				if (p->data.price < p->next->data.price)
				{
					Book temp;
					temp = p->data;
					p->data = p->next->data;
					p->next->data = temp;
				}
				p = p->next;
			}
		}
	}
	return OK;
}
Status ChangeList(LinkList& L)
{
	LinkList p;
	p = L->next;
	float sum = 0.0, average = 0.0;
	while (p)
	{
		sum = sum + p->data.price;
		p = p->next;//忘记
	}
	average = sum / ListLength(L);
	cout << fixed << setprecision(2) << average << endl;
	p = L->next;//指针回到头
	while (p)
	{
		if (p->data.price <= average)
			p->data.price = p->data.price * 1.2;
		else
			p->data.price = p->data.price * 1.1;
		p = p->next;
	}
	return OK;
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
}//用输出函数2
float MaxPrice(LinkList& L)
{
	float max = 0.0;
	LinkList p;
	p = L->next;
	while (p)
	{
		if (p->data.price > max)
		{
			max = p->data.price;
		}
		p = p->next;
	}
	return max;
}//只用输出函数3
Status PrintList3(LinkList& L)
{
	float max = 0.0;
	max = MaxPrice(L);
	if (!ListEmpty(L))
	{
		LinkList p;
		p = L->next;
		int count = 0;
		while (p)
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
}
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
		while (p)
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
}//不用输出函数
Status FindLocation(LinkList& L, int length)
{
	int findcount = 0, i, j = 0, locationnum = 0;
	cin >> findcount;
	LinkList p;
	p = L->next;
	for (i = 0; i < findcount; i++)
	{
		cin >> locationnum;
		if (locationnum<1 || locationnum>length)
			cout << "Sorry，the book on the best position doesn't exist!" << endl;
		while (p)
		{
			if (j + 1 == locationnum)//数组从0序号开始
			{
				cout << p->data.no << " " << p->data.name << " " << fixed << setprecision(2) << p->data.price << endl;
			}
			p = p->next;
			j++;
		}
	}
	return OK;
}//不用输出函数
Status ListInsert(LinkList& L, int length)
{
	int insertlacationnum = 0, j = 1;//1.
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
	strcpy_s(s->data.no, no);
	strcpy_s(s->data.name, name);
	s->data.price = price;
	while (p)
	{
		if (j + 1 == insertlacationnum)//3.注意取值会影响到查找，应使p指针置于所查找结点的前一节点
		{
			s->next = p->next;
			p->next = s;
			length++;//未让返回长度
		}
		p = p->next;
		j++;
	}
	return OK;
}//用长度控制输出，两个变量，第二个变量用length
Status ListDelete(LinkList& L, int length)
{
	int deletenum = 0, j = 0;
	cin >> deletenum;
	if (deletenum<1 || deletenum>length)
	{
		cout << "Sorry，the position to be deleted is invalid!" << endl;
		exit(0);
	}
	LinkList p;
	p = L;
	while (p)
	{
		if (j + 1 == deletenum)
		{
			LinkList q;
			q = p->next;
			p->next = q->next;
			delete q;
			length--;//未让返回长度
		}
		p = p->next;
		j++;
	}
	return OK;
}//用长度控制输出，两个变量，第二个变量用length
Status DeleteSame(LinkList& L, int length)
{
	LinkList p;
	p = L->next;
	int i, j;
	for (i = 1; i <= length; i++)//给定pq存在才循环
	{
		LinkList q;
		q = p->next;
		for (j = 2; q && j <= length; j++)
		{
			if (!strcmp(p->data.no, q->data.no))
			{
				LinkList t;
				t = q;
				q = q->next;
				p->next = q;
				delete t;
				length--;
			}
			q = q->next;
		}
		p = p->next;
	}
	cout << length << endl;
	return OK;
}//用长度控制输出，两个变量，第二个变量用length

int main()
{
	LinkList L, head;//因为头结点后面在插入时会移动到尾部
	InitList(L);
	head = L;
	char no[20], name[50];
	float price;
	int length = 0;
	while (1)
	{
		cin >> no >> name >> price;
		if (*no == '0' && *name == '0' && price == 0)
			break;
		else
		{
			CreatList(L, no, name, price);
			length++;
		}
	}
	/*LinkList L, head;//因为头结点后面在插入时会移动到尾部
	InitList(L);
	head = L;
	char no[20], name[50];
	float price;
	int length = 0;
	cin >> length;
	int i;
	for (i = 0; i < length; i++)
	{
		cin >> no >> name >> price;
		CreatList(L, no, name, price);
	}*///用长度控制输入
	//cout << length << endl;
	//具体函数，如SortList(head);
	//PrintList(head);
	return 0;
}