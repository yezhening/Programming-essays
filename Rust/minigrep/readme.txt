运行：
cargo run autumn poem.txt

测试：
cargo test

设置不区分大小写：（win 需要在 cmd 窗口）
set CASE_INSENSITIVE=1
cargo run AUTUMN poem.txt

设置区分大小写：（win 需要在 cmd 窗口）
set CASE_INSENSITIVE=
cargo run AUTUMN poem.txt

测试标准错误直接输出到屏幕上而不是文件：
cargo run > output.txt
cargo run autumn poem.txt > output.txt