package main

import (
	"encoding/json"
	"fmt"
	"io"
	"math"
	"os"
	"strconv"
	"strings"
	"time"
)

func otherPkg() {
	// time 包
	t := time.Now()
	fmt.Println(t)

	// math
	a := math.Sqrt(64)
	fmt.Println(a)

	// io
	file, err := os.Open("example.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	_, err = io.Copy(os.Stdout, file)
	if err != nil {
		panic(err)
	}
	fmt.Println()

	// json
	type person struct {
		Name string `json:"name"`
		Age  int    `json:"age"`
	}

	p := person{Name: "张三", Age: 30}
	jsonData, err := json.Marshal(p)
	if err != nil {
		panic(err)
	}
	fmt.Println(string(jsonData))

	// strconv
	intVal, err := strconv.Atoi("3")
	if err != nil {
		panic(err)
	}
	fmt.Println(intVal)

	// strings
	str := "aaa"
	upperStr := strings.ToUpper(str)
	fmt.Println(upperStr)

	// os 包
	os.Exit(0)
}

/*
输出：
2024-09-22 16:24:03.4747672 +0800 CST m=+0.009179801
8
测试 io 包
{"name":"张三","age":30}
3
AAA
*/
