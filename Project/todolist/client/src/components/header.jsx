export function Header({ todoText, setTodoText, addTodo }) {
    return (
        <div>
            <input
                type="text"
                placeholder="Todo..."
                value={todoText}
                onInput={(e) => { setTodoText(e.target.value) }} />
            <button onClick={addTodo}>ADD</button>
        </div>
    )
}