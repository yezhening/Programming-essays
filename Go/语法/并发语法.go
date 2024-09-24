package main

import (
	"fmt"
	"sync"
	"time"
)

// 并发
func func1() {
	fmt.Println("func1")
}

func func2() {
	fmt.Println("func2")
}

// chan 管道（同步通信）
func func3(ch chan int) {
	ch <- 1
}

func concurrenceGrammar() {
	// 并发
	go func1()
	func2()

	// chan 管道（同步通信）
	ch := make(chan int)
	go func3(ch)

	res := <-ch
	fmt.Println(res)

	// select-case
	c1 := make(chan string)
	c2 := make(chan string)

	go func() {
		time.Sleep(1 * time.Second)
		c1 <- "from c1"
	}()
	go func() {
		time.Sleep(2 * time.Second)
		c2 <- "from c2"
	}()

	select {
	case msg1 := <-c1:
		fmt.Println(msg1)
	case msg2 := <-c2:
		fmt.Println(msg2)
	}

	// sync.WaitGroup
	var wg sync.WaitGroup

	for i := 0; i < 2; i++ {
		wg.Add(1) // 增加计数

		go func(id int, wg *sync.WaitGroup) {
			defer wg.Done() // 减少计数

			fmt.Printf("Worker %d finished\n", id)
		}(i, &wg)
	}

	wg.Wait() // 等待计数为 0
	fmt.Println("All workers finished")

	// sync.Mutex
	var mutex sync.Mutex
	counter := 0

	for i := 0; i < 100; i++ {
		wg.Add(1)

		go func() {
			defer wg.Done()

			mutex.Lock()
			counter++
			mutex.Unlock()
		}()
	}

	wg.Wait()
	fmt.Println(counter)

	// sync.Once
	var once sync.Once

	for i := 0; i < 2; i++ {
		wg.Add(1)

		go func() {
			defer wg.Done()

			once.Do(func() {
				fmt.Println("sync.Once")
			}) // 保证一个函数只被执行一次
		}()
	}

	wg.Wait()

	// sync.Map
	var m sync.Map // 用于并发安全处理 map 数据

	for i := 0; i < 100; i++ {
		wg.Add(1)

		go func(k, v string) {
			defer wg.Done()

			m.Store(k, v)
		}(fmt.Sprintf("key%d", i), fmt.Sprintf("value%d", i))
	}

	wg.Wait()
	if value, ok := m.Load("key99"); ok {
		fmt.Println(value)
	}
}

/*
输出：
func1
func2
1
from c1
Worker 1 finished
Worker 0 finished
All workers finished
100
sync.Once
value99
*/
