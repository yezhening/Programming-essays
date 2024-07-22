#include<iostream>
#include<cstring>
using namespace std;

class bign
{
public:
	bign();
	~bign();
	bign operator=(char *p);
	bign operator = (int p);
	bign& operator+=(const bign&);
	friend bign operator+(const bign&, const bign&);
	friend bign operator-(const bign&, const bign&);
	friend bign operator*(const bign&, const bign&);
	friend bign operator/(const bign&, const bign&);
	friend bool operator<(const bign&, const bign&);
	friend bool operator>(const bign&, const bign&);
	friend bool operator<=(const bign&, const bign&);
	friend bool operator>=(const bign&, const bign&);
	friend bool operator==(const bign&, const bign&);
	friend bool operator!=(const bign&, const bign&);
	friend ostream& operator<<(ostream&, const bign&);
	friend istream& operator>>(istream&, int j);
private:
	int length;
	int number[500];
};

bign::bign()
{
	length = 0;
}
bign::~bign()
{
	length = 0;
}
bign bign::operator=(char* p)
{
	length = strlen(p);
	int i;
	for (i = 0; i < length; i++);
		number[i] = p[i];
	return *this;
}
 bign bign::operator = (int p)
 {
	 
	 return *this;
 }
 ostream& operator<<(ostream& i, const Complex& j)
 {
	 i << j.x << " " << "+" << " " << j.y << "i";
	 return i;
 }
 istream& operator>>(istream& i, int j)
 {
	 i >> j;
	 return i;
 }


int main() 
{
	bign a, b, c;
	a = "123456789123456789987654321999999999999999999999999";
	cin >> b;
	c = 12345;
	cout << (a + b) << endl;
	cout << (a - b) << endl;
	cout << (a * b) << endl;
	cout << (a / b) << endl;
	cout << (a < b) << endl;
	cout << (a > c) << endl;
	a += b;
	cout << a << endl;
	cout << (a <= b) << endl;
	cout << (a >= c) << endl;
	a += c;
	cout << a << endl;
	cout << (a == b) << endl;
	cout << (a != c) << endl;
	return 0;
}