import { useTodolist } from "./hooks";

import { Header } from "./components/header";
import { Main } from "./components/main";

export function App() {
    const {
        todoText,
        setTodoText,
        todoList,
        addTodo,
        toggleTodo,
        delTodo
    } = useTodolist()

    const headerProps = { todoText, setTodoText, addTodo }
    const mainProps = { todoList, toggleTodo, delTodo }

    return (
        <>
            <Header {...headerProps} />
            {/* JSX 传递一个对象的每个属性作为单独的 props 要使用展开语法 */}
            <Main {...mainProps} />
        </>
    )
}