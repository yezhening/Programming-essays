#include <vector>
#include <iostream>
#include <algorithm> // 添加<algorithm>头文件用于生成排列

using std::cout;
using std::endl;
using std::vector;

class CPermutation // 修改类名
{
public:
    CPermutation(int n, int m) : n(n), m(m) {}

    bool next()
    {
        // 第一次进入初始化当前排列
        if (this->curPerm.empty())
        {
            for (int i = 0; i < this->m; ++i)
            {
                this->curPerm.push_back(i);
            }

            return true;
        }

        // 使用std::next_permutation来生成下一个排列
        if (std::next_permutation(this->curPerm.begin(), this->curPerm.end()))
        {
            return true;
        }
        else
        {
            return false; // 已经遍历所有排列
        }
    }

    inline void printCurPerm()
    {
        for (const int p : this->curPerm)
        {
            cout << p << " ";
        }
        cout << endl;
    }

private:
    int n;
    int m;

    vector<int> curPerm; // 当前排列
};

int main()
{
    int n = 5;
    int m = 3;
    CPermutation perm(n, m);

    while (perm.next())
    {
        perm.printCurPerm();
    }

    return 0;
}
