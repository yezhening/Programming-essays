use rand::Rng; // trait
use std::cmp::Ordering;
use std::io; // prelude

fn main() {
    println!("猜数游戏！！");
    let secret_number = rand::thread_rng().gen_range(1, 101);
    // 区间左闭右开
    // println!("神秘数字是：{}", secret_number);

    loop {
        println!("猜测一个数");

        let mut guess = String::new(); // i32 u32 i64
        io::stdin().read_line(&mut guess).expect("无法读取行");
        // shadow，变量类型转换为整数并覆盖
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        println!("你猜测的数是：{}", guess);

        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small"), // arm
            Ordering::Greater => println!("Too big"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        };
    }
}
