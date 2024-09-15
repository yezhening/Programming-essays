#include <mutex>
#include <thread>
#include <iostream>

std::mutex mtx; // 全局互斥量

void thread_func(int num, char ch)
{
    mtx.lock(); // 上锁

    for (int i = 0; i < num; ++i) // 临界区
    {
        std::cout << ch;
    }
    std::cout << std::endl;

    mtx.unlock(); // 解锁
}

int main()
{
    std::thread th_1(thread_func, 50, '*');
    std::thread th_2(thread_func, 50, '$');

    th_1.join();
    th_2.join();

    return 0;
}
/*
输出：
**************************************************
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/