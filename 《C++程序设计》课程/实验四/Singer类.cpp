#include<iostream>
#include<string>
#include<cstring>
using namespace std;
class Singer
{
public:
	Singer();
	string getName();
	friend istream& operator>>(istream&, Singer& k);
	friend ostream& operator<<(ostream&, Singer& k);
	friend bool operator>(Singer& i, Singer& j);
	friend bool operator==(Singer& i, Singer& j);
private:
	string name;
	string sex;
	int age;
	double score;
};

Singer::Singer()
{}
string Singer::getName()
{
	return name;
}
bool operator>(Singer& i, Singer& j)
{
	if (i.score > j.score)
		return true;
	return false;
}
bool operator==(Singer& i, Singer& j)
{
	if (i.score == j.score)
		return true;
	return false;
}
istream& operator>>(istream& m, Singer& k)
{
	m >> k.name >> k.sex >> k.age >> k.score;
	return m;
}
ostream& operator<<(ostream& n, Singer& k)
{
	n << k.name << " " << k.sex << " " << k.age << " " << k.score;
	return n;
}


int main()
{
	Singer s1, s2;
	cin >> s1 >> s2;
	cout << s1 << "\n" << s2 << endl;
	if (s1 > s2)
		cout << s1.getName() << "'s score is higher than " << s2.getName() << "'s.\n";
	else if (s1 == s2)
		cout << s1.getName() << "'s score is equal to " << s2.getName() << "'s.\n";
	else
		cout << s1.getName() << "'s score is lower than " << s2.getName() << "'s.\n";
	return 0;
}