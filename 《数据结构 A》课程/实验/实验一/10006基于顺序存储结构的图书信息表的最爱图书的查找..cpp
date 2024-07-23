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
Status FindFavorite(SqList& L)
{
	int findcount = 0,i ,j,favoritenum=0;
	cin >> findcount;
	char lovename[50];
	char flagname[50]="c";
	for (i = 0; i < findcount; i++)
	{
		cin >> lovename;
		for (j = 0; j < L.length; j++)
		{
			if (!strcmp(L.elem[j].name, lovename))//strcmp相等返回0
			{
				favoritenum++;
				strcpy_s(flagname, lovename);
			}
		}
		if(!strcmp(flagname,"c"))
			cout << "Sorry，there is no your favourite!" << endl;
	}
	cout << favoritenum << endl;
	for (i = 0; i < L.length; i++)
	{
		if (!strcmp(L.elem[i].name , flagname))
		{
			cout << L.elem[i].no << " " << L.elem[i].name << " " << fixed << setprecision(2) << L.elem[i].price << endl;
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
	FindFavorite(L);
	return 0;
}