/*	椭圆曲线基本运算实现
	计算满足条件的椭圆曲线上的点
	方程：y2=x3+ax+b mod p，参数a,b,p
*/
#include <stdio.h>
#include <math.h>
#include "func.h"

int Point[100][2] = {0};  // 全局变量，满足条件的点的集合
int a = 1, b = 1, P = 23; // 设置初始参数
// unit32 Ex_Euclid(unit32 f, unit32 d);


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

void ECC_Add(int p[2], int q[2], int r[2])
{
	// 判断p、q是否相等p[0]==q[0]，分情况计算lamda
	int lamda = 0;
	if (p[0] != q[0])
	{
		lamda = (q[1] - p[1]) % P * Ex_Euclid(P, (q[0] - p[0]));
	}
	else
	{
		lamda = (3 * p[0] * p[0] + a) % P * Ex_Euclid(P, 2 * p[1]);
	}
	// 计算输出r的x值和y值
	r[0] = (lamda * lamda - p[0] - q[0]) % P;  // x
	r[1] = (lamda * (p[0] - r[0]) - p[1]) % P; // y
	if (r[1] < 0)
	{
		r[1] += P;
	}
}

void ECC_Multi(int p[2], int n, int q[2])
{
	// 利用加法函数，for循环p点累加n-1次输出
	for (int i = 0; i < n - 1; i++)
	{
		ECC_Add(p, p, q);
	}
}

int main()
{
	int point_num = 0;
	printf("\n椭圆曲线 y^2=x^3+%dx+%d mod %d上的点：\n", a, b, P);
	point_num = Get_ECC_Point(a, b, P, Point);
	for (int i = 0; i < point_num; i++)
	{
		printf("\t(%d,%d)", Point[i][0], Point[i][1]);
		if ((i + 1) % 6 == 0)
			printf("\n");
	}
	int p[2] = {3, 10}, q[2] = {9, 7}, r[2] = {0};
	ECC_Add(p, q, r);
	printf("\n加法运算：p=(%d,%d),q=(%d,%d),p+q=(%d,%d)\n", p[0], p[1], q[0], q[1], r[0], r[1]);
	int n = 2;
	ECC_Multi(p, n, r);
	printf("\n乘法运算：p=(%d,%d),n=%d,n*p=(%d,%d)\n", p[0], p[1], n, r[0], r[1]);

	return 0;
}
