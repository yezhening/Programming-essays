package main

import (
	"database/sql" // SQL 接口
	"fmt"
	"sync"
	"time"

	_ "github.com/go-sql-driver/mysql" // MySQL 驱动
)

func main() {
	// 连接数据库
	db, err := sql.Open("mysql", "root:root@tcp(175.24.200.226:3306)/yzndb")
	if err != nil {
		panic(err)
	}
	defer db.Close()

	// 配置数据库连接池
	db.SetMaxOpenConns(100) // 最大打开的连接数

	// 1. 创建表
	create_table_sql := `
	CREATE TABLE yzn_person2 
	(
		id INT PRIMARY KEY AUTO_INCREMENT,
		name VARCHAR(100) NOT NULL
	);
	`
	create_table(db, create_table_sql)

	// 2. 插入100条数据
	insert_data_concurrently(db, 100)

	// 3. 插入1000条数据
	insert_data_concurrently(db, 1000)
}

// 创建表
func create_table(db *sql.DB, create_table_sql string) {
	start := time.Now() // 记录开始时间

	_, err := db.Exec(create_table_sql)
	if err != nil {
		fmt.Printf("创建表失败: %v\n", err)
	}

	end := time.Now() // 记录结束时间
	elapsed := end.Sub(start)
	seconds := elapsed.Seconds()
	fmt.Printf("创建表完成，用时: %fs\n", seconds)
}

// 并发插入数据
func insert_data_concurrently(db *sql.DB, insert_num int) {
	start := time.Now()

	var wg sync.WaitGroup

	for i := 0; i < insert_num; i++ {
		wg.Add(1)

		go func(id int) {
			defer wg.Done()
			insert_data(db, id)
		}(i)
	}

	wg.Wait()

	end := time.Now()
	elapsed := end.Sub(start)
	seconds := elapsed.Seconds()
	fmt.Printf("插入%d条数据完成，用时: %fs\n", insert_num, seconds)
}

// 插入数据
func insert_data(db *sql.DB, id int) {
	name := fmt.Sprintf("虚拟名称%d", id)

	_, err := db.Exec("INSERT INTO yzn_person2 (name) VALUES (?)", name)
	if err != nil {
		fmt.Printf("插入数据失败: id %d, Error: %v\n", id, err)
	}
}
