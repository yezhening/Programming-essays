#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>

class ThreadPool
{
public:
    ThreadPool(size_t num);
    ~ThreadPool();

private:
    std::vector<std::thread> threads_;
    void work();
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::queue<std::function<void()>> tasks_{};
    bool stop_flag_ = false;

public:
    void add_task(std::function<void()> task);
};

ThreadPool::ThreadPool(size_t num)
{
    // 创建线程
    for (size_t i = 0; i < num; ++i)
    {
        this->threads_.emplace_back([this]
                                    { this->work(); }); // 线程执行 work()
    }
}

void ThreadPool::work()
{
    std::function<void()> task{};

    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            // while (tasks.empty()) // 被通知唤醒但未竞争到而条件不满足（虚假唤醒）
            // {
            //     this->cond_var_.wait(lock, [this]
            //                          { return !tasks.empty(); }); // 释放锁阻塞直到任务队列不为空就加锁返回
            // }
            // notify_one() 已不存在 notify_all() 的虚假唤醒，只唤醒一个:
            // https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one
            // If any threads are waiting on *this, calling unblocks one of the waiting threads. notify_one
            this->cond_var_.wait(lock, [this]
                                 { return ((this->tasks_.empty() == false) || (this->stop_flag_ == true)); });
            // 释放锁，阻塞直到任务队列不为空 或者 停止 就加锁返回

            task = std::move(this->tasks_.front()); // 取任务
            this->tasks_.pop();
        } // RAII 释放锁

        task(); // 执行任务
    }
}

ThreadPool::~ThreadPool()
{
    // 唤醒并等待所有线程结束
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->stop_flag_ = true;
    }

    this->cond_var_.notify_all();
    for (std::thread &th : this->threads_)
    {
        th.join();
    }
}

void ThreadPool::add_task(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->tasks_.push(std::move(task));
    }

    this->cond_var_.notify_one();
}

int main()
{
    // 创建一个有3个线程的线程池
    ThreadPool thread_pool(3);

    // 创建4个任务
    for (size_t i = 0; i < 4; ++i)
    {
        thread_pool.add_task([]()
                             { std::cout << "Thread id: " << std::this_thread::get_id() << std::endl; });
    }

    std::cin.get(); // 暂停（线程池一般会单例复用而不是销毁）

    return 0;
}
/*
可能的输出：
Thread id: 2
Thread id: 2
Thread id: 4
Thread id: 3
*/