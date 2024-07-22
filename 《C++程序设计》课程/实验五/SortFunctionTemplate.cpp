#include <iostream>
//using namespace std;
template <typename T>
void Input(T a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
}
template <typename T>
void Sort(T a[], int n)
{
    T t;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n - 1; j++)
        {
            if (a[i] > a[j + 1])
            {
                t = a[i];
                a[i] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}
template <typename T>
void Output(T a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (i != n - 1)
            std::cout << a[i] << ", ";
        else
        {
            std::cout << a[i];
            printf("\n");
        }
    }
}
int main()
{
    const int LEN = 5;
    int type;
    while (std::cin >> type)
    {
        switch (type)
        {
        case 0:
        {
            int a1[LEN];
            Input<int>(a1, LEN); Sort<int>(a1, LEN); Output<int>(a1, LEN);
            break;
        }
        case 1:
        {
            char a2[LEN];
            Input(a2, LEN); Sort(a2, LEN); Output(a2, LEN);
            break;
        }
        case 2:
        {
            double a3[LEN];
            Input(a3, LEN); Sort(a3, LEN); Output(a3, LEN);
            break;
        }
        }
    }

    return 0;
}


