// 主逻辑和测试放在 lib.rs，模块化
use std::error::Error;
use std::{env, fs};

// 关联的变量作为结构体
pub struct Config {
    pub query: String,
    pub filename: String,
    pub case_insensitive: bool,
}

// 返回 Config 就相当于构造函数
impl Config {
    pub fn new(args: &[String]) -> Result<Config, &'static str> {
        // 错误处理
        if args.len() < 3 {
            // panic!("参数不足") // 针对程序编写的反馈
            return Err("参数不足"); // Result<>针对程序使用的反馈
        }

        let query = args[1].clone();
        let filename = args[2].clone();
        // Config 实例需要所有权，但不能借用，切片不可复制
        // 可以简单低效克隆，效率换取无需管理生命周期的简洁性
        let case_insensitive = env::var("CASE_INSENSITIVE").is_ok();
        // 从环境变量获取是否需要大小写敏感搜索
        // env::var() 返回 Result<>，只关心是否出现而不是里面的内容，再 is_ok() 取得 bool 值
        Ok(Config {
            query,
            filename,
            case_insensitive,
        })
    }
}

// main 中逻辑足够清晰
// Box<dyn Error>：实现 error trait 的任意类型
pub fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.filename)?;
    // ? 如果错误不会 panic 而是返回给调用者
    // println!("{}", contents);

    // 判断是否需要大小写敏感再搜索
    let results = if config.case_insensitive {
        search_case_insensitive(&config.query, &contents)
    } else {
        search(&config.query, &contents)
    };

    // 输出
    for line in results {
        println!("{}", line);
    }

    Ok(())
}

// 不满足生命周期原则
// 结果要放 contents 中的某一行，所以要同样的生命周期
pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    let mut results = Vec::new();

    for line in contents.lines() {
        if line.contains(query) {
            results.push(line);
        }
    }

    results
}

pub fn search_case_insensitive<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    // 双方都变成小写或大写进行比较即可
    let query = query.to_lowercase(); // 会创建新的字符串进行覆写
    let mut results = Vec::new();

    for line in contents.lines() {
        if line.to_lowercase().contains(&query) {
            results.push(line);
        }
    }

    results
}

// TDD 测试驱动开发
// 1. 编写失败的测试
// 2. 修复测试，即完成逻辑功能
#[cfg(test)]
mod tests {
    use super::*;

    // 普通测试结果
    #[test]
    fn one_result() {
        let query = "duct";
        let contents = "\
Rust:
safe, fast, productive.
Pick three.";

        assert_eq!(vec!["safe, fast, productive."], search(query, contents));
        // 查到匹配行
    }

    // 区分大小写
    #[test]
    fn case_sensitive() {
        let query = "duct";
        let contents = "\
Rust:
safe, fast, productive.
Pick three.
Duct tape.";

        assert_eq!(vec!["safe, fast, productive."], search(query, contents));
    }

    // 不区分大小写
    #[test]
    fn case_insensitive() {
        let query = "rUsT";
        let contents = "\
Rust:
safe, fast, productive.
Pick three.
Trust me.";

        assert_eq!(
            vec!["Rust:", "Trust me."],
            search_case_insensitive(query, contents)
        );
    }
}
