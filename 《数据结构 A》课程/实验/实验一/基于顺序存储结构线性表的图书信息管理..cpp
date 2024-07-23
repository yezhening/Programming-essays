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

typedef struct//图书信息定义
{
	char no[20];//图书序号
	char name[50];//图书名字
	float price;//图书价格
}Book;
typedef struct
{
	Book* elem;//存储空间的基地址
	int length;//图书表中当前图书个数
}SqList;//图书表的顺序存储结构类型为SqList

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
Status PrintList(SqList& L)
{
	int i;
	for (i = 0; i < L.length; i++)
	{
		cout << L.elem[i].no << " " << L.elem[i].name << " " << fixed << setprecision(2) << L.elem[i].price << endl;
	}
	return OK;
}
Status SortList(SqList& L)
{
	int i, j;
	for (i = 0; i < L.length - 1; i++)
	{
		for (j = 0; j < L.length - 1 - i; j++)
		{
			if (L.elem[j].price < L.elem[j + 1].price)
			{
				Book temp;
				temp = L.elem[j];
				L.elem[j] = L.elem[j + 1];
				L.elem[j + 1] = temp;
			}
		}
	}
	return OK;
}
Status ChangeList(SqList& L)
{
	float sum = 0.0, average = 0.0;
	int i;
	for (i = 0; i < L.length; i++)
	{
		sum = sum + L.elem[i].price;
	}
	average = sum / L.length;
	cout << fixed << setprecision(2) << average << endl;
	for (i = 0; i < L.length; i++)
	{
		if (L.elem[i].price <= average)
			L.elem[i].price = L.elem[i].price * 1.2;
		else
			L.elem[i].price = L.elem[i].price * 1.1;
	}
	return OK;
}
Status InvertList(SqList& L)
{
	int i;
	for (i = 0; i < L.length / 2; i++)
	{
		Book B;
		B = L.elem[i];
		L.elem[i] = L.elem[L.length - i - 1];
		L.elem[L.length - i - 1] = B;
	}
	return OK;
}
float MaxPrice(SqList& L)
{
	float max = 0.0;
	int i;
	for (i = 0; i < L.length; i++)
	{
		if (L.elem[i].price > max)
			max = L.elem[i].price;
	}
	return max;
}//和第二个输出函数一起，主函数用长度控制输入
Status PrintList2(SqList& L)
{
	float max = 0.0;
	max = MaxPrice(L);
	int i, count = 0;
	for (i = 0; i < L.length; i++)
	{
		if (L.elem[i].price == max)
		{
			count++;
		}
	}
	cout << count << endl;
	for (i = 0; i < L.length; i++)
	{
		if (L.elem[i].price == max)
		{
			cout << L.elem[i].no << " " << L.elem[i].name << " " << fixed << setprecision(2) << L.elem[i].price << endl;
		}
	}
	return OK;
}
Status FindFavorite(SqList& L)
{
	int findcount = 0, i, j, favoritenum = 0;
	cin >> findcount;
	char lovename[50];
	char flagname[50] = "c";
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
		if (!strcmp(flagname, "c"))
			cout << "Sorry，there is no your favourite!" << endl;
	}
	cout << favoritenum << endl;
	for (i = 0; i < L.length; i++)
	{
		if (!strcmp(L.elem[i].name, flagname))
		{
			cout << L.elem[i].no << " " << L.elem[i].name << " " << fixed << setprecision(2) << L.elem[i].price << endl;
		}
	}
	return OK;
}//不用输出函数
Status FindLocation(SqList& L)//默认查找一次
{
	int findcount = 0, i, j, locationnum = 0;
	cin >> findcount;
	for (i = 0; i < findcount; i++)
	{
		cin >> locationnum;
		if (locationnum<1 || locationnum>L.length)
			cout << "Sorry，the book on the best position doesn't exist!" << endl;//应有break：错
		for (j = 0; j < L.length; j++)
		{
			if (j + 1 == locationnum)//数组从0序号开始
			{
				cout << L.elem[j].no << " " << L.elem[j].name << " " << fixed << setprecision(2) << L.elem[j].price << endl;
			}
		}
	}
	return OK;
}//不用输出函数
Status ListInsert(SqList& L)
{
	int insertlacationnum = 0, j;
	char no[20], name[50];
	float price;
	cin >> insertlacationnum;
	cin >> no >> name >> price;
	if (insertlacationnum<1 || insertlacationnum>L.length + 1)
	{
		cout << "Sorry，the position to be inserted is invalid!" << endl;
		exit(0);
	}
	for (j = L.length - 1; j >= insertlacationnum - 1; j--)
	{
		L.elem[j + 1] = L.elem[j];
	}
	strcpy_s(L.elem[insertlacationnum - 1].no, no);//_s
	strcpy_s(L.elem[insertlacationnum - 1].name, name);
	L.elem[insertlacationnum - 1].price = price;
	L.length++;
	return OK;
}
Status ListDelete(SqList& L)
{
	int deletenum = 0, j;
	cin >> deletenum;
	if (deletenum<1 || deletenum>L.length)
	{
		cout << "Sorry，the position to be deleted is invalid!" << endl;
		exit(0);
	}
	for (j = deletenum; j <= L.length - 1; j++)
	{
		L.elem[j - 1] = L.elem[j];
	}
	L.length--;
	return OK;
}
Status DeleteSame(SqList& L)
{
	int i, j, k;
	for (i = 0; i < L.length; i++)
	{
		for (j = 1; j < L.length; j++)
		{
			if (!strcmp(L.elem[i].no, L.elem[j].no) && i != j)
			{
				for (k = j + 1; k <= L.length - 1; k++)
				{
					L.elem[k - 1] = L.elem[k];
				}
				L.length--;
			}
		}
	}
	return OK;
}//主函数用长度控制输入

int main()
{
	SqList L;
	InitList(L);
	char no[20], name[50];
	float price;
	int length = 0;
	while (1)
	{
		cin >> no >> name >> price;
		if (*no == '0' && *name == '0' && price == 0)
			break;
		else
			CreatList(L, no, name, price);
		length++;
	}
	/*SqList L;
	InitList(L);
	char no[20], name[50];
	float price;
	int length=0,i;
	cin >> length;
	for (i = 0; i < length; i++)
	{
		cin >> no >> name >> price;
		if (*no == '0' && *name == '0' && price == 0)//不需要
			break;
		else
			CreatList(L, no, name, price);
	}*///用长度控制输入
	//cout << length << endl;
	//具体函数
	return 0;
}