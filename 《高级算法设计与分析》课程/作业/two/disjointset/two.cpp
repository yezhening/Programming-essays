#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_map>
using std::cout;
using std::endl;
class DisjointSet
{
private:
    std::vector<int> parent;
    std::vector<int> rank;

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

public:
    std::vector<std::vector<int>> disjointSearch(int n, std::vector<std::pair<int, int>> pairs)
    {
        parent.resize(n);
        rank.resize(n, 0);

        // 初始化并查集，每个元素的父节点是自己
        for (int i = 0; i < n; ++i)
        {
            parent[i] = i;
        }

        // 按照字典序排序pairs数组
        std::sort(pairs.begin(), pairs.end());

        // 合并集合
        for (const auto &pair : pairs)
        {
            int rootX = find(pair.first);
            int rootY = find(pair.second);
            if (rootX != rootY)
            {
                if (rank[rootX] < rank[rootY])
                {
                    parent[rootX] = rootY;
                }
                else if (rank[rootX] > rank[rootY])
                {
                    parent[rootY] = rootX;
                }
                else
                {
                    parent[rootY] = rootX;
                    rank[rootX]++;
                }
            }
        }

        // 构建结果
        // std::vector<std::vector<int>> result;
        // std::vector<int> tempSet;
        // int currentRoot = -1;
        // for (int i = 0; i < n; ++i)
        // {
        //     cout << i << endl;
        //     int root = find(i);
        //     cout << root << endl;
        //     if (root != currentRoot)
        //     {
        //         if (currentRoot != -1)
        //         {
        //             result.push_back(tempSet);
        //         }

        //         tempSet.clear();
        //         currentRoot = root;
        //     }
        //     tempSet.push_back(i);
        // }
        // result.push_back(tempSet);
        // 构建结果
        std::unordered_map<int, std::vector<int>> rootToSet;
        for (int i = 0; i < n; ++i)
        {
            int root = find(i);
            rootToSet[root].push_back(i);
        }

        std::vector<std::vector<int>> result;
        for (const auto &pair : rootToSet)
        {
            result.push_back(pair.second);
        }

        return result;
    }
};

int main()
{
    int n = 9;
    std::vector<std::pair<int, int>> pairs = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {0, 2}, {4, 6}, {0, 8}};
    DisjointSet ds;
    std::vector<std::vector<int>> result = ds.disjointSearch(n, pairs);

    std::cout << "Disjoint Sets: \n";
    for (const auto &set : result)
    {
        for (int num : set)
        {
            std::cout << num << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

// // 5. 构建结果
// int pre_root = -1;                          // 记录上一次的根节点，约定值是0~eq_rel_size-1，初始为-1表示未找到根节点
// vector<int> tmp_vec(0);                     // 临时的一维向量，一个一维向量存储一棵树/同一个根节点的值
// for (int i = 0; i < this->eq_rel_size; ++i) // 遍历元素值
// {
//     int cur_root = this->find_parent(i); // 找当前根节点

//     // 情况1：cur_root == pre_root，如果当前根节点和上次根节点相同，说明是同一个根节点同一棵树，前面肯定已经构造向量，直接把元素加入向量
//     if (cur_root == pre_root)
//     {
//         tmp_vec.push_back(i);
//     }
//     // 情况2：如果当前根节点和上次根节点不相同，说明不是同一个根节点同一棵树，当前根节点指向的是一颗新树
//     else // cur_root != pre_root
//     {
//         // 情况2.1：如果上次根节点是-1，则表示上次根节点没有树
//         if (pre_root == -1)
//         {
//             pre_root = cur_root;  // 记录
//             tmp_vec.clear();      // 创建新集合
//             tmp_vec.push_back(i); // 加入元素
//         }
//         // 情况2.2：如果上次根节点不是-1，表示之前tmp_vec已经记录了一棵树，需要用第二维度记录这棵树
//         // 然后同2.1操作
//         else // pre_root != -1
//         {
//             this->disjoint_set.push_back(tmp_vec); // 第二维度记录一棵树结果

//             pre_root = cur_root;  // 记录
//             tmp_vec.clear();      // 创建新集合
//             tmp_vec.push_back(i); // 加入元素
//         }
//     }
// }
// this->disjoint_set.push_back(tmp_vec); // 记录操作最后一棵树
