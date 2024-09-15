#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

int main()
{
    // 使用输出迭代器写入数据
    std::vector<int> numbers{1, 2, 3, 4, 5};
    std::ostringstream oss{};

    std::ostream_iterator<int> it(oss, "-");
    for (int n : numbers)
    {
        *it = n;
        ++it;
    }
    std::cout << oss.str() << std::endl;

    return 0;
}
/*
输出：
1-2-3-4-5-
*/