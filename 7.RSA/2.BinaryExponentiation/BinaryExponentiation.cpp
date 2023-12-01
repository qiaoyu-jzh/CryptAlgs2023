// 快速指数
#include <stdio.h>
int FE(int a, int t, int n) // 返回值为a的t次方模n的值
{
	int x = 1;
	while (t)
	{
		if (t & 1)
		{
			int x = x * a % n;
		}
		x = x * x % n;
		t >>= 1;
	}
	return x;
}
int main()
{
	int a, t, n;

	printf("求解a的t次方模n的值：");
	scanf("%d,%d,%d", &a, &t, &n);
	printf("\n\n");

	int ans = 0;
	ans = FE(a, t, n);
	printf("%d的%d次方模%d的值为：%d", a, t, n, ans); // 66，77，119，结果为19
	return 0;
}
