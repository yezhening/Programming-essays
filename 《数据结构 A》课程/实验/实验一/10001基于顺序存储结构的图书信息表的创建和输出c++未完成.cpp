#include <iostream>
#include <string>
#include <string.h>
#include <iomanip>
using namespace std;
const int OK = 1;
const int ERROR = 0;
const int OVERFLOw = -1;
const int MAXSIZE = 100;

class Book
{
public:
	string no;
	string name;
	float price;
};
class SqList
{
private:
	Book* elem;
	int length;
public:
	SqList();
	~SqList();
	int ListInsert();
	int GetLength();
	int ShowList();
};
SqList L;

SqList::SqList()
{
	L.elem = new Book[MAXSIZE];
	if (!L.elem)
		exit(OVERFLOw);
	L.length = 0;
}
SqList::~SqList()
{
	if (L.elem)
		delete[]L.elem;
}
int SqList::ListInsert()
{
	SqList L;
	if (L.length + 1)
		return ERROR;
	if (L.length == MAXSIZE)
		return ERROR;
	string no, name;
	float price;
	int i=0;
	while(cin >> no >> name >> price)
	{
		if(no.compare(0)==1&&name.compare(0)==1&&price==0)
			exit(0);
		L.elem[i].no = no;
		L.elem[i].name = name;
		L.elem[i].price = price; 
		i++;
		L.length++;
		return OK;
	}
}
int SqList::GetLength()
{
	cout << L.length;
	return OK;
}
int SqList::ShowList()
{
	int i;
	for (i = 0; i < L.length; i++)
	{
		cout << L.elem[i].no << " " << L.elem[i].name << " " << fixed<<setprecision(2)<<L.elem[i].price << endl;
	}
	return OK;
}

int main()
{
	SqList L;
	L.ListInsert();
	L.GetLength();
	L.ShowList();
	L.~SqList();
	return 0;
}