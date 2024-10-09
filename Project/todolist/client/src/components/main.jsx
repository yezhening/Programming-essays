export function Main({ todoList, toggleTodo, delTodo }) {
    return (
        <div>
            <ul>
                {
                    todoList && todoList.map(todo => (
                        <li key={todo.id}>
                            <input
                                type="checkbox"
                                checked={todo.completed}
                                onChange={() => { toggleTodo(todo.id) }}
                            />
                            <span style={{ textDecoration: todo.completed ? "line-through" : "" }}>{todo.content}</span>
                            <button onClick={() => { delTodo(todo.id) }}>DEL</button>
                        </li>
                    ))
                }
            </ul>
        </div>
    )
}