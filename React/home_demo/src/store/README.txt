统一状态管理使用流程

header 点击按钮收起或展开 aside，涉及兄弟组件的通信
不用中间组件传递状态
用 redux 和 reducer 统一管理状态

依据教程和文档的快速上手学习理解：
https://www.bilibili.com/video/BV1kc411D7F9?spm_id_from=333.788.videopod.sections&vd_source=9ac1c0a6d496218112b60d49bc768cd7
https://cn.redux.js.org/tutorials/quick-start

1. 创建 reducer，见 fold.js
2. 创建 store，引入 reducer，见 store.js
3. 在 index 挂载 store，见顶层 index.js
4. 在组件中使用 store，见 main.js（侧边栏和顶部栏需要状态值传入，则在顶层中间桥梁 main.js 中使用）
4.1 使用状态值显示，见 commonAside.js
4.2 调用修改状态值的动作函数，见 commonHeader.js
useSelector() 取状态值
const dispatch = useDispatch()
dispatch(setIsFold()) 调用修改状态值的动作函数