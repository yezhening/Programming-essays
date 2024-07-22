#include <iostream>
#include <string>
#include<algorithm>
using namespace std;
bool cmp(char i, char j)
{
	return i > j;
}
int main()
{
	string s;
	cin >> s;
	sort(s.begin(),s.end(),cmp);
	cout << s << endl;
	return 0;
}
