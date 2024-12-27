import { configureStore } from '@reduxjs/toolkit'
import FoldReducer from './reducers/fold'

const ConfigureStore = configureStore({
    reducer: {
        fold: FoldReducer
    }
})

export default ConfigureStore