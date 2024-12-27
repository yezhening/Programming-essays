// 1. 状态值 
// 2. 修改状态值的动作函数
// 3. 导出
import { createSlice } from '@reduxjs/toolkit'

const foldSlice = createSlice({
    name: 'fold',
    initialState: {
        isFold: false    // 折叠 fasle，即默认展开
    },
    reducers: {
        setIsFold: (state) => {
            state.isFold = !state.isFold
        }
    }
})

export const { setIsFold } = foldSlice.actions  // 用于在使用时回调，dispatch(setIsFold())
export default foldSlice.reducer    // 用于在 store 注册