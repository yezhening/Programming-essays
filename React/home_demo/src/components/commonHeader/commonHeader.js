import { Layout, Button, Avatar, Dropdown } from 'antd';
import './commonHeader.css'
import { MenuFoldOutlined } from '@ant-design/icons';
import userPic from '../../assets/images/user.png'

import { useDispatch } from 'react-redux';
import { setIsFold } from '../../store/reducers/fold'

const { Header } = Layout;

const CommonHeader = ({ isFold }) => {
    // 登出
    const logout = () => {
    }

    // 下拉菜单的数据
    const items = [
        {
            key: '1',
            label: (
                <a target="_blank" rel="noopener noreferrer" href='https://www.aliyun.com'>
                    个人中心
                </a>
                // 需要 href 属性，否则有 eslint 警告
            ),
        },
        {
            key: '2',
            label: (
                <a onclick={() => { logout() }} target="_blank" rel="noopener noreferrer" href='https://www.aliyun.com'>
                    登出
                </a>
                // 添加点击登出事件
            ),
        }
    ]

    // 获取 dispatch
    const dispatch = useDispatch()

    return (
        <Header className='header-container'>
            {/* 删除 Header 白色样式 */}
            <Button
                type="text"
                // 引入图标和完善样式
                icon={<MenuFoldOutlined />}
                style={{
                    fontSize: '16px',
                    width: 64,
                    height: 32,
                    background: '#fff'
                }}
                onClick={() => { dispatch(setIsFold()) }}
            // 注册按钮，展开收起侧边栏
            />
            {/* 添加右上角标和下拉菜单 */}
            <Dropdown menu={{ items }}>
                <Avatar size={36} src={userPic} />
            </Dropdown>
        </Header>
    )
}

export default CommonHeader