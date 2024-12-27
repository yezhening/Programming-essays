import Main from '../pages/main'
import Home from '../pages/home/home'
import Mall from '../pages/mall/mall'
import User from '../pages/user/user'
import PageOne from '../pages/other/pageOne'
import PageTwo from '../pages/other/pageTwo'
import { createBrowserRouter, Navigate } from 'react-router-dom'

const routes = [
    {
        path: '/',
        Component: Main,
        children: [
            {
                path: '/',
                element: <Navigate to='home' replace />   // 重定向
            },
            {
                path: 'home',
                Component: Home
            },
            {
                path: 'mall',
                Component: Mall
            },
            {
                path: 'user',
                Component: User
            },
            {
                path: 'other',
                children: [
                    {
                        path: 'pageOne',
                        Component: PageOne
                    },
                    {
                        path: 'pageTwo',
                        Component: PageTwo
                    }
                ]
            }
        ]
    }
]

export default createBrowserRouter(routes)