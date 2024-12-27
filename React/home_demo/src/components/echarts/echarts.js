// 通用的渲染考虑封装成组件
// 由外部用户传参决定渲染成什么样的图表
import * as echarts from 'echarts'
import { useEffect, useRef } from 'react'

// 直接引入教程的配置，数据从 http 请求获取
const axisOption = {
    // 图例文字颜色
    textStyle: {
        color: "#333",
    },
    // 提示框
    tooltip: {
        trigger: "axis",
    },
    xAxis: {
        type: "category", // 类目轴
        data: [],
        axisLine: {
            lineStyle: {
                color: "#17b3a3",
            },
        },
        axisLabel: {
            interval: 0,
            color: "#333",
        },
    },
    yAxis: [
        {
            type: "value",
            axisLine: {
                lineStyle: {
                    color: "#17b3a3",
                },
            },
        },
    ],
    color: ["#2ec7c9", "#b6a2de", "#5ab1ef", "#ffb980", "#d87a80", "#8d98b3"],
    series: [],
}

const normalOption = {
    tooltip: {
        trigger: "item",
    },
    color: [
        "#0f78f4",
        "#dd536b",
        "#9462e5",
        "#a6a6a6",
        "#e1bb22",
        "#39c362",
        "#3ed1cf",
    ],
    series: [],
}
// 有坐标系的如折线图和柱状图
// 没坐标系的如饼状图

// 由外部用户传参决定渲染成什么样的图表
const ECharts = ({ style, isAxisChart = true, chartData }) => {
    // 每个图标绑定的 dom 不同
    // 1. 用于持久引用 dom，组件重新渲染也引用同一个元素
    // 2. 用于访问 dom，因为不推荐直接操作 dom，echarts 需要与 dom 交互
    const echartRef = useRef()

    // 1. useState() 双向绑定，数据-视图变化
    // 2. useRef() 单向绑定，数据变视图不变，视图变数据不变，用于创建视图不变的响应式变量
    let echartobj = useRef()

    // 引入副作用
    // 为了组件挂载后执行初始化图表
    useEffect(() => {
        // echartRef.current 就可以拿到引用的 dom
        echartobj.current = echarts.init(echartRef.current)

        // 请求数据后设置 option
        let options
        if (isAxisChart) {
            axisOption.xAxis.data = chartData.xData
            axisOption.series = chartData.series
            // 完善配置axisOption.xAxis.data+数据chartData.xData
            // 数据的 xData 和 series 要和传递进来的字段对应

            options = axisOption
        }
        else {
            normalOption.series = chartData.series

            options = normalOption
        }

        echartobj.current.setOption(options)
    }, [isAxisChart, chartData])
    // 数据源变化需要更新
    // eslint 警告需要添加 isAxisChart

    return (
        <div style={style} ref={echartRef}></div>
    )
}

export default ECharts