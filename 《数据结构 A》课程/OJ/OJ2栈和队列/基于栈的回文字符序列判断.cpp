#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MASSIZE 1000
typedef char SElemType;
typedef int Status;

typedef struct
{
	SElemType* base;
	SElemType* top;
	int stacksize;
}SqStack;
Status InitStack(SqStack& S)
{
	S.base = new SElemType[MASSIZE];
	if (!S.base)
		exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = MASSIZE;
	return OK;
}
Status Push(SqStack& S, SElemType e)
{
	if (S.top - S.base == S.stacksize)
		return ERROR;
	*S.top++ = e;
	return OK;
}
Status Pop(SqStack& S, SElemType& e)
{
	if (S.top == S.base)
		return ERROR;
	e = *--S.top;
	return OK;
}
void Huiwen()
{
	while (1)
	{
		SqStack S;
		InitStack(S);
		string str;
		cin >> str;
		if (str == "0")
			exit(0);
		int i, n = 0;
		n = str.length();
		for (i = 0; i < n / 2; i++)
		{
			Push(S, str[i]);
		}
		if (n % 2!=0)
			i++;
		SElemType t;
		while (i < n)
		{
			SElemType e = 'c';
			Pop(S, e);
			t = e;
			if (t != str[i])
			{
				cout << "NO" << endl;
				break;
			}
			if (i == n - 1)
			{
				cout << "YES" << endl;
				break;
			}
			i++;
		}
	}
}

int main()
{
	Huiwen();
	return 0;
}


