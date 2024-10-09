package model

import (
	"database/sql"
	"time"
)

// 直接用 gorm.Model 无法给 JSON 标签 转成小写，需要复制
// 报红字段需要引入和替换
type GormModel struct {
	Id        uint         `gorm:"primarykey" json:"id"`
	CreatedAt time.Time    `json:"created_at"`
	UpdatedAt time.Time    `json:"updated_at"`
	DeletedAt sql.NullTime `gorm:"index" json:"deleted_at"`
}

type TodoList struct {
	GormModel        // 继承
	Content   string `json:"content"`
	Completed bool   `json:"completed"`
}
