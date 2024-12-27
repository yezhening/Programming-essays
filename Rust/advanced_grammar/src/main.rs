use advanced_grammar::Data;
use std::sync::{mpsc, Mutex};
use std::{rc::Rc, thread, time::Duration};

fn main() {
    // ——————————13.1 - 闭包——————————
    // ——————————13.4 - 闭包：使用闭包捕获上下文——————————
    let x = 10;
    let closure = |num| {
        println!("{} {}", num, x);
    };

    closure(5); // 使用时可以推断类型

    // ——————————13.5 - 迭代器：Iterator trait 和 next 方法——————————
    let v = vec![1, 2, 3];
    for val in v.iter() {
        print!("{}-", val)
    }
    println!();

    // ——————————15.1 - 使用 Box 指向 Heap 上的数据——————————
    let b = Box::new(5); // 把栈上数据可以存在堆上（智能指针）
    println!("{}", b);

    // ——————————15.4 - Rc——————————
    let s = Rc::new(String::from("hello"));

    let s1 = Rc::clone(&s); // 增加引用计数

    print!("{}-", Rc::strong_count(&s));

    {
        let s2 = Rc::clone(&s);
        print!("{}-", Rc::strong_count(&s));
    }

    println!("{}-", Rc::strong_count(&s));

    // ——————————16.1 - 多线程同时运行代码——————————
    let handle = thread::spawn(|| {
        for i in 1..10 {
            print!("{}-", i);

            thread::sleep(Duration::from_millis(1));
            // 小睡眠才能看到交替输出现象
        }
    });

    for i in 11..20 {
        print!("{}-", i);

        thread::sleep(Duration::from_millis(1));
    }
    handle.join().unwrap();
    println!();

    // ——————————16.2 - 使用消息传递来跨线程传递数据——————————
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = 1;
        tx.send(val).unwrap();
    });

    let received = rx.recv().unwrap();
    println!("Got: {}", received);

    // ——————————16.3 - 共享状态的并发——————————
    let m = Mutex::new(0);
    {
        let mut num = m.lock().unwrap();
        *num += 1;
    }

    println!("{:?}", m);

    // ——————————17.1 - 面向对象语言特性——————————
    // ——————————17.2 - 使用 Trait 对象来存储不同类型的值——————————
    // 封装
    let d = Data::new();
    println!("{}", d.get_data());

    // 继承->组合
    trait Draw {
        fn draw(&self);
    }

    struct Circle {
        radius: i32,
    }

    impl Draw for Circle {
        fn draw(&self) {
            println!("Drawing a circle with radius {}", self.radius);
        }
    }

    struct Square {
        side: i32,
    }

    impl Draw for Square {
        fn draw(&self) {
            println!("Drawing a square with side length {}", self.side);
        }
    }

    let circle = Circle { radius: 1 };
    let square = Square { side: 2 };

    // 多态
    let shapes: Vec<&dyn Draw> = vec![&circle, &square]; //借用
    for shape in shapes.iter() {
        shape.draw();
    }

    // ——————————19.1 - Unsafe Rust——————————
    let mut num = 5;
    let r = &mut num as *mut i32;

    // println!("{}", *r);
    unsafe {
        println!("{}", *r);
    }

    unsafe fn dangerous() {}

    unsafe {
        dangerous();
    }

    unsafe trait Foo {}

    unsafe impl Foo for i32 {}

    // ——————————Rust 19.2 - 高级 Trait——————————
    trait Iterator {
        type Item; // 关联类型，而不是泛型

        fn next(&mut self) -> Option<Self::Item>;
    }

    struct Counter {}

    impl Iterator for Counter {
        type Item = i32;

        fn next(&mut self) -> Option<Self::Item> {
            None
        }
    }

    // ——————————Rust 19.3 - 高级类型——————————
    type Key = u32;
    let a: Key = 5;

    // ——————————19.4 - 高级函数和闭包——————————
    func2(func);
}

fn func() {
    print!("fn-");
}

fn func2(f: fn()) {
    f();
    f();
}

/*
输出：
5 10
1-2-3-
5
2-3-2-
11-1-12-2-13-3-14-4-15-5-16-6-17-18-7-8-19-9-
Got: 1
Mutex { data: 1, poisoned: false, .. }
0
Drawing a circle with radius 1
Drawing a square with side length 2
5
fn-fn-
*/
