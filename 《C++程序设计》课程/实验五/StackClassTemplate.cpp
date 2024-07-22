#include <iostream>
using namespace std;
template<class T, int SIZE = 20>
class Stack
{
private:
    T    array[SIZE];        //数组，用于存放栈的元素
    int top;                //栈顶位置（数组下标）
public:
    Stack()                //构造函数，初始化栈
    {
        top = -1;
    }
    void Push(const T& p)  //元素入栈
    {
        top = top + 1;
        if (top < SIZE)
        {
            array[top] = p;
        }
    }
    T Pop()                //栈顶元素出栈
    {
        if (top != -1)
        {
            return array[top--];
        }
        else
            exit(1);
    }
    void Clear()           //将栈清空
    {
        top = -1;
    }
    const T& Top() const  //访问栈顶元素
    {
        if (top != -1)
        {
            return array[top];
        }
        else
            exit(1);
    }
    bool Empty() const     //测试栈是否为空
    {
        if (top == -1)
            return true;
        else
            return false;
    }
    bool Full() const     //测试是否栈满
    {
        if (top == SIZE-1)
            return true;
        else
            return false;
    }
    int Size()            //返回当前栈中元素个数
    {
        return top + 1;
    }
};
int main()
{
    Stack<int, 10> intStack;

    int n;
    cin >> n; //n<=10
    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        intStack.Push(temp);
    }

    for (int i = 0; i < n; i++)
    {
        cout << intStack.Top() << " ";
        intStack.Pop();
    }
    cout << endl;

    if (intStack.Empty())
        cout << "Now, intStack is empty." << endl;

    Stack<string, 5> stringStack;
    stringStack.Push("One");
    stringStack.Push("Two");
    stringStack.Push("Three");
    stringStack.Push("Four");
    stringStack.Push("Five");
    cout << "There are " << stringStack.Size() << " elements in stringStack." << endl;
    stringStack.Clear();
    if (stringStack.Empty())
        cout << "Now, there are no elements in stringStack" << endl;

    return 0;
}