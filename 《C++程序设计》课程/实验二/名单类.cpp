#include<iostream>
#include <string>
using namespace std;

class Date    //日期类
{
private:
    int Date_year;    //年
    int Date_month;    //月
    int Date_day;    //日
public:
    Date(int year=2000, int month=1, int day=1);
    void show();    //以“年-月-日”格式输出年月日
    ~Date();
};

class Croster    //名单类
{
private:
    string name;
    Date birthday;
public:
    Croster();
    Croster(string name,int year,int month,int day);
    Croster(string name, Date date);
    void show();//显示姓名和出生日期
    ~Croster();
};

Date::Date(int year,int month, int day)
{
	Date_year=year;
	Date_month=month;
	Date_day=day;
}
void Date::show()
{
	cout<<Date_year<<"-"<<Date_month<<"-"<<Date_day<<endl;
}
Date::~Date()
{}
Croster::Croster():birthday(0,0,0)
{
	name="NULL";
}
Croster::Croster(string name,int year,int month,int day):birthday(year,month,day)
{
	this->name=name; 
}
Croster::Croster(string name,Date date):birthday(date)
{
	this->name=name; 
}
void Croster::show()
{
	cout<<"Name: "<<name<<", "<<"Birthday: ";
	birthday.show();
}
Croster::~Croster()
{}

int main()
{
	while(1)
	{
		int i;
		cin>>i;
		if(i==0)
		{
			Croster p;
			p.show();
		}
		else if(i==1)
		{
			string n;
			int y,m,d;
			cin>>n>>y>>m>>d;
			Croster p(n,y,m,d);
			p.show();
		}
		else if(i==2)
		{
			string n;
			int y,m,d;
			cin>>n>>y>>m>>d;
			Date birthday(y,m,d);
			Croster p(n,birthday);
			p.show();
		}
		else
		{
			exit(0);
		}
	}	
	return 0;
}
