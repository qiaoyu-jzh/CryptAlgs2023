/*	椭圆曲线基本运算实现
	计算满足条件的椭圆曲线上的点
	方程：y2=x3+ax+b mod p，参数a,b,p
*/
#include <stdio.h>
#include <math.h>

int Point[100][2] = {0}; // 全局变量，满足条件的点的集合

int Get_ECC_Point(int a, int b, int p, int (*point)[2]) // 计算符合条件的所有点，存入Point中，并返回点的数目
{
	int flag = (4 * a * a * a + 27 * b * b) % p;
	if (flag == 0)
	{
		printf("Not Ellipse curves");
		return -1;
	}
	int i = 0;
	for (int x = 0; x < p; x++)
	{
		for (int y = 0; y < p; y++)
		{
			if ((y * y) % p == (x * x * x + a * x + b) % p)
			{
				point[i][0] = x;
				point[i][1] = y;
				i++;
			}
		}
	}
	return i;
}

int main()
{
	// 设置初始参数
	int a = 1, b = 1, p = 23;
	int n = 0;
	printf("\n椭圆曲线 y^2=x^3+%dx+%d mod %d上的点：\n", a, b, p);
	n = Get_ECC_Point(a, b, p, Point);
	for (int i = 0; i < n; i++)
	{
		printf("\t(%d,%d)", Point[i][0], Point[i][1]);
		if ((i + 1) % 6 == 0)
			printf("\n");
	}
	return 0;
}
