import axios from "axios"
import { useEffect, useState } from "react"

export function useTodolist() {
    const [todoText, setTodoText] = useState("")
    const [todoList, setTodoList] = useState([])

    // 期望渲染时就已经请求获取到数据
    // useEffect() 引入副作用函数，数据获取是一个例子
    // 可以在特定时刻请求数据；可以只在依赖项变化时再获取数据等
    useEffect(() => {
        // useEffect() 返回清除函数，不能直接用 async/await 返回数据
        // 再封装一层立即执行函数
        // 1. 立即请求数据
        // 2. 否则可能每次渲染都创建一个新的函数示例，这个只创建一次
        (async () => {
            const { data } = await axios("http://127.0.0.1:8080/list")

            setTodoList(data.data)  // 1. 约定服务端返回有 data 字段的 JSON
        })()
    }, [])

    const addTodo = async () => {
        const { data } = await axios.post("http://127.0.0.1:8080/add", {
            content: todoText   // 2. 约定服务端接收有 content 和 id 字段的 JSON
        })

        const todo = {
            id: data.data,  // 返回 id
            // new Date().getTime()
            // 必须 new 得到对象，否则运行时报错: getTime() 不是 func
            content: todoText,
            completed: false
        }

        setTodoList([
            todo,
            ...todoList
        ])

        setTodoText("")
    }

    const toggleTodo = async (id) => {
        const { data } = await axios.post("http://127.0.0.1:8080/toggle", {
            id: id
        })

        setTodoList(todoList.map(todo => {
            if (todo.id === data.data) {    // 返回 id
                todo.completed = !todo.completed
            }
            return todo
        }))
    }

    const delTodo = async (id) => {
        const { data } = await axios.post("http://127.0.0.1:8080/del", {
            id: id
        })

        setTodoList(todoList.filter(todo => {
            if (todo.id !== data.data) {    // 返回 id
                return true
            }
            else {
                return false
            }
        }))
    }

    return {
        todoText,
        setTodoText,
        todoList,
        addTodo,
        toggleTodo,
        delTodo
    }
}