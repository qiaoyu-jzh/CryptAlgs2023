#include <stdio.h>
int ExEuclid(int f, int d) // 扩展欧几里德算法
{						   /*输入：两个正整数，要求f>d；
						   输出 :f和d的最大公因子，若f和d互素进一步输出d关于f的逆元*/
	int x1 = 1, x2 = 0, x3 = f;
	int y1 = 0, y2 = 1, y3 = d;
	while (1)
	{
		if (y3 == 0)
		{
			return y3;
			break;
		}
		if (y3 == 1)
		{
			if (y2 < 0)
			{
				y2 = y2 + f;
			}
			return y2;
			break;
		}

		int Q = x3 / y3;
		int t1 = x1 - Q * y1, t2 = x2 - Q * y2, t3 = x3 - Q * y3;
		x1 = y1, x2 = y2, x3 = y3;
		y1 = t1, y2 = t2, y3 = t3;
	}
}

int crt(int a[], int m[], int n) // 中国剩余定理
{								 /*输入：方程组数n，余数集合a，模数集合m
								 输出：重构得出的数*/
	int M = 1, MI[n] = {0}, e[n] = {0}, temp = 0;
	for (int i = 0; i < n; i++)
	{
		M = M * m[i];
	}

	for (int i = 0; i < n; i++)
	{
		MI[i] = M / m[i];
		e[i] = ExEuclid(m[i], MI[i]);
		temp += MI[i] * e[i] * a[i];
	}
	// printf("temp=%d\n", temp);

	return temp % M;
}

int main()
{
	int N = 4;
	int i, ans;
	int a[4] = {1, 2, 3, 5};
	int m[4] = {2, 3, 5, 7};

	for (i = 0; i < N; i++)
	{
		printf("模%d余%d	", m[i], a[i]);
	}

	ans = crt(a, m, N);

	printf("\n通过中国剩余定理计算得到的结果为：%d\n", ans); // 模210下等于173
	return 0;
}
