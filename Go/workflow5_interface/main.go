package main

import (
	"database/sql"
	"encoding/json"
	"log"
	"net/http"
	"os"

	_ "github.com/go-sql-driver/mysql" // 空导入，为了执行包的init()注册驱动
)

var dbHandlerGlobal *sql.DB // 全局让各个处理函数都能使用
// const serverAddrGlobal = "127.0.0.1:8000"

const serverAddrGlobal = "211.71.149.249:8000"

func main() {
	// 打开日志
	logHandler := openLogFile()
	defer logHandler.Close()
	log.SetOutput(logHandler) // 重定向标准输出到日志

	// 连接数据库
	dbHandlerGlobal = connectDb()
	defer dbHandlerGlobal.Close()

	// 注册路由和处理
	registerRouteAndHandler()

	// 启动服务器
	startServer(serverAddrGlobal)
}

func openLogFile() *os.File {
	logHandler, err := os.OpenFile("./log.txt", os.O_WRONLY|os.O_APPEND, 0600)
	if err != nil {
		log.Fatalf("Failed to open log file: %v", err)
	}

	return logHandler
}

func connectDb() *sql.DB {
	dbConnectConfig := "tmp:tmp@tcp(211.71.149.249:3306)/jk2024"
	//dbConnectConfig := "root:123@tcp(localhost:3306)/jk2024"
	dbHandlerGlobal, err := sql.Open("mysql", dbConnectConfig)
	if err != nil {
		log.Fatalf("Error connecting to the database: %v", err)
	}

	err = dbHandlerGlobal.Ping() // 验证连接
	if err != nil {
		log.Fatalf("Error pinging the database: %v", err)
	}
	log.Println("Connected to the database successfully")

	return dbHandlerGlobal
}

func startServer(addr string) {
	log.Printf("Server starting on address %s\n", addr)
	err := http.ListenAndServe(addr, nil) // 默认协程并发处理请求
	if err != nil {
		log.Fatalf("Failed to start server: %v\n", err)
	}
}

func registerRouteAndHandler() {
	http.HandleFunc("/", testInterface)

	http.HandleFunc("/userStatus", updateUserStatus)

	http.HandleFunc("/userOneClockInOutStatus", queryOneUserClockInOutStatus)
	http.HandleFunc("/userClockInOutStatus", insertUserClockInOutStatus)
	http.HandleFunc("/userAllClockInOutStatus", queryAllUserClockInOutStatus)
}

func testInterface(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello, this is a simple API!"))
}

func updateUserStatus(w http.ResponseWriter, r *http.Request) {
	// 判断请求方法
	if r.Method != http.MethodPut {
		http.Error(w, "Only PUT method is allowed", http.StatusMethodNotAllowed)
		return
	}

	// 解码
	type requestDataStruct struct {
		Dn     string `json:"dn"` // 字段需要被外部包/json解码器访问需要大写导出，否则有警告
		Ou     string `json:"ou"`
		Time   string `json:"time"`
		Ip     string `json:"ip"`
		Status string `json:"status"`
	}
	var requestData requestDataStruct

	jsonDecoder := json.NewDecoder(r.Body)
	err := jsonDecoder.Decode(&requestData)
	// log.Printf("Decoded JSON: %+v\n", data)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// 更新数据库
	querySql := "UPDATE userstatus SET ou=?, time=?, ip=?, status=? WHERE dn=?;"
	sqlStatement, err := dbHandlerGlobal.Prepare(querySql)
	if err != nil {
		http.Error(w, "Failed to update the record", http.StatusInternalServerError)
		return
	}
	defer sqlStatement.Close()

	_, err = sqlStatement.Exec(requestData.Ou, requestData.Time, requestData.Ip, requestData.Status, requestData.Dn)
	if err != nil {
		http.Error(w, "Failed to update the record", http.StatusInternalServerError)
		return
	}

	// 成功响应
	w.WriteHeader(http.StatusOK)
}

func queryOneUserClockInOutStatus(w http.ResponseWriter, r *http.Request) {
	// 判断请求方法
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST method is allowed", http.StatusMethodNotAllowed)
		return
	}

	// 解码
	type requestDataStruct struct {
		Dn     string `json:"dn"`
		Time   string `json:"time"`
		Status string `json:"status"`
	}
	var requestData requestDataStruct

	jsonDecoder := json.NewDecoder(r.Body)
	err := jsonDecoder.Decode(&requestData)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// 查询数据库
	querySql := "SELECT COUNT(*) FROM clock WHERE dn=? AND DATE(time)=DATE(?) AND state=?;"
	// 用DATE()获取数据库字段的日期而不是时间部分进行比对，time字段是string但符合日期格式就能用；数据库里是state而不是status
	resultRows := dbHandlerGlobal.QueryRow(querySql, requestData.Dn, requestData.Time, requestData.Status)

	var resultRowsCount int
	err = resultRows.Scan(&resultRowsCount)
	if err != nil {
		http.Error(w, "Failed to query the record", http.StatusInternalServerError)
		return
	}

	// 响应结果
	type responseResultStruct struct {
		Result bool `json:"result"`
	}
	var responseResult responseResultStruct
	if resultRowsCount >= 1 {
		responseResult = responseResultStruct{Result: true} // 即使一直点击打卡按钮同一天打卡多次都是有效
	} else {
		responseResult = responseResultStruct{Result: false}
	}

	// 编码发送
	response, err := json.Marshal(responseResult)
	if err != nil {
		http.Error(w, "Failed to marshal response", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
	w.Header().Set("Content-Type", "application/json")
	w.Write(response)
}

func insertUserClockInOutStatus(w http.ResponseWriter, r *http.Request) {
	// 判断请求方法
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST method is allowed", http.StatusMethodNotAllowed)
		return
	}

	// 解码JSON
	type requestDataStruct struct {
		Dn     string `json:"dn"`
		Ip     string `json:"ip"`
		Time   string `json:"time"`
		Status string `json:"status"`
	}
	var requestData requestDataStruct

	jsonDecoder := json.NewDecoder(r.Body)
	err := jsonDecoder.Decode(&requestData)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// 插入数据库
	querySql := "INSERT INTO clock (dn, time, ip, state) VALUES (?, ?, ?, ?);" // 数据库字段是state不是status
	sqlStatement, err := dbHandlerGlobal.Prepare(querySql)
	if err != nil {
		http.Error(w, "Failed to prepare the statement", http.StatusInternalServerError)
		return
	}
	defer sqlStatement.Close()

	_, err = sqlStatement.Exec(requestData.Dn, requestData.Time, requestData.Ip, requestData.Status)
	if err != nil {
		http.Error(w, "Failed to insert the record", http.StatusInternalServerError)
		return
	}

	// 成功响应
	w.WriteHeader(http.StatusOK)
	// w.WriteHeader(http.StatusCreated) // 通常对于资源创建成功的响应是201
}

func queryAllUserClockInOutStatus(w http.ResponseWriter, r *http.Request) {
	// 判断请求方法
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST method is allowed", http.StatusMethodNotAllowed)
		return
	}

	// 解码JSON
	type requestDataStruct struct {
		Dn string `json:"dn"`
	}
	var requestData requestDataStruct

	jsonDecoder := json.NewDecoder(r.Body)
	err := jsonDecoder.Decode(&requestData)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// 查询数据库
	querySql := "SELECT time, ip, state FROM clock WHERE dn=?;" // 数据库字段是state
	resultRows, err := dbHandlerGlobal.Query(querySql, requestData.Dn)
	if err != nil {
		http.Error(w, "Failed to query the database", http.StatusInternalServerError)
		return
	}
	defer resultRows.Close()

	// 响应结果
	type responseResultStruct struct {
		Time   string `json:"time"`
		Ip     string `json:"ip"`
		Status string `json:"status"`
	}
	var responseResults []responseResultStruct

	for resultRows.Next() {
		var responseResult responseResultStruct
		err = resultRows.Scan(&responseResult.Time, &responseResult.Ip, &responseResult.Status)
		if err != nil {
			http.Error(w, "Failed to scan row", http.StatusInternalServerError)
			return
		}

		responseResults = append(responseResults, responseResult)
	}

	// 编码发送
	response, err := json.Marshal(responseResults)
	if err != nil {
		http.Error(w, "Failed to marshal results to JSON", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
	w.Header().Set("Content-Type", "application/json")
	w.Write(response)
}
