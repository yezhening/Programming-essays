#include <iostream>
#include <cstring>
using namespace std;
class Book
{
private:
   char *name;                //书名
   char *author;              //作者
   int sale;                  //销售量
public:
   Book();                               //无参构造函数
   Book(char *a, char *b, int c);         //有参构造函数
   Book(const Book &);                   //拷贝构造函数
   void print();                          //显示数据
   ~Book();                              //析构函数
};

Book::Book()
{
	name=new char[100];
	strcpy(name,"No name");
	author=new char[100];
	strcpy(author,"No author");
	sale=0;
}
Book::Book(char *a, char *b, int c)
{
	name=new char[strlen(a)+1];
	strcpy(name,a);
	author=new char[strlen(b)+1];
	strcpy(author,b);
	if(!name||!author)
	{
		cout<<"Out of memory"<<endl;
	}
	sale=c;
}
Book::Book(const Book &p)
{
	name=new char[strlen(p.name)+1];
	author=new char[strlen(p.author)+1];
	strcpy(name,p.name);
	strcpy(author,p.author);
	if(!name||!author)
	{
		cout<<"Out of memory"<<endl;
	}
	sale=p.sale;
}
Book::~Book()
{
	delete[] name;
	delete[] author;
}
void Book::print()
{
	cout<<"Name: "<<name<<"\t"<<"Author: "<<author<<"\t"<<"Sale: "<<sale<<"\n";
}

int main()
{
	char n[100];
	char a[100];
	int sale;
    cin.getline(n,sizeof(n));
	cin.getline(a,sizeof(a));
	cin>>sale;
	if((strcmp(n,"-1")==0)&&(strcmp(a,"-1")==0)&&(sale==-1))
	{
		Book bk1;
		bk1.print();
	}
	else if((strcmp(n,"0")==0)&&(strcmp(a,"0")==0)&&(sale==0))
	{
		Book bk1;
		Book bk2(bk1);
		bk2.print();
	}
	else
	{
		Book bk1(n,a,sale);
		bk1.print();
	}
    return 0;
}