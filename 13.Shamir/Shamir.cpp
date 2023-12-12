/*Shamir秘钥共享
求多项式的拉格朗日插值公式方法
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Elgamal.h"

// 全局变量
#define P 19 // 素数P

void Initial(int s, int t, int *a) // 输入秘密数s和门限值t,输出随机生成的t-1次多项式系数数组a
{
	// a[] save a(0)-a(t-1)
	a[0] = s;
	srand((unsigned)time(NULL));
	for (int i = 1; i < t; i++)
	{

		a[i] = rand() % (P - 1) + 1;
	}
}

void Separate_Key(int s, int t, int n, int (*point)[2]) // 输入秘密数s、n和门限值t，输出随机生成的n个公私钥值对
{
	int a[t] = {0};
	Initial(s, t, a); // 生成ai

	// n个互不相同的随机整数 x%P 生成xi
	// int *x = (int(*))malloc(sizeof(int) * n);
	int x[n] = {0};
	srand((unsigned)time(NULL));
	for (int i = n - 1; i >= 0; i--)
	{
		x[i] = rand() % (P - 1) + 1;
		for (int j = n - 1; j > i; j--)
		{
			if (x[j] == x[i])
			{
				i += 1;
				break;
			}
		}
		// n--;
	}

	// generate (xi,yi)   yi=a0+a1*x1+...(at-1)*(xt-1)^(t-1)
	for (int i = 0; i < n; i++)
	{
		point[i][0] = x[i];
		for (int j = 1; j < t; j++)
		{
			point[i][1] += Powmod(x[i], j, P) * a[j];
		}
		point[i][1] += a[0];
		point[i][1] %= P;
	}
}

int Reconstruct_Key(int t, int (*point)[2]) // 秘钥重构，输入n个子秘钥对，返回秘密数
{
	// 重构就是求出a[0]的值，需要t个（xi,yi）
	// t个随机数（n范围内的）
	// int index[t] = {0};
	// srand((unsigned)time(NULL));
	// for (int i = 0; i < t;i++){
	// 	index[i]=rand() % (n - 1) + 1;
	//
	// 默认用前6个（xi,yi）
	int k = 0;
	for (int s = 0; s < t; s++)
	{
		int val = point[s][1];
		for (int j = 0; j < t; j++)
		{
			if (j == s)
				continue;
			val *= (-point[j][0]);
			val %= P;
			int temp = (point[s][0] - point[j][0] + P) % P;
			val *= Ex_Euclid(P, temp);
			val %= P;
		}
		k += val;
		k = (k + P) % P;
	}

	return k;
}

void Display(int (*point)[2], int n)
{
	printf("\n分割后的子秘钥对为：\n");
	for (int i = 0; i < n; i++)
	{
		printf("  (%d,%d)", point[i][0], point[i][1]);
	}
	printf("\n");
}

int main()
{
	int t = 6, n = 8, s = 13;
	printf("Shamir门限方案(%d,%d)，秘密数k:%d\n", t, n, s);
	int(*point)[2]; // save (xi,yi)
	point = (int(*)[2])malloc(sizeof(int) * n * 2);
	Separate_Key(s, t, n, point);
	Display(point, n);

	int k = Reconstruct_Key(t, point);
	printf("\n重构获得的秘密数k:%d\n", k);

	return 0;
}
