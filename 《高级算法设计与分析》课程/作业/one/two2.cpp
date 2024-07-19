#include <iostream>
#include <vector>

// 递归函数，用于生成所有 m 个元素的排列
void generatePermutations(std::vector<int> &currentPermutation, std::vector<bool> &used, int m)
{
    // 如果当前排列已包含 m 个元素，打印它并返回
    if (currentPermutation.size() == m)
    {
        for (int i : currentPermutation)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        return;
    }

    // 遍历所有可能的元素
    for (int i = 1; i <= used.size(); ++i)
    {
        if (!used[i - 1])
        {                                                      // 如果元素尚未被使用
            used[i - 1] = true;                                // 标记该元素已使用
            currentPermutation.push_back(i);                   // 将该元素添加到当前排列中
            generatePermutations(currentPermutation, used, m); // 递归生成下一个位置的元素
            currentPermutation.pop_back();                     // 撤销选择，准备尝试其他可能的元素
            used[i - 1] = false;                               // 标记该元素未使用
        }
    }
}

int main()
{
    int n, m;
    std::cout << "Enter the total number of elements (n): ";
    std::cin >> n;
    std::cout << "Enter the number of elements to select (m): ";
    std::cin >> m;

    if (m > n)
    {
        std::cout << "Error: m should be less than or equal to n." << std::endl;
        return 1;
    }

    std::vector<int> currentPermutation; // 用于存储当前排列的向量
    std::vector<bool> used(n, false);    // 用于跟踪哪些元素已经被使用

    std::cout << "All permutations of " << m << " elements from " << n << " elements:" << std::endl;

    generatePermutations(currentPermutation, used, m); // 调用排列生成函数

    return 0;
}
