# README.md

[TOC]

## 如何在freebsd编译go

修改bsd环境

- rm -rf /usr/local/go 删除旧版本go
- uname -a查看freebsd版本

> FreeBSD Jabber2 11.4-RELEASE FreeBSD 11.4-RELEASE #0 r362094: Fri Jun 12 18:27:15 UTC 2020     root@releng2.nyi.freebsd.org:/usr/obj/usr/src/sys/GENERIC  amd64

- 搜索freebsd11只支持go 1.17-1.19.x

> [Go Wiki: Go on FreeBSD - The Go Programming Language (google.cn)](https://golang.google.cn/wiki/FreeBSD)

- 使用go1.19.13.freebsd-amd64.tar.gz安装

> [All releases - The Go Programming Language (google.cn)](https://golang.google.cn/dl/)

- tar -C /usr/local -xzf go1.19.13.freebsd-amd64.tar.gz

> [第 22.8 节 Rust/Go 环境的配置 | FreeBSD 从入门到跑路 (bsdcn.org)](https://book.bsdcn.org/di-22-zhang-bian-cheng-yu-kai-fa/di-22.8-jie-rustgo-huan-jing-de-pei-zhi)

- go version查看安装成功

移植程序

- 修改环境变量go env -w GOMODCACHE = C:\Users\DSHH\Desktop\workflow5_interface\pkg（freebsd离线，需要本地包）

```
# 以下不需要用
go list -m -versions github.com/go-sql-driver/mysql 查看包版本
go list -m -f '{{.GoVersion}}' github.com/go-sql-driver/mysql@v1.8.1 查看包需要的go版本
在go.mod 降低依赖包版本，需要在1.17-1.19.x之间
```

- go mod init main
- go mod tidy

- go.mod加上替换为本地包的路径的语句

```
replace github.com/go-sql-driver/mysql => ./pkg/github.com/go-sql-driver/mysql@v1.8.1

replace filippo.io/edwards25519 => ./pkg/filippo.io/edwards25519@v1.1.0
```

- 修改go.mod这行格式1.22.4修改为1.22

```
go 1.22
```

- 修改main.go的serverAddrGlobal变量为服务器地址
- 文件夹整个放在服务器上

- 进入文件夹
- go build main main.go
- ./main & 后台运行
- curl http://211.71.149.249:8000/ 测试输出

## 修改程序后移到freebsd运行

- 将程序放到/root
- 执行命令

```cmd
cd /root/workflow5_interface/

./start.sh
```

注意：windows 拖到 freebsd 的 .sh 需要需要权限并使用 LF 而不是 CRLF 结尾，否则不可运行