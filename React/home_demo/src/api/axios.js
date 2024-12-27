// axios 二次封装
// 封装成一个类对象 http 导出

// 前端逻辑用 http 调用封装的方法 request()
// request() 内部处理 axios 相关的逻辑
// 请求和响应由拦截器拦截（在请求或响应被 then 或 catch 处理前拦截并处理
import axios from 'axios'

const baseUrl = '/api'  // 可以用于分组不同的 api

class HttpRequest {
    constructor(baseUrl) {
        this.baseUrl = baseUrl
    }

    // 获取自带的参数
    getInsideConfig() {
        const config = {
            baseUrl: this.baseUrl,
            header: {}
        }

        return config
    }

    // 直接复制文档的示例代码，将 axios 改为 instance 绑定到实例
    interception(instance) {
        // 添加请求拦截器
        instance.interceptors.request.use(function (config) {
            // 在发送请求之前做些什么
            return config;
        }, function (error) {
            // 对请求错误做些什么
            return Promise.reject(error);
        });

        // 添加响应拦截器
        instance.interceptors.response.use(function (response) {
            // 2xx 范围内的状态码都会触发该函数。
            // 对响应数据做点什么
            return response;
        }, function (error) {
            // 超出 2xx 范围的状态码都会触发该函数。
            // 对响应错误做点什么
            return Promise.reject(error);
        });
    }

    // 外部调用的方法
    request(options) {
        // 合并自带的参数和外部的参数
        options = {
            ...this.getInsideConfig(),
            ...options
        }

        // 创建 axios 对象
        const instance = axios.create()

        // 注册拦截器
        this.interception(instance)

        // 传递参数给 axios 对象调用获取数据
        return instance(options)
    }
}

// 警告说赋值给一个变量再导出
const http = new HttpRequest(baseUrl)
export default http