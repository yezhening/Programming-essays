package main

import (
	"errors"
	"fmt"
)

// error 接口
func testError() error {
	return errors.New("test error")
}

// panice 和 recover
func myPanic() {
	panic("panic")
}

func otherGrammar() {
	// error 接口
	err := testError()
	if err != nil {
		fmt.Println(err)
	}

	// switch-case
	day := 2
	switch day {
	case 1:
		fmt.Println("Monday")
	case 2:
		fmt.Println("Tuesday")
	case 3:
		fmt.Println("Wednesday")
	default:
		fmt.Println("Invalid day")
	}

	// panic 和 recover
	defer func() {
		if r := recover(); r != nil {
			fmt.Println(r)
		}
	}()
	// defer recover() 在 panic() 之前注册才能捕获
	myPanic()
	fmt.Println("After myPanic()")
}

/*
输出：
test error
Tuesday
panic
*/
