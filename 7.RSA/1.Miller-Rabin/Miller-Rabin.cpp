#include <stdio.h>
#include <stdlib.h>
#define time 10
bool witness(int a, int n) // 对n是否为素数进行判断，随机数为a
{
	int y = 1, b = n - 1;
	while (b)
	{
		if (b & 1)
		{
			y = y * a % n;
		}
		int x = a;
		a = a * a % n; // 判断 x^2 mod n =1？
		if (a == 1 && x != 1 && x != n - 1)
		{
			return false;
		}
		b >>= 1;
	}
	if (y != 1)
	{
		return false;
	}
	return true;
}
int main()
{
	int n;
	printf("需要测试的大素数为："); // 判断25409是否为素数
	scanf("%d", &n);
	printf("\n\n");

	// 随机数a取值
	int a = 0;
	int i;

	for (i = 0; i < time; i++)
	{
		a = rand() % n;
		printf("随机数a=%d,", a);
		if (witness(a, n) == false)
			printf("%d是合数\n", n);
		else
			printf("%d是素数\n", n);
	}
	return 0;
}
