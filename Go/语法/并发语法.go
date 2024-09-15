package main

import (
	"fmt"
)

// 并发
func func1() {
	fmt.Println("func1")
}

func func2() {
	fmt.Println("func2")
}

// 管道（同步通信）
func func3(ch chan int) {
	ch <- 1
}

func concurrenceGrammar() {
	// 并发
	go func1()
	func2()

	// 管道（同步通信）
	ch := make(chan int)
	go func3(ch)

	res := <-ch
	fmt.Println(res)
}

/*
输出：
func1
func2
1
*/
