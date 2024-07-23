#include <iostream>
#include <stdlib.h>
using namespace std;
#define OK 1
#define ERROR 0
#define OVERFLOW -1
typedef int SElemType;
typedef int Status;

typedef struct
{
	SElemType top[2],bot[2];
	SElemType* V;
	SElemType m;
}DblStack;
Status InitStack(DblStack& S,int n)
{
	S.m = n;
	S.V = new SElemType[n];
	S.top[0] = -1;
	S.bot[0] = -1;
	S.top[1] = S.m;
	S.bot[1] = S.m ;
	return OK;
}
Status Isempty0(DblStack& S)
{
	if (S.top[0] == -1)
		return ERROR;
	else
		return OK;
}
Status Isempty1(DblStack& S)
{
	if (S.top[1] == S.m)
		return ERROR;
	else
		return OK;
}
Status IsFull(DblStack& S)
{
	if (S.top[0]+1== S.top[1])
		return OK;
	else
		return ERROR;
}
Status Push0(DblStack& S,SElemType e)
{
	if (S.top[0] + 1 == S.top[1])
		return ERROR;
	S.top[0]++;
	S.V[S.top[0]]=e;
	return OK;
}
Status Push1(DblStack& S, SElemType e)
{
	if (S.top[0] + 1 == S.top[1])
		return ERROR;
	S.top[1]--;
	S.V[S.top[1]] = e;
	return OK;
}
Status Pop0(DblStack& S, SElemType &e)
{
	if (S.top[0] == -1 && S.top[1] == S.m)
		return ERROR;
	else
	{
		e=S.V[S.top[0]];
		S.top[0]--;
	}
	return OK;
}
Status Pop1(DblStack& S, SElemType& e)
{
	if (S.top[0] == -1 && S.top[1] == S.m)
		return ERROR;
	else
	{
		e=S.V[S.top[1]];
		S.top[1]++;
	}
	return OK;
}

int main()
{
	while (1)
	{
		DblStack S;
		int length;
		cin >> length;
		if (length == 0)
			exit(0);
		InitStack(S, length);
		int e0, e1, d0, d1;
		cin >> e0 >> e1 >> d0 >> d1;
		int i,e;
		for (i = 0; i < e0; i++)
		{
			cin >> e;
			Push0(S, e);
		}
		for (i = 0; i < e1; i++)
		{
			cin >> e;
			Push1(S,e);
		}
		cout << IsFull(S) << endl;
		for (i = 0; i < d0; i++)
		{
			Pop0(S,e);
			cout << e<<" ";
		}
		cout<<Isempty0(S)<<endl;
		for (i = 0; i < d1; i++)
		{
			Pop1(S,e);
			cout << e<<" ";
		}
		cout << Isempty1(S) << endl;
	}
	return 0;
}
