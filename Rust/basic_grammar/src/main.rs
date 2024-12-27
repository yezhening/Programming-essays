use std::{collections::HashMap, fs::File};

fn main() {
    // ——————————5 stuct——————————
    // 注解，派生于 Debug trait（理解为接口），以可以打印结构体信息
    #[derive(Debug)]

    // 结构体
    struct User {
        name: String,
        age: u64, // 最后一个字段也有逗号
    }

    // 方法
    impl User {
        fn print(&self) {
            println!("{}, {}", self.name, self.age)
        }

        // 关联函数（类似静态方法；通常用于构造器）
        fn init() -> User {
            // 实例化（没分号，匿名返回）
            User {
                name: String::from("张三"),
                age: 18,
            }
        }
    }

    // 使用
    let u = User::init();
    println!("{:?}", u);
    u.print();

    // ——————————6.1  定义枚举——————————
    #[derive(Debug)]
    enum Ip {
        V4(String),
        // 考虑将该字段更改为单元类型，以在保留字段编号的同时抑制此警告，
        // 或者删除该字段："()"
        // 警告的意思是：可能该枚举未被使用
        V6(String),
        // 警告要用大写命名
        // 警告用了 #Debug 但未使用
    }

    impl Ip {
        fn print(&self) {
            println!("{:?}", self)
        }
    }

    let ip = Ip::V4(String::from("127.0.0.1"));
    ip.print();

    //  ——————————6.2  Option 枚举——————————
    let some = Some(5); // 编译器推断类型
    let none: Option<i32> = None; // 推断不出需要显式指明类型
                                  // 只要不是 Option<T> 类型，就可以安全地假设该值必不为空 None
                                  // Option<T> 和 T 不同，必须显式转换从而确保安全
                                  // 用于取代 Null 的设计
                                  // 只要不是 Option<T>，就必不为空，而 Option<T> 需要手动转换成 T
    println!("{:?}", some);
    println!("{:?}", none);

    // ——————————6.3 match——————————
    // 类似 switch case
    let a = 2;
    match a {
        1 => println!("1"),
        2 => println!("2"),
        3 => {
            println!("3");
            println!("3");
        }
        i32::MIN..=0 => println!("Matched from i32::MIN to 0"), // = 号的意思是右闭区间
        _ => println!("Matched from 4 to i32::MAX"),
        // 分支需要覆盖所有可能的值，这里是 a i32 的所有可能取值
    }

    // ——————————6.4 if let——————————
    if let 2 = a {
        println!("if let");
    } else {
        println!("if let else");
    }
    // 只关注一种可能性
    // match 的语法糖

    // ——————————8.1 Vector——————————
    // let v: Vec<i32> = Vec::new();
    let mut v = vec![0, 1, 2];
    v.push(3);
    println!("{}", v[0]);

    // ——————————8.5 HashMap（上）——————————
    let mut mp = HashMap::new();
    mp.insert(String::from("key"), 10);

    let value = mp.get("key");
    match value {
        Some(s) => println!("{}", s),
        None => println!("None"),
    };

    // ——————————9.1 不可恢复的错误与 panic!——————————
    // panic!("panic")

    // ——————————9.2 Result 枚举与可恢复的错误（上）——————————
    // let f = File::open("a.txt").unwrap();

    // ——————————10.2 泛型——————————
    struct Point<T, U> {
        x: T,
        y: U,
    }
    let p = Point { x: 5, y: 1.0 };

    // ——————————10.3 Trait（上）——————————
    trait Interface {
        fn func(&self); // 必须有 self
    }

    impl<T, U> Interface for Point<T, U> {
        fn func(&self) {
            println!("trait");
        }
    }

    p.func();

    // ——————————10.5 生命周期（1）——————————
    func("1", "12");
}

fn func<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
// 只是标注引用生命周期的关系
// 'a 标注的生命名周期是 x 和 y 重叠/最短的部分

/*
输出：
User { name: "张三", age: 18 }
张三, 18
V4("127.0.0.1")
Some(5)
None
2
if let
0
10
trait
*/
