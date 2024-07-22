#include <iostream>
#include <string>
#include <vector>
#include<algorithm>
using namespace std;
bool cmp(string i, string j)
{
	return i > j;
}
int main()
{
	int N;
	cin >> N;
	vector<string> v(N);
	for (vector<string>::size_type i = 0; i < N; i++)
	{
		cin>>v[i];
		sort(v[i].begin(), v[i].end());
	}
	sort(v.begin(), v.end(), cmp);
	for (vector<string>::size_type i = 0; i < N; i++)
	{
		cout << v[i]<<endl;
	}
	return 0;
}
