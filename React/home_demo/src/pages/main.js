// 引入 Ant Design
// 再拆组件
import { theme, Layout } from 'antd'
import CommonAside from '../components/commonAside/commonAside'
import CommonHeader from '../components/commonHeader/commonHeader'

import { useSelector } from 'react-redux'

import { Outlet } from 'react-router-dom'

const { Content, Footer } = Layout

const Main = () => {
    const {
        token: { colorBgContainer, borderRadiusLG },
    } = theme.useToken()

    // 获取展开收起状态值
    const isFold = useSelector((state) => {
        return state.fold.isFold
        // fold 是 store/reducers/fold.js 注册的 name
    })

    return (
        <Layout className='main-container'>
            <CommonAside isFold={isFold} />
            {/* 传入状态值给组件，判断是否展开或收起 */}
            <Layout>
                <CommonHeader isFold={isFold} />
                <Content
                    style={{
                        margin: '24px 16px',
                        padding: 24,
                        minHeight: 280,
                        background: colorBgContainer,
                        borderRadius: borderRadiusLG,
                    }}
                >
                    <Outlet />
                    {/* 放置子路由的出口，即显示子路由内容的容器 */}
                </Content>
                {/* Content 类似提供一个容器，有样式设置，不让里面的组件过于贴边 */}
                <Footer style={{ textAlign: 'center' }}>
                    system ©{new Date().getFullYear()} Created by 夜悊
                </Footer>
            </Layout>
        </Layout>
    )
}

export default Main