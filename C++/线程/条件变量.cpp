#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

std::mutex mtx;             // 全局互斥量
bool ready_flag = false;    // 条件变量等待的条件，false 等待，true 唤醒
std::condition_variable cv; // 全局条件变量

void print_thread_id()
{
    std::unique_lock<std::mutex> lock(mtx); // 加锁
    while (ready_flag == false)             // 循环等待条件，避免 notify_all() 的虚假唤醒（被通知唤醒但未竞争到而条件不成立）
    {
        cv.wait(lock, []()
                { return (ready_flag == true); }); // 释放锁，阻塞等待条件成立
    }

    // wait() 加锁返回，进入临界区
    std::cout << "Thread id: " << std::this_thread::get_id() << std::endl;
} // RAII 解锁

void go_thread()
{
    std::unique_lock<std::mutex> lock(mtx);
    ready_flag = true; // 置位条件为成立

    cv.notify_all(); // 唤醒所有等待的线程
}

int main()
{
    // 创建线程
    std::vector<std::thread> threads(5);
    for (int i = 0; i < 5; ++i)
    {
        threads.at(i) = std::thread(print_thread_id);
    }
    std::cout << "5 threads ready to race..." << std::endl;

    // 唤醒线程
    go_thread();

    // 等待线程
    for (std::thread &th : threads)
    {
        th.join();
    }

    return 0;
}
/*
输出：
5 threads ready to race...
Thread id: 2
Thread id: 5
Thread id: 4
Thread id: 6
Thread id: 3
*/