#include<iostream>
#include<cstring>
using namespace std;

class String
{
private:
	char* s;
public:
	String();
	String(const char*);
	String(const String&);
	~String();
	String& operator=(const char*);
	String& operator=(const String&);
	String operator+(const char*);
	String operator+(const String&);
	String& operator+=(const char*);
	String& operator+=(const String&);
	friend istream& operator>>(istream&, String&);
	friend ostream& operator<<(ostream&, const String&);
	friend bool operator==(const String&, const char*);
	friend bool operator==(const String&, const String&);
	friend bool operator!=(const String&, const char*);
	friend bool operator!=(const String&, const String&);
};

String::String()
{
	s = new char[1000];
	strcpy(s, "\0");
}
String::String(const char* p)
{
	s = new char[strlen(p) + 1];
	strcpy(s, p);
}
String::String(const String& p)
{
	s = new char[strlen(p.s) + 1];
	strcpy(s, p.s);
}
String::~String()
{
	delete[] s;
}
String& String::operator=(const char* p)
{
	delete[] s;
	s = new char[strlen(p) + 1];
	strcpy(s, p);
	return *this;
}
String& String::operator=(const String& p)
{
	delete[] s;
	s = new char[strlen(p.s) + 1];
	strcpy(s, p.s);
	return *this;
}
String String::operator+(const char* p)
{
	char* t;
	t = new char[strlen(s) + strlen(p) + 1];
	strcpy(t, this->s);
	strcat(t, p);
	return t;
}
String String::operator+(const String& p)
{
	char* t;
	t = new char[strlen(s) + strlen(p.s) + 1];
	strcpy(t, s);
	strcat(t, p.s);
	return t;
}
String& String::operator+=(const char* p)
{
	char* t;
	t = new char[strlen(s) + strlen(p) + 1];
	strcpy(t, s);
	strcat(t, p);
	delete[] s;
	s= new char[strlen(s) + strlen(p) + 1];
	strcpy(s, t);
	return *this;
}
String& String::operator+=(const String& p)
{
	char* t;
	t = new char[strlen(s) + strlen(p.s) + 1];
	strcpy(t, s);
	strcat(t, p.s);
	delete[] s;
	s = new char[strlen(s) + strlen(p.s) + 1];
	strcpy(s, t);
	return *this;
}
istream& operator>>(istream& i, String& j)
{
	i >> j.s;
	return i;
}
ostream& operator<<(ostream& i, const String& j)
{
	i << j.s;
	return i;
}
bool operator==(const String& i, const char* j)
{
	if (strcmp(i.s, j) == 0)
		return true;
	return false;
}
bool operator==(const String& i, const String& j)
{
	if (strcmp(i.s ,j.s)==0)
		return true;
	return false;
}
bool operator!=(const String& i, const char* j)
{
	if (strcmp(i.s , j) != 0)
		return true;
	return false;
}
bool operator!=(const String& i, const String& j)
{
	if (strcmp(i.s ,j.s)!=0)
		return true;
	return false;
}

int main()
{
	String s;
	s += "hello";
	cout << s << endl;
	String s1("String1");
	String s2("copy of ");
	s2 += "String1";
	cout << s1 << "\n" << s2 << endl;
	String s3;
	cin >> s3;
	cout << s3 << endl;
	String s4("String4"), s5(s4);
	cout << (s5 == s4) << endl;
	cout << (s5 != s4) << endl;
	String s6("End of "), s7("my string.");
	s6 += s7;
	cout << s6 << endl;
	return 0;
}

