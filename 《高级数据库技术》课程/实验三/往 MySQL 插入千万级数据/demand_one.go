package main

import (
	"database/sql" // SQL 接口
	"fmt"
	"sync"

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
	db.SetMaxOpenConns(1000) // 最大打开的连接数

	// 创建表
	create_table_sql := `
	CREATE TABLE yzn_person 
	(
		id INT PRIMARY KEY AUTO_INCREMENT,
		name VARCHAR(100) NOT NULL
	);
	`
	_, err = db.Exec(create_table_sql)
	if err != nil {
		fmt.Printf("创建表失败: %v\n", err)
	}

	// 使用协程组插入数据
	var wg sync.WaitGroup
	const insert_num = 1000000 // 数量

	for i := 0; i < insert_num; i++ {
		wg.Add(1)

		go func(id int) {
			defer wg.Done()
			insert_data(db, id)
		}(i)
	}

	wg.Wait()
	fmt.Println("所有数据插入完成")
}

// 插入数据
func insert_data(db *sql.DB, id int) {
	name := fmt.Sprintf("虚拟名称%d", id)

	_, err := db.Exec("INSERT INTO yzn_person (name) VALUES (?)", name)
	if err != nil {
		fmt.Printf("插入数据失败: id %d, Error: %v\n", id, err)
	} else {
		fmt.Printf("插入数据成功: id %d\n", id)
	}
}
