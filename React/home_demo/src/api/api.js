// 为了提高扩展性
// 在 axios.js 中对 axios 请求数据的二次封装
// 前端逻辑直接调用 api.js 的函数
import http from './axios'

export const getData = () => {
    return http.request({
        url: '/home/getData',
        method: 'get'
    })
}