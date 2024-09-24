package main

import (
	"fmt"
	"unsafe"
)

func unsafePkg() {
	var value int = 3
	intPtr := &value

	var f float64 = *(*float64)(unsafe.Pointer(intPtr)) // int 小类型 转换为 float64 大类型，不安全
	fmt.Println(f)
}

/*
输出：
1.5e-323
*/
