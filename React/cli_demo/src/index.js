import React from 'react'
import ReactDOM from 'react-dom'

// jsx 直接渲染
// const element = <h1>hello world !</h1>
// ReactDOM.render(element,document.getElementById("root"))

// 函数组件和 props 属性（用于父传子通信；表示静态数据）
// function Hello(props) {
//     return <h1>{props.str}</h1>
// }

// const element = <Hello str="Hello world!" />
// ReactDOM.render(element, document.getElementById("root"))

// 类组件和 props 属性
// class Hello2 extends React.Component {
//     render() {
//         return <h1>{this.props.str}</h1>
//     }
// }

// const element = <Hello2 str="Hello world!" />
// ReactDOM.render(element, document.getElementById("root"))

// 拆分文件组件
// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App />)

// 事件处理
// function handleClick(){
//     console.log('点击了按钮')
// }

// function App2() {
//     return (
//         <button onClick={handleClick}>按钮</button>
//     )
// }

// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App2 />)

// 状态
// import React, { useState } from 'react'
// function App2() {
//     const [num, setNum] = useState(1)

//     function handleClick() {
//         console.log('点击了按钮')
//         setNum(100)
//     }

//     return (
//         <>
//             <h3>{num}</h3>
//             <button onClick={handleClick}>按钮</button>
//         </>
//     )
// }

// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App2 />)

// 图片和样式引入
// import image from './logo.svg'

// function App2() {
//     const css = {
//         widow: 200,
//         height: 200
//     }
//     return (
//         <img
//             src={image}
//             alt=""
//             style={css} />
//     )
// }

// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App2 />)

// 组件复用 + props属性个性化
// function Article(props) {
//     return (
//         <div>
//             <h2>{props.title}</h2>
//             <p>{props.content}</p>
//         </div>
//     )
// }

// function App2() {
//     return (
//         <>
//             <Article
//                 title='标题1'
//                 content='内容1'
//             />
//             <Article
//                 title='标题2'
//                 content='内容2'
//             />
//             <Article
//                 title='标题3'
//                 content='内容3'
//             />
//         </>
//     )
// }

// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App2 />)

// 父传子通信2：children 插槽（用于传递 jsx 而不是单一变量）
// function List({ children }) {
//     return (
//         <ul>
//             {children}
//         </ul>
//     )
// }

// function App2() {
//     return (
//         <>
//             <List>
//                 <li>列表项1</li>
//                 <li>列表项2</li>
//                 <li>列表项3</li>
//             </List>
//         </>
//     )
// }

// const root = ReactDOM.createRoot(document.getElementById("root"))
// root.render(<App2 />)

// 子传父通信：利用事件机制
import { useState } from 'react'

function Detail(props) {
    const [num, setNum] = useState(1)

    function handleClick() {
        setNum(100)
        props.onActive(num)
    }

    return (
        <div>
            <p>{num}</p>
            <button onClick={handleClick}>按钮</button>
        </div>
    )
}

function App2() {
    function handleActive(num) {
        console.log(num)
    }

    return (
        <>
            <Detail onActive={handleActive} />
        </>
    )
}

const root = ReactDOM.createRoot(document.getElementById("root"))
root.render(<App2 />)