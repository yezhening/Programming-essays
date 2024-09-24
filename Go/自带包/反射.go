package main

import (
	"fmt"
	"reflect"
)

type person struct {
	name string
	age  int
}

func (p person) Print() { // 必须公有给 reflect 包查找
	fmt.Println("person")
}

func reflectPkg() {
	p := person{name: "张三", age: 30}

	// 查看类型和值
	typ := reflect.TypeOf(p)
	val := reflect.ValueOf(p)
	fmt.Println("Type:", typ)
	fmt.Println("Value:", val)

	// 查看字段
	fmt.Println("Fields:")
	for i := 0; i < val.NumField(); i++ {
		fieldName := typ.Field(i).Name
		field := val.Field(i)
		fmt.Printf("Field: %s, Value: %v\n", fieldName, field)
	}

	// 查看方法
	fmt.Println("Methods:")
	for i := 0; i < typ.NumMethod(); i++ {
		method := typ.Method(i)
		fmt.Printf("Method: %s, Type: %s\n", method.Name, method.Type)
	}

	// 调用方法
	method := val.MethodByName("Print")
	method.Call(nil)
}

/*
输出：
Type: main.person
Value: {张三 30}
Fields:
Field: name, Value: 张三
Field: age, Value: 30
Methods:
Method: Print, Type: func(main.person)
person
*/
