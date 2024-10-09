package service

import (
	"server/model"

	"gorm.io/gorm"
)

type TodoList struct {
	*gorm.DB
}

func (list *TodoList) GetList() []model.TodoList {
	var todos []model.TodoList

	list.Order("id desc").Find(&todos) // ORM 映射直接操作

	return todos
}

func (list *TodoList) AddTodo(content string) uint {
	todo := model.TodoList{
		// gorm 自动填充自己的字段
		Content:   content,
		Completed: false,
	}

	list.Create(&todo)

	return todo.Id
}

func (list *TodoList) ToggleTodo(id uint) uint {
	var completed bool

	list.Model(&model.TodoList{}).Where("id = ?", id).Select("completed").Scan(&completed)
	list.Model(&model.TodoList{}).Where("id = ?", id).Update("completed", !completed)

	return id
}

func (list *TodoList) DelTodo(id uint) uint {
	list.Delete(&model.TodoList{}, id)

	return id
}
