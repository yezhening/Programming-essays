import './App.css';
import { RouterProvider } from 'react-router-dom';
import router from './router/router'

function App() {
  return (
    <div className='app'>
      <RouterProvider router={router} />
      {/* 引入路由 */}
    </div>
  );
}

export default App