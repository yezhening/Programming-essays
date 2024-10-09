package db

import (
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

func Connect(dbName string) *gorm.DB {
	dsn := "root:root@tcp(175.24.200.226:3306)/" + dbName + "?charset=utf8mb4&parseTime=True&loc=Local"
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		panic(err.Error())
	}

	return db
}
