// 拦截请求，返回模拟数据
import Mock from 'mockjs'

// 直接引入教程给的数据
import homeApi from './mockServeData/home'

Mock.mock('/home/getData', homeApi.getStatisticalData)