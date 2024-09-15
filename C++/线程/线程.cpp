#include <iostream>
#include <thread>

void thread_func()
{
    std::cout << std::this_thread::get_id() << std::endl; // 输出线程 id
}

int main()
{
    // （主线程）创建并执行子线程
    std::thread thread_1(thread_func);
    std::thread thread_2(thread_func);

    // （主线程）等待子线程完成
    thread_1.join();
    thread_2.join();

    return 0;
}
/*
输出：
2
3
*/