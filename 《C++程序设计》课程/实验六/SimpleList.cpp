#include <iostream>
#include <string>
#include <list>
#include<algorithm>
using namespace std;
struct Student
{
    int no;
    string name;
};


void Input(std::list<Student>& li)
{
    Student s;
    int n;
    std::cin >> n;
    for (std::list<Student>::size_type i = 0; i < n; i++)
    {
        std::cin >> s.no;
        std::cin >> s.name;
        li.push_back(s);
    }
}
void Show(const std::list<Student>& li)
{
    for (auto it = li.begin(); it != li.end(); it++)
    {
        std::cout << (*it).no << ", " << (*it).name;
        printf("\n");
    }
}

int main()
{

    std::list<Student> li;
    Input(li); //输入
    Show(li); //输出

    return 0;
}
