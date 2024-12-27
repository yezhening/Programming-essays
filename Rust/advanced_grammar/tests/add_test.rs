// ——————————11.10 - 集成测试——————————
// use advanced_grammar; // 不行
use advanced_grammar::add; // 不行

#[test]
fn it_works() {
    let result = add(2, 2);
    assert_eq!(result, 4);
}
