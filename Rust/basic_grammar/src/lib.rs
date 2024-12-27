// ——————————7.1 - Package, Crate, Module——————————
mod mod_tmp {
    pub mod a {
        pub fn b() {}
    }
}

// ——————————7.2 - 路径——————————
pub fn c() {
    crate::mod_tmp::a::b(); // 绝对路径
    mod_tmp::a::b(); // 相对路径（使用者和定义者在同一模块）
}
// 条目（函数和模块等）啥都没有，默认私有
// 使用 pub 关键字设为公有
// mod_tmp 是根级，同个文件可以互相调用而不用公有

// ——————————7.3 - 路径（2）- super, pub struct、enmu——————————
mod d {
    pub fn e() {
        // c();
        super::c(); // super：上级目录/作用域（相对路径）
        crate::c(); // 绝对路径
    }
}
// pub struct，字段默认私有
// pub enum，枚举默认公共，因为公共了才有用

// ——————————7.4 - use 关键字（上）——————————
use crate::mod_tmp::a;
// use mod_tmp::a;
pub fn f() {
    a::b();
}
