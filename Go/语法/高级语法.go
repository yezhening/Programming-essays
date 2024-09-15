package main

import (
	"fmt"
)

// 结构（封装）
type point struct {
	x int
	y int
}

// 方法（封装）
func (po *point) setPoint(x, y int) {
	po.x = x
	po.y = y
}

// 接口（多态）
type shape interface {
	print()
}

type rectangle struct {
}

func (re rectangle) print() {
	fmt.Println("rectangle")
}

type circle struct {
}

func (ci circle) print() {
	fmt.Println("circle")
}

// 组合匿名字段（继承）
type person struct {
	name string
}

type man struct {
	person
	sex string
}

func advancedGrammar() {
	// 结构（封装）
	po := point{1, 2}
	fmt.Println(po)

	// 方法（封装）
	po.setPoint(3, 4)
	fmt.Println(po)

	// 接口（多态）
	var sh shape

	sh = rectangle{}
	sh.print()

	sh = circle{}
	sh.print()

	// 组合匿名字段（继承）
	ma := man{person{"张三"}, "男"}
	fmt.Println(ma.name)
	fmt.Println(ma.sex)
}

/*
输出：
{1 2}
{3 4}
rectangle
circle
张三
男
*/
