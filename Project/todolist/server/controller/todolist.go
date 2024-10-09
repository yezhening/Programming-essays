package controller

import (
	"net/http"
	"server/service"

	"github.com/gin-gonic/gin"
)

// 为了接收 POST 时获取 JSON 请求数据，必须提前声明数据的类型
type PostBody struct {
	Id      uint
	Content string
}

type TodoList struct {
	service.TodoList
}

// 方法
func (list *TodoList) Cors(c *gin.Context) {
	// CORS 固定写法
	c.Header("Access-Control-Allow-Origin", "*")
	c.Header("Access-Control-Allow-Headers", "Content-Type")
	c.Header("Access-Control-Allow-Methods", "GET,POST")

	if c.Request.Method == "OPTIONS" {
		c.JSON(http.StatusOK, "")
		c.Abort()
		return
	}

	c.Next()
}

func (list *TodoList) List(c *gin.Context) {
	result := list.GetList()

	c.JSON(http.StatusOK, gin.H{
		"code": 0,
		"msg":  "ok",
		"data": result,
	})
}

func (list *TodoList) Add(c *gin.Context) {
	var body PostBody
	c.ShouldBindJSON(&body) // 接收请求数据

	result := list.AddTodo(body.Content)

	c.JSON(http.StatusOK, gin.H{
		"code": 0,
		"msg":  "ok",
		"data": result,
	})
}

func (list *TodoList) Toggle(c *gin.Context) {
	var body PostBody
	c.ShouldBindJSON(&body)

	result := list.ToggleTodo(body.Id)

	c.JSON(http.StatusOK, gin.H{
		"code": 0,
		"msg":  "ok",
		"data": result,
	})
}

func (list *TodoList) Del(c *gin.Context) {
	var body PostBody
	c.ShouldBindJSON(&body)

	result := list.DelTodo(body.Id)

	c.JSON(http.StatusOK, gin.H{
		"code": 0,
		"msg":  "ok",
		"data": result,
	})
}
