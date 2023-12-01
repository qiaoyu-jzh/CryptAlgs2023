#include <stdio.h>
int ExEuclid(int f, int d) // ��չŷ������㷨
{						   /*���룺������������Ҫ��f>d��
						   ��� :f��d��������ӣ���f��d���ؽ�һ�����d����f����Ԫ*/
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

int crt(int a[], int m[], int n) // �й�ʣ�ඨ��
{								 /*���룺��������n����������a��ģ������m
								 ������ع��ó�����*/
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
		printf("ģ%d��%d	", m[i], a[i]);
	}

	ans = crt(a, m, N);

	printf("\nͨ���й�ʣ�ඨ�����õ��Ľ��Ϊ��%d\n", ans); // ģ210�µ���173
	return 0;
}
