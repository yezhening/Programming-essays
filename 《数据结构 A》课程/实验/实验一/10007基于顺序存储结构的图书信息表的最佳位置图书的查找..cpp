#include <iostream>
#include <stdlib.h>
#include<string.h>
#include <iomanip>
using namespace std;
#define OK 1
#define ERROR  0
#define OVERFLOW  -1
#define MAXSIZE  100
typedef int Status;

typedef struct
{
	char no[20];
	char name[50];
	float price;
}Book;
typedef struct
{
	Book* elem;
	int length;
}SqList;

Status InitList(SqList& L)
{
	L.elem = new Book[MAXSIZE];//注意
	if (!L.elem)
		exit(OVERFLOW);
	L.length = 0;
	return OK;
}
Status CreatList(SqList& L, char* no, char* name, float& price)
{
	Book B;
	strcpy_s(B.no, no);//_s
	strcpy_s(B.name, name);
	B.price = price;
	L.elem[L.length] = B;//注意
	L.length++;
	return OK;
}
Status FindLocation(SqList& L)//默认查找一次
{
	int findcount = 0,i ,j,locationnum=0;
	cin >> findcount;
	for (i = 0; i < findcount; i++)
	{
		cin >> locationnum;
		if (locationnum<1|| locationnum>L.length)
			cout << "Sorry，the book on the best position doesn't exist!" << endl;//应有break：错
		for (j = 0; j < L.length; j++)
		{
			if (j+1 == locationnum)//数组从0序号开始
			{
				cout << L.elem[j].no << " " << L.elem[j].name << " " << fixed << setprecision(2) << L.elem[j].price << endl;
			}
		}
	}
	return OK;
}

int main()
{
	SqList L;
	InitList(L);
	char no[20], name[50];
	float price;
	int length = 0, i;
	cin >> length;
	for (i = 0; i < length; i++)
	{
		cin >> no >> name >> price;
		CreatList(L, no, name, price);
	}
	FindLocation(L);
	return 0;
}