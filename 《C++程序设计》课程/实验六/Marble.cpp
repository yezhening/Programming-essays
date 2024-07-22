#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int N, Q, M[1000],i,x;
    while (cin >> N >> Q)
    {
        for (i = 0; i < N; i++)
        {
            cin >> M[i];
        }
        sort(M, M + N);
        for(i=1;i<=Q;i++) 
        {
            cin >> x;
            int p;
            p= lower_bound(M, M + N, x) - M;
            if (M[p] == x)
                cout << x << " found at " << p + 1 << endl;
            else
                cout << x << " not found " << endl;
        }
    }
    return 0;
}