#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
#define MAXSIZE 100
#define OK 1;
#define ERROR 0;
#define OVERFLOW -1
typedef int Status;
char op[7] = { '+', '-', '*', '/', '(', ')', '=' };//运算符数组

typedef struct
{
	char* base;
	char* top;
	int stacksize;

}SqStackOPTR;

typedef struct
{
	double* base;
	double* top;
	int stacksize;

}SqStackOPND;

Status InitStack(SqStackOPTR& S)
{
	S.base = new char[MAXSIZE];
	if (!S.base) return 0;
	S.top = S.base;
	S.stacksize = MAXSIZE;
	return OK;
}

Status InitStack(SqStackOPND& S)
{
	S.base = new double[MAXSIZE];
	if (!S.base) return 0;
	S.top = S.base;
	S.stacksize = MAXSIZE;
	return OK;
}

Status Push(SqStackOPTR& S, char e)
{
	if (S.top - S.base == S.stacksize) 
		return ERROR;
	*S.top++ = e;
	return OK;
}

Status Push(SqStackOPND& S, double e)
{
	if (S.top - S.base == S.stacksize) 
		return ERROR;
	*S.top++ = e;
	return OK;
}

Status Pop(SqStackOPTR& S, char& e)
{
	if (S.top == S.base) 
		return ERROR;
	e = *--S.top;
	return OK;
}

Status Pop(SqStackOPND& S, double& e)
{
	if (S.top == S.base)	
		return ERROR;
	e = *--S.top;
	return OK;
}

char GetTop(SqStackOPTR S)
{
	if (S.top != S.base)
		return *(S.top - 1);
}

double GetTop(SqStackOPND S)
{
	if (S.top != S.base)
		return *(S.top - 1);
}

Status In(char ch) //判断ch是否为运算符
{
	int i;
	for (i = 0; i < 7; i++) 
	{
		if (ch == op[i]) 
		{
			return OK;
		}
	}
	return ERROR;
}

char Precede(char c1, char c2)
{
	if ((c1 == '(' && c2 == ')') || (c1 == '=' && c2 == '='))
		return '=';
	else if (((c1 == '+' || c1 == '-' || c1 == '*' || c1 == '/' || c1 == ')') && (c2 == '+' || c2 == '-' || c2 == ')' || c2 == '=')) || ((c1 == '*' || c1 == '/' || c1 == ')') && (c2 == '*' || c2 == '/')))
		return '>';
	else if (((c1 == '(' || c1 == '=') && c2 != ')' && c2 != '=') || ((c1 == '+' || c1 == '-') && (c2 == '*' || c2 == '/')) || c1 == '(' || c2 == '(') 
		return '<';
	else
		cout << c1 << " " << c2 << "没有输出" << endl;
}

double Operate(double d1, char theta, double d2) //计算两数运算结果
{
	switch (theta) 
	{
	case '+':
		return d1 + d2;
	case '-':
		return d1 - d2;
	case '*':
		return d1 * d2;
	case '/':
		return d1 / d2;
	}
	return 0;//必须要
}


double EvaluateExpression(char ch) //算术表达式求值的算符优先算法
{
	SqStackOPTR OPTR;
	SqStackOPND OPND;
	InitStack(OPTR);
	InitStack(OPND);
	Push(OPTR, '=');
	while (ch != '=' || (GetTop(OPTR) != '=')) 
	{
		char flag = '+';
		if (ch == '-')
		{
			flag = ch;
		}
		if (!In(ch))
		{
			double m = 0, n = 0;
			while (ch != '.' && ch >= '0' && ch <= '9')
			{
				m = m * 10 + (ch - 48);//ACALL
				cin >> ch;
			}
			if (ch == '.')
				cin >> ch;
			int k = 1;
			while (ch >= '0' && ch <= '9')
			{
				n = n + (ch - 48) * pow(10.0, -k);
				k++;
				cin >> ch;
			}
			if (flag == '+')
			{
				Push(OPND, m+n);
			}
			else
			{
				Push(OPND, -(m + n)); 
			}
		}
		else
			switch (Precede(GetTop(OPTR), ch)) 
			{
			case '<':
				Push(OPTR, ch);
				cin >> ch; 
				break;
			case '>':
				char theta; 
				double a, b;
				Pop(OPTR, theta); 
				Pop(OPND, b);
				Pop(OPND, a); 
				Push(OPND, Operate(a, theta, b)); 
				break;
			case '=': 
				char x;
				Pop(OPTR, x);
				cin >> ch; 
				break;
			}
	}
	return GetTop(OPND);
}

int main()
{
	while (1)
	{
		char ch;
		cin >> ch;
		if (ch == '=')
			break;
		double result;
		result = EvaluateExpression(ch);
		cout << fixed << setprecision(2) << result << endl;
	}
	return 0;
}