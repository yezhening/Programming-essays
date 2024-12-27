use std::sync::mpsc;
use std::sync::Arc;
use std::sync::Mutex;
use std::thread;

// 需要创建线程等待任务，而不是立即执行
// worker 封装线程
struct Worker {
    id: usize,
    // execute() 期望替代 thread::spawn()
    // thread::spawn() 返回的线程类型是 thread::JoinHandle<T>
    // 只是处理，没有返回值，所以是 JoinHandle<()>

    // worker.thread.join().unwrap(); 需要得到所有权
    // 使用 Option<> 的 take() 获取 Some() 所有权，原来位置变成 None
    thread: Option<thread::JoinHandle<()>>,
}

impl Worker {
    // Arc<Mutex：期望多线程共享，并互斥访问接收端
    fn new(id: usize, receiver: Arc<Mutex<mpsc::Receiver<Message>>>) -> Worker {
        let thread = thread::spawn(move || loop {
            // move 取 receiver 的所有权
            // 死循环等待接收任务

            // 从线程池接收任务->消息
            let message = receiver.lock().unwrap().recv().unwrap();
            match message {
                Message::NewJob(job) => {
                    println!("Worker {} got a job; executing.", id);

                    // (*job)(); // 执行任务
                    job.call_box();
                    // 这里接收任务并处理时，还是会占有锁，导致阻塞变成串行
                }
                Message::Terminate => {
                    println!("Worker {} was told to terminate.", id);

                    break;
                }
            }
        });

        Worker {
            id, // 键值相同可以省略
            thread: Some(thread),
        }
    }
}

// (*job)(); 不可，不知道从 Box<> 取出的内容多大
// 让其可以调用 Box<> 自己：
trait FnBox {
    fn call_box(self: Box<Self>);
}

// Job 等所有实现 FnOnce 的
// 让其实现 FnBox
// 以可以调用 call_box()
impl<F: FnOnce()> FnBox for F {
    fn call_box(self: Box<F>) {
        (*self)()
    }
}

// struct Job;
// type Job = Box<dyn FnOnce() + Send + 'static>;
type Job = Box<dyn FnBox + Send + 'static>;

// 发送任务或者停机信号给线程，以可以退出死循环
enum Message {
    NewJob(Job),
    Terminate,
}

pub struct ThreadPool {
    workers: Vec<Worker>,
    sender: mpsc::Sender<Message>,
    // 线程池有发送端，发送任务给各个线程，线程有接收端
}

impl ThreadPool {
    /// Create a new ThreadPool.
    ///
    /// The size is the number of threads in the pool.
    ///
    /// # Panics
    ///   
    /// The `new()` function will panic if the size is zero.
    pub fn new(size: usize) -> ThreadPool {
        assert!(size > 0);

        let (sender, receiver) = mpsc::channel();
        let receiver = Arc::new(Mutex::new(receiver)); // 包装安全形式

        let mut workers = Vec::with_capacity(size);
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver))); // 共享接收端
        }

        ThreadPool { workers, sender }
    }

    // execute() 期望替代 thread::spawn()
    // 所以参数闭包的 where 约束和其相同
    // Job 类型就是闭包的类型，也就是 Job 是闭包的别称类型
    pub fn execute<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        let job = Box::new(f); // 闭包包装为任务
        self.sender.send(Message::NewJob(job)).unwrap(); // 线程池发送任务
    }
}

impl Drop for ThreadPool {
    // 优雅停机
    fn drop(&mut self) {
        // 发送停机信号，期望退出线程
        println!("Sending terminate message to all workers.");

        for _ in &mut self.workers {
            self.sender.send(Message::Terminate).unwrap();
        }

        println!("Shutting down all workers.");

        // 等待线程退出
        for worker in &mut self.workers {
            println!("Shutting down worker {}", worker.id);

            // worker.thread.join().unwrap();
            if let Some(thread) = worker.thread.take() {
                thread.join().unwrap();
            }
        }
    }
}
