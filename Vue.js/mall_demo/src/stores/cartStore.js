import { useUserStore } from "@/stores/userStore";
import { findNewCartListAPI, insertCartAPI, delCartAPI, updateCartItem } from '@/apis/cart'
export const useCartStore = defineStore(
    'cart',
    () => {
        const userStore = useUserStore();
        const isLogin = computed(() => userStore.userInfo.token);
        const cartList = ref([]);

        // 获取登录后最新购物车列表action
        const updateLoginCartList = async () => {
            const res = await findNewCartListAPI()
            cartList.value = res.result
        }

        const addCart = async (goods) => {
            if (isLogin.value) {
                //登录之后加入购物车逻辑
                await insertCartAPI(goods)
                updateLoginCartList()
            } else {
                //判断商品是否在购物车
                const item = cartList.value.find(item => goods.skuId === item.skuId);
                if (item) {
                    item.count += goods.count
                } else {
                    cartList.value.push(goods)
                }
            }
        }
        // 删除购物项
        const delCart = async (skuId) => {
            if (isLogin.value) {
                //登录之后加入购物车逻辑
                // console.log([skuId])
                await delCartAPI([skuId])
                // updateNewList() // 写错了
                updateLoginCartList()
            } else {
                // 思路：
                // 1. 找到要删除项的下标值 - splice
                // 2. 使用splice删除找到的项，原数组改变
                const idx = cartList.value.findIndex((item) => skuId === item.skuId)
                cartList.value.splice(idx, 1)
            }
        }
        // 清除购物车
        const clearCart = () => {
            cartList.value = []
        }
        // 计算属性
        // 1. 总的数量 所有项的count之和
        const allCount = computed(() => cartList.value.reduce((a, c) => a + c.count, 0))
        // 2. 总价 所有项的count*price之和
        const allPrice = computed(() => cartList.value.reduce((a, c) => a + c.count * c.price, 0))

        // 全选功能
        const checkAll = (selected) => {
            // 把cartList中的每一项的selected都设置为当前的全选框状态
            cartList.value.forEach(item => item.selected = selected)
        }

        // 3. 是否全选计算属性
        const isAll = computed(() => cartList.value.every((item) => item.selected))

        // 4. 已选择数量
        const selectedCount = computed(() => cartList.value.filter(item => item.selected).reduce((a, c) => a + c.count, 0))
        // 5. 已选择商品价钱合计
        const selectedPrice = computed(() => cartList.value.filter(item => item.selected).reduce((a, c) => a + c.count * c.price, 0))

        //修改购物项
        const updateCart = async (goods) => {
            const { skuId, count, selected } = goods
            // console.log(skuId)
            if (isLogin.value) {
                await updateCartItem(skuId, { count, selected })
            }
        }
        return {
            allCount,
            allPrice,
            cartList,
            isAll,
            selectedCount,
            selectedPrice,
            addCart,
            delCart,
            checkAll,
            updateLoginCartList,
            clearCart,
            updateCart
        }
    },
    {
        persist: true
    }
)