// ——————————11.1 - 编写和运行测试——————————
pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)] // 属性宏，用于条件编译，仅在测试模式（cargo test）运行
mod tests {
    use super::*; // 导入外部模块

    #[test] // 声明测试函数
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }

    // #[test]
    // fn another() {
    //     panic!("fail")
    // }

    // ——————————11.4 - 编写测试：使用 should_panic 检查是否恐慌——————————
    #[test]
    #[should_panic]
    fn another2() {
        panic!("fail")
    }

    // ——————————11.5 - 编写测试：在测试中使用 Result——————————
    // ——————————11.8 - 控制测试运行：忽略测试——————————
    #[test]
    #[ignore]
    fn another3() -> Result<(), String> {
        if 1 + 2 == 4 {
            Ok(())
        } else {
            Err(String::from("fail"))
        }
    }
}

pub struct Data {
    data: i32,
}

impl Data {
    pub fn new() -> Data {
        Data { data: 0 }
    }

    pub fn get_data(&self) -> i32 {
        self.data
    }
}
