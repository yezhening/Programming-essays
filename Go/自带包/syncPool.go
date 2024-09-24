package main

import (
	"fmt"
	"sync"
)

type myStruct struct {
	data int
}

var pool = sync.Pool{
	New: func() interface{} {
		fmt.Println("Creating a new myStruct instance")
		return &myStruct{
			data: 1,
		}
	},
}

func syncPoolPkg() {
	myInstance := pool.Get().(*myStruct) // 取
	myInstance.data = 100                // 修改
	pool.Put(myInstance)                 // 放

	myInstanceAgain := pool.Get().(*myStruct) // 再取/复用
	fmt.Println(myInstance == myInstanceAgain)
	fmt.Println(myInstanceAgain.data)
}

/*
输出：
Creating a new myStruct instance
true
100
*/
