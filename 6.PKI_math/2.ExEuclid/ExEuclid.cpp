#include <stdio.h>
void ExEuclid(int f, int d) // 扩展欧几里德算法
{							/*输入：两个正整数，要求f>d；
							输出 :f和d的最大公因子，若f和d互素进一步输出d关于f的逆元*/
	int x1 = 1, x2 = 0, x3 = f;
	int y1 = 0, y2 = 1, y3 = d;
	while (1)
	{
		if (y3 == 0)
		{
			printf("gcd(%d,%d) = %d\n", f, d, x3);
			break;
		}
		if (y3 == 1)
		{
			if (y2<0)
			{
				y2 = y2 + f;
			}

			printf("gcd(%d,%d) = %d\n", f, d, y3);
			printf("%d 的逆元是 %d: \n", d, y2);
			break;
		}

		int Q = x3 / y3;
		int t1 = x1 - Q * y1, t2 = x2 - Q * y2, t3 = x3 - Q * y3;
		x1 = y1, x2 = y2, x3 = y3;
		y1 = t1, y2 = t2, y3 = t3;
	}
}
int main()
{
	// 求f,d的最大公因子
	//    int f=1769;
	//	int d=550;
	int f, d;
	printf("输入两个正整数：");
	scanf("%d,%d", &f, &d);
	ExEuclid(f, d);
}
