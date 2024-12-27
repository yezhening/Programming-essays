import { Row, Col, Card, Table, Space } from 'antd';
import './home.css'
import userPic from '../../assets/images/user.png'

import { getData } from '../../api/api'
import { useEffect, useState } from 'react';

import * as Icon from '@ant-design/icons'
import React from 'react';

import MyEcharts from '../../components/echarts/echarts'

// table 列数据
const columns = [
    {
        title: '课程',
        dataIndex: 'name'
    },
    {
        title: '今日购买',
        dataIndex: 'todayBuy'
    },
    {
        title: '本月购买',
        dataIndex: 'monthBuy'
    },
    {
        title: '总购买',
        dataIndex: 'totalBuy'
    }
]

// 订单统计数据
const countData = [
    {
        "name": "今日支付订单",
        "value": 1234,
        "icon": "CheckCircleOutlined",
        "color": "#2ec7c9"
    },
    {
        "name": "今日收藏订单",
        "value": 3421,
        "icon": "ClockCircleOutlined",
        "color": "#ffb980"
    },
    {
        "name": "今日未支付订单",
        "value": 1234,
        "icon": "CloseCircleOutlined",
        "color": "#5ab1ef"
    },
    {
        "name": "本月支付订单",
        "value": 1234,
        "icon": "CheckCircleOutlined",
        "color": "#2ec7c9"
    },
    {
        "name": "本月收藏订单",
        "value": 3421,
        "icon": "ClockCircleOutlined",
        "color": "#ffb980"
    },
    {
        "name": "本月未支付订单",
        "value": 1234,
        "icon": "CloseCircleOutlined",
        "color": "#5ab1ef"
    }
]

const iconToElement = (name) => {
    return React.createElement(Icon[name]) // 依据名字获取对象，构造组件
}

const Home = () => {
    // const userPic = require('../../assets/images/user.png')

    // echart 响应数据
    const [echartData, setEchartData] = useState({})

    // 副作用相关逻辑，如网络请求数据
    // dom 首次渲染完成触发
    useEffect(() => {
        getData().then(({ data }) => {
            console.log(data.data)
            const { tableData, orderData, userData, videoData } = data.data   // 根据数据内容取

            // 表格数据
            setTableData(tableData)

            // 组装 ECharts 数据
            const order = orderData
            const xData = order.date    // 横轴数组

            const series = []
            const keyArray = Object.keys(order.data[0])
            // 第一项组装成数组：苹果 vivo oppo
            // 遍历keyArray，作为 series 的 name
            // 遍历keyArray，再根据 苹果 从data取数字，作为 series 的 data

            keyArray.forEach((key) => {
                series.push({
                    name: key,
                    data: order.data.map((item) => { return item[key] }),
                    type: 'line'
                })
            })

            setEchartData({
                // 折线图
                order: {
                    xData,  // 横轴
                    series  // 数据
                    // 传递过去，要和 echart.js 使用的字段名称对应
                    // echart.js 取字段的值构造 option 并绘图
                },
                // 柱状图
                user: {
                    xData: userData.map((item) => { return item.date }),
                    series: [
                        {
                            name: '新增用户',
                            data: userData.map((item) => { return item.new }),
                            type: 'bar'
                        },
                        {
                            name: '活跃用户',
                            data: userData.map((item) => { return item.active }),
                            type: 'bar'
                        }
                    ]
                },
                // 饼状图
                video: {
                    series: [
                        {
                            data: videoData,
                            type: 'pie'
                        }
                    ]
                }
            })
        })
    }, [])

    // 需要显示在页面的动态数据需要 useState()
    // 取 table 数据
    const [tableData, setTableData] = useState()

    return (
        <Row className='home'>
            <Col span={8}>
                {/* flex 属性会自动换行，可以设置不换行 */}
                <Space direction="vertical" size="middle" style={{ display: 'flex' }}>
                    <Card hoverable>
                        <div className='user'>
                            <img src={userPic} alt="" />
                            {/* 没有 alt 属性 eslint 会有警告 */}
                            <div className='userInfo'>
                                <p className='name'>Admin</p>
                                <p>管理员</p>
                            </div>
                        </div>
                        <div className='login-info'>
                            <p>上次登入时间：<span>2024-10-15</span></p>
                            <p>上次登入地点：<span>北京</span></p>
                            {/* todo：这里动态获取数据 */}
                        </div>
                    </Card>
                    <Card hoverable>
                        <Table
                            columns={columns}
                            dataSource={tableData}
                            pagination={false}
                            rowKey={"name"} // 需要 key 否则有警告
                        />
                    </Card>
                </Space>
            </Col>
            <Col span={16}>
                <div className='num'>
                    {
                        countData.map((item, index) => {
                            return (
                                <Card key={index}>
                                    <div className='icon-box' style={{ background: item.color }}>
                                        {iconToElement(item.icon)}
                                        {/* 图标需要转换成组件 */}
                                    </div>
                                    <div className='detail'>
                                        <p className='num'>￥{item.value}</p>
                                        <p className='txt'>{item.name}</p>
                                    </div>
                                </Card>
                            )
                        })
                    }
                </div>
                {/* <div id='main' style={{ height: '300px' }}>
                    {/* echarts 已经渲染 dom，但是高度为0而看不到 */}
                {/* </div> */}
                {echartData.order && <MyEcharts style={{ height: '280px' }} isAxisChart={true} chartData={echartData.order} />}
                {/* 属性要对上 echart.js 的名字 */}
                {/* && 短路操作，一开始还没请求数据不渲染，否则报错无法渲染 */}
                <div className='graph'>
                    {echartData.user && <MyEcharts style={{ height: '240px', width: '50%' }} isAxisChart={true} chartData={echartData.user} />}
                    {echartData.video && <MyEcharts style={{ height: '260px', width: '50%' }} isAxisChart={false} chartData={echartData.video} />}
                </div>
            </Col>
        </Row>
    )
}

export default Home