#include <iostream>
#include <cstring>
using namespace std;

class Date
{
public:
   Date(int d = 0, int m = 0, int y = 0); //构造函数
   int get_day() const; // 返回day
   int get_month() const; //返回month
   int get_year() const; // 返回year
   static void set_default(int, int, int); //设置default_date
   static int get_default_day(); //返回缺省day
   static int get_default_month(); //返回缺省month
   static int get_default_year(); //返回缺省year
   Date & add_year(int n); //加n年
   Date & add_month(int n); //加n月，考虑超过12月
   Date & add_day(int n); //加n天，考虑进位月和年，考虑闰年
private:
   int day, month, year;
   static Date default_date; //初始化为 1901年1月1日
};

Date::Date(int d, int m, int y)
{
	if(d!=0&&m!=0&&y!= 0)
	{
		day=d;
		month=m;
		year=y;		
	}
}
int Date::get_day() const
{
	return day;
}
int Date::get_month() const
{
	return month;	
}
int Date::get_year() const
{
	return year;
}
void Date::set_default(int x, int y, int z)
{
	Date::default_date=Date(x,y,z);
}
int Date::get_default_day()
{
	return default_date.get_day();
}
int Date::get_default_month() 
{
	return default_date.get_month();
}
int Date::get_default_year()
{
	return default_date.get_year();
}
Date & Date::add_year(int n)
{
	this->year=year+n;
	return *this;
}
int judge(int y,int m)
{
    switch(m)
    {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 2:
			{
				if(y%4==0&&y%100!=0||y%400==0)
				{
					return 29;
				}
				else
				{
					return 28;
				}
			}
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
    }
}
Date & Date::add_month(int n)
{
	month=month+n;
	while(month>12)
	{
		this->year=year+1;
		this->month=month-12;
	}
	while(day>judge(year,month))
	{
		this->day=day-judge(year,month);
		this->month=month+1;
	}	
	return *this;
}

Date & Date::add_day(int n)
{
    day=day+n;
	while(day>judge(year,month))
    {
		day=day-judge(year,month);
		this->month=month+1;
		while(month>12)
		{
			this->year=year+1;
			this->month=month-12;
		}
    }
    return *this;
}
Date Date::default_date(1,1,1901);

int main()
{
    char type[110];
    int day,mon,year;
    int addday,addmon,addyear;
    while(cin>>type)
    {
       if(strcmp(type,"Date") == 0)
       {
            cin>>day>>mon>>year;
            Date mydate(day,mon,year);
            cin>>addday>>addmon>>addyear;
            mydate.add_day(addday).add_month(addmon).add_year(addyear);
            cout << mydate.get_day() << " " << mydate.get_month() << " " << mydate.get_year() << endl;
       }
       else if(strcmp(type,"defaultDate") == 0)
       {
           cout << Date::get_default_day() << " " << Date::get_default_month() << " "<< Date::get_default_year() << endl;
       }
       else if(strcmp(type,"setdefaultDate") == 0)
       {
           cin>>day>>mon>>year;
           Date::set_default(day,mon,year);
           cout << Date::get_default_day() << " " << Date::get_default_month() << " " << Date::get_default_year() << endl;
       }
    }
    return 0;
}