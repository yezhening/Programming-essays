use minigrep::Config;
use std::env;
use std::process;

fn main() {
    // 1. 接受命令行参数
    let args: Vec<String> = env::args().collect();
    // 接收非法字符用 env::args_os()
    // println!("{:?}", args);

    let config = Config::new(&args).unwrap_or_else(|err| {
        eprintln!("{}", err); // 一般可以让标准输出到文件，标准错误直接打印到屏幕
        process::exit(1);
    });
    // 闭包

    // 2. 读取文件
    // 3. 搜索
    if let Err(e) = minigrep::run(config) {
        eprintln!("{}", e);
        process::exit(1);
    }
}
