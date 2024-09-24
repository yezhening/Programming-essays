package main

import (
	"context"
	"fmt"
	"time"
)

func contextPkg() {
	ctx, cancelFunc := context.WithCancel(context.Background()) // 可用于跨协程通信
	go working(ctx)

	// 等待一段时间后发送取消通知
	time.Sleep(1 * time.Second)
	cancelFunc()

	// 等待一段时间确保协程接收到取消信号并结束
	time.Sleep(1 * time.Second)
}

func working(ctx context.Context) {
	for {
		select {
		case <-ctx.Done():
			fmt.Println("工作结束！")
			return
		default:
			fmt.Println("工作中...")
		}
	}
}

/*
输出：
工作中...
工作中...
工作中...
工作结束！
person
*/
