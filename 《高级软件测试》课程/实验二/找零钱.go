package main

import (
	"fmt"
	"math"
)

func main() {
	// 价格和付款
	price := 30
	payment := 100

	// 找零钱
	res, err := find_change(price, payment)
	if err != nil {
		fmt.Println("错误：", err)
		return
	}

	// 输出找零钱结果
	fmt.Println("找零钱结果：")
	for denomination, count := range res {
		fmt.Printf("%d元：%d张\n", denomination, count)
	}
}

// 找零钱
func find_change(price, payment int) (map[int]int, error) {
	// 价格和付款必须是正数
	if price <= 0 {
		return nil, fmt.Errorf("价格必须是正数")
	}
	if payment <= 0 {
		return nil, fmt.Errorf("付款必须是正数")
	}

	// 价格和付款必须<=100
	if price > 100 {
		return nil, fmt.Errorf("价格必须<=100")
	}
	if payment > 100 {
		return nil, fmt.Errorf("付款必须<=100")
	}

	// 价格和付款不能是小数
	if math.Floor(float64(price)) != float64(price) {
		return nil, fmt.Errorf("价格不能是小数")
	}
	if math.Floor(float64(payment)) != float64(payment) {
		return nil, fmt.Errorf("付款不能是小数")
	}

	// 付款不够价格
	if payment < price {
		return nil, fmt.Errorf("付款金额不足价格")
	}

	// 可用面额（要使数量最少，则优先找大零钱）
	denominations := []int{50, 10, 5, 1}

	// 应找零钱
	change := payment - price

	// 结果
	res := make(map[int]int)

	// 遍历面额
	for _, denomination := range denominations {
		// 当前面额需要的数量
		count := change / denomination

		// 如果需要当前面额，则记录结果
		if count > 0 {
			res[denomination] = count

			// 更新剩余应找零钱
			change -= denomination * count
		}
	}

	return res, nil
}

/*
输出：
找零钱结果：
50元：1张
10元：2张
*/
