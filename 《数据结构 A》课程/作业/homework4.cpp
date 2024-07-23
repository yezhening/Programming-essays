#include<iostream>
using namespace std;
int main()
{
    int a[10][10],dp[10][10],n, i, j;
    cin >> n;
    for (i = 0; i <= 9; i++)//赋初值
    {
        for (j = 0; j <= 9; j++)
        {
            a[i][j] = 9999999;
           dp[i][j] = 9999999;
        }
    }
    for (i = 1; i <= n; i++)//赋值
    {
        for (j = 1; j <= i; j++)
        {
            cin >> a[i][j];
        }
    }
    dp[1][1] = a[1][1];//顶上赋值
    for (i = 2; i <= n; i++)//“递归”向上求
    {
        for (j = 1; j <= i; j++)
        {
            if (dp[i - 1][j - 1] <= dp[i - 1][j])
            {
                dp[i][j] = a[i][j] + dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = a[i][j] + dp[i - 1][j];
            }
        }
    }
    for (i = 1; i <= n; i++)//输出第一行
    {
        cout << dp[n][i]<<" ";
    }
    return 0;
}