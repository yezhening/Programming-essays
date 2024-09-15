#include <sstream>
#include <iterator>
#include <iostream>

int main()
{
    // 使用输入迭代器读取数据
    std::istringstream iss("1 2 3 4 5");

    std::istream_iterator<int> it(iss);
    std::istream_iterator<int> end{};
    while (it != end)
    {
        std::cout << *it << " ";
        ++it;
    }

    return 0;
}
/*
输出：
1 2 3 4 5
*/