import { useTodolist } from "./hooks";

function App() {
  /**
   * {
   *    id:number
   *    content:string
   *    completed:boolean
   * }
   */
  // 自定义hook useTodolist()

  const {
    todoList,
    todoText,
    setTodoText,
    addTodo,
    toggleTodo,
    removeTodo
  } = useTodolist()

  return (
    <div className="App">
      <div>
        <input type="text" placeholder="Todo..." value={todoText} onInput={(e) => setTodoText(e.target.value)} />
        <button onClick={addTodo}>ADD</button>
      </div>
      <div>
        <ul>
          {
            todoList && todoList.map(todo => (
              <li key={todo.id}>
                <input type="checkbox" checked={todo.completed} onChange={() => toggleTodo(todo.id)} />
                <span style={{ textDecoration: todo.completed ? "line-through" : '' }}>{todo.content}</span>
                <button onClick={() => removeTodo(todo.id)}>DELETE</button>
              </li>
            ))
          }
        </ul>
      </div>
    </div>
  );
}

export default App;