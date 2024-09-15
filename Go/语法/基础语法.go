package main

import (
	"fmt"
)

func print(int) int {
	fmt.Println("func()")
	return 1
}

func basicGrammar() {
	fmt.Println("Hello World")

	// 变量
	a := 1
	fmt.Println(a)

	// 类型
	var b string = "2"
	fmt.Printf("%T\n", b)

	// 运算符
	c := a + 2
	fmt.Println(c)

	// 分支结构
	if c == 2 {
		fmt.Println("c == 2")
	} else {
		fmt.Println("c != 2")
	}

	// 循环结构
	for i := 0; i < 5; i++ {
		fmt.Print(i)
	}
	fmt.Println()

	// 函数
	print(1)

	// 数组
	arr := [3]int{1, 2, 3}
	fmt.Println(arr)

	// 切片
	slice := []int{1, 2, 3}
	fmt.Println(slice)

	// 映射
	ma := map[string]int{
		"a": 1,
		"b": 2,
	}
	fmt.Println(ma)

	// 指针
	p := &ma
	fmt.Println(*p)
}

/*
输出：
Hello World
1
string
3
c != 2
01234
func()
[1 2 3]
[1 2 3]
map[a:1 b:2]
map[a:1 b:2]
*/
