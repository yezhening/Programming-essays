# include <iostream>
using namespace std;

void SortArray(int array[], int n)
{
	int i = 0, j = n - 1, temp;
	while (i < j)
	{
		while ((array[i] % 2) != 0)
			i++;
		while ((array[j] % 2) == 0)
			j--;
		if (i < j)
		{
			temp = array[i];//简单交换而不维持次序
			array[i] = array[j];
			array[j] = temp;
		}
	}
}

int main(void)
{
	int array[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int i;
	for (i = 0; i < 10; i++)
	{
		cout<<array[i]<<" ";
	}
	printf("\n");
	SortArray(array, 10);
	int j;
	for (int j = 0; j < 10; j++)
	{
		cout<<array[j]<<" ";
	}
	printf("\n");
	return (0);
}