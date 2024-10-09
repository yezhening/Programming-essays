package main

import (
	"fmt"
	"server/controller" // 记得应该是 模块名/目录名
	"server/db"
	"server/service"

	"github.com/gin-gonic/gin"
)

func main() {
	db := db.Connect("study")
	// db.AutoMigrate(&model.TodoList{}) // ORM 自动迁移表
	fmt.Println(db)

	// 服务需要操作数据库
	todolistService := service.TodoList{DB: db} // 字段显式指明
	// 控制器中有服务
	todoListController := controller.TodoList{TodoList: todolistService}

	r := gin.Default()

	// 路由绑定控制器
	r.Use(todoListController.Cors) // CORS
	r.GET("/list", todoListController.List)
	r.POST("/add", todoListController.Add)
	r.POST("/toggle", todoListController.Toggle)
	r.POST("/del", todoListController.Del)

	r.Run(":8080")
}
