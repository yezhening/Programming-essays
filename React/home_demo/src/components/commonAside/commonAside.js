import { Layout, Menu } from 'antd';
import MenuConfig from '../../config/menuConfig'
import * as Icon from '@ant-design/icons'
import React from 'react';

const { Sider } = Layout;

const iconToElement = (name) => {
    return React.createElement(Icon[name]) // 依据名字获取对象，构造组件
}

// 根据 Ant Design 文档，处理菜单配置数据
// todo：若存在多级需要递归处理才通用
const items = MenuConfig.map((item) => {
    // 没有子菜单的
    const res = {
        key: item.path, // 路径可以作为唯一标识
        icon: iconToElement(item.icon), // 将字符串转换为一个组件：<UserOutlined />
        label: item.label
    }

    // 有子菜单
    if (item.children) {
        res.children = item.children.map((subItem) => {
            // 类似无子菜单，填充二级数据
            const subRes = {
                key: subItem.path,
                icon: iconToElement(subItem.icon),
                label: subItem.label
            }

            return subRes
        })
    }

    return res
})

const CommonAside = ({ isFold }) => {
    // 解构得展开收起状态值
    return (
        <Sider trigger={null} collapsed={isFold}>
            {/* 属性设置展开或收起 */}
            <h3 className='app-name'>
                {
                    isFold ? '系统' : '-系统-'
                }
            </h3>
            {/* 添加名称；根据展开收起显示不同内容 */}
            <Menu
                theme="dark"
                mode="inline"
                defaultSelectedKeys={['1']}
                items={items}   // 可以从配置文件获取菜单项以显示
                style={{    // 侧边栏铺满高度
                    height: '100%'
                }}
            />
        </Sider>
    )
}

export default CommonAside