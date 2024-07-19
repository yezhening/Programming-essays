#include <vector>
#include <iostream> // INT_MIN

using std::cout;
using std::endl;
using std::max;
using std::vector;

// 最大子数组工具类
class MaxSubarraySum
{
public:
    // 求最大子数组和
    // 参数：数组，数组左索引，数组右索引（左闭右闭区间）
    // 返回值：最大子数组和
    static int get_max_subarray_sum(const vector<int> &arr, int left, int right)
    {
        // 递归求解
        // 1. 递归结束条件
        // 子数组只有一个元素，返回该元素
        if (left == right)
        {
            return arr.at(left);
        }

        // 2. 递归逻辑
        // 求左、右和中三部分的和取最大值
        // 重点理解：左边递归到底部时，实际求的就是左、右、中的最大值；逐层返回后，该值就是左边递归的最大值
        int mid = left + ((right - left) / 2); // == (left + right) / 2，防止溢出

        int left_sum = get_max_subarray_sum(arr, left, mid);
        int right_sum = get_max_subarray_sum(arr, mid + 1, right);
        int cross_sum = get_max_crossing_sum(arr, left, mid, right);

        int max_sum = left_sum > right_sum ? left_sum : right_sum;
        max_sum = max_sum > cross_sum ? max_sum : cross_sum;
        return max_sum;
    }

private:
    // 求最大中间子数组和
    // 参数：数组，数组左索引，数组中间索引，数组右索引（左闭右闭区间）
    // 返回值：最大中间子数组和
    static int get_max_crossing_sum(const vector<int> &arr, int left, int mid, int right)
    {
        // 1. 向左边求最大子数组和
        int tmp_sum = 0;
        int left_sum = INT_MIN;
        for (int i = mid; i >= left; --i)
        {
            tmp_sum += arr[i];

            if (tmp_sum > left_sum)
            {
                left_sum = tmp_sum;
            }
        }

        // 2. 向右边求最大子数组和
        tmp_sum = 0;
        int right_sum = INT_MIN;
        for (int i = mid + 1; i <= right; ++i)
        {
            tmp_sum += arr[i];
            if (tmp_sum > right_sum)
            {
                right_sum = tmp_sum;
            }
        }

        // 3. 中间的最大值，即必须包括左边 + 右边的最大值；
        // 重点理解：中间要想取到最大值，必须包括左右两边取最大值时的元素
        int cross_sum = left_sum + right_sum;
        return cross_sum;
    }
};

int main()
{
    const vector<int> arr{8, -3, -4, 6, -8, 9, -5, 7};

    int max_subarray_sum = MaxSubarraySum::get_max_subarray_sum(arr, 0, arr.size() - 1);

    cout << "最大子数组和: " << max_subarray_sum << endl;

    return 0;
}