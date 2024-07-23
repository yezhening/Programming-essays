#include <iostream>
#include <stdlib.h>
using namespace std;
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define MASSIZE 100
typedef int SElemType;
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
SElemType GetTop(SqStack S)//未用到
{
	if (S.top != S.base)
		return *(S.top - 1);
}
void InOutS()
{
	int length = 0;
	SqStack S;
	while (1)
	{
		InitStack(S);//在每次循环都创建一次
		cin >> length;
		if (length == 0)
			exit(0);
		int i;
		int num = 0;
		int flag = 0;
		for (i = 0; i < length; i++)
		{
			cin >> num;
			if (num != -1 && flag == 0)
				Push(S, num);
			else
			{
				if (flag == 0)
				{
					int e = 0;
					if (Pop(S, e))//未设置循环情况,不是弹出num
					{
						cout << e << endl;
					}
					else
					{
						cout << "POP ERROR" << endl;
						flag = 1;
					}
				}
			}
		}
	}
}

int main()
{
	InOutS();
	return 0;
}


