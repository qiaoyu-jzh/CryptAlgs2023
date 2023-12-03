/*Elgamal算法实现*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
typedef unsigned int unit32;
unit32 Powmod(unit32 a, unit32 t, unit32 n) // 快速指数算法计算以a为底的t次幂模n的值
{
	int temp, i;
	int b[64] = {0};
	temp = t;
	i = 0;
	while (temp > 0)
	{
		b[i] = temp % 2;
		temp = temp / 2;
		i++;
	}

	// 取k值（二进制数组中最大非零的下标）
	int k = 0;
	for (i = 63; i >= 0; i--)
	{
		if (b[i] == 1)
		{
			k = i;
			break;
		}
	}
	// printf("k=%d\n\n",k);

	// 快速指数算法
	int j = 0;
	int x = 1;
	for (i = k; i >= 0; i--)
	{
		j = 2 * j;
		x = (x * x) % n;
		if (b[i] == 1)
		{
			j++;
			x = (x * a) % n;
		}
	}
	return x;
}

unit32 Ex_Euclid(unit32 f, unit32 d) // 求d模f的逆元
{
	int x1, x2, x3;
	int y1, y2, y3;
	int t1, t2, t3;
	int q;

	x1 = 1;
	x2 = 0;
	x3 = f;

	y1 = 0;
	y2 = 1;
	y3 = d;

	int flag = 1;
	while (flag)
	{
		if (y3 == 1)
		{
			flag = 0;
			if (y2 < 0)
			{
				y2 = y2 + f; // 若为负数，则加f变为整数即可
			}
		}
		if (y3 > 1)
		{
			q = x3 / y3;

			t1 = x1 - q * y1;
			t2 = x2 - q * y2;
			t3 = x3 - q * y3;

			x1 = y1;
			x2 = y2;
			x3 = y3;

			y1 = t1;
			y2 = t2;
			y3 = t3;
		}
	}
	return y2;
}

// unit32 Primitive_root(unit32 p)
// {

// }
unit32 Primitive_root(unit32 p) // 求数p的本原根
{
	// 线性筛选素数---------------------
	bool isnp[p];		// 是否为素数
	int prime[p] = {0}; // 素数表
	int pNum = 0;		// 素数表下标
	isnp[1] = true;
	for (int i = 2; i < p; i++)
	{
		if (!isnp[i])
		{					   // 在isnp[i]==0时进入
			isnp[i] = 1;	   // 是素数标记1
			prime[pNum++] = i; // 是素数的话就保存到prime表里
		}
		for (int j = 0; j < pNum && i * prime[j] < p; j++)
		{
			isnp[i * prime[j]] = 1; // 把已登记质数的所有倍数，筛选掉，不再登记prime
			if (i % prime[j] == 0)
				break; // 避免重复筛选，比如6被2筛掉后，不会再被3筛掉
		}
	}
	//------------------------------------
	// 求p-1的质因子并判断是不是本原根
	int elem[p] = {0}, elemNum = 0, K = p - 1;
	for (int i = 0; i < pNum; i++) // 找出K=p-1能整除的素数
	{

		if (!(K % prime[i])) // 当prime[i]是k的因子时进入
		{
			elem[elemNum++] = prime[i];
			K /= prime[i];
		}
		if (K == 1)
			break;
		if (K < prime[i])
		{
			// elem[elemNum++] = prime[i];
			break;
		}
	}
	int g = 0;
	bool flag;
	for (int i = 2; i < p; i++)
	{
		flag = true;
		for (int j = 0; j < elemNum; j++)
		{

			if (Powmod(i, (p - 1) / elem[j], p) == 1) // 判断本原根
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			g = i;
			break;
		}
	}
	return g;
}

//**********************************Elgamal算法参数和加解密****************************//
unit32 p = 127;				  // 素数
unit32 g = Primitive_root(p); // 本原根
unit32 r = 0, s[1024] = {0};  // 密文全局变量

void Elgamal_Byte_Encrypto(char *input, char *output, unit32 puk)
{ // 输入：明文字符串input，公钥puk
	// 输出：密文字符串output
	int i = 0;
	unit32 k = rand() % (p - 1) + 1; // 随机加密数K
	while (input[i] != '\0')
	{
		output[i] = (int)input[i];
		s[i] = output[i] * Powmod(puk, k, p) % p;
		output[i] = (char)s[i];
		i++;
	}
	r = Powmod(g, k, p);
}

void Elgamal_Byte_DEcrypto(char *input, char *output, unit32 prk)
{ // 输入：密文input，私钥prk
	// 输出：解密后的明文output
	int i = 0;
	unit32 R = Powmod(r, prk, p);
	while (input[i] != '\0')
	{
		output[i] = (int)input[i];
		s[i] = output[i] * Ex_Euclid(p, R) % p;
		output[i] = (char)s[i];
		i++;
	}
}

int main()
{
	unit32 a, b; // 私钥a，加密随机数b
	unit32 m;	 // 明文
	unit32 temp = 0;

	srand(time(NULL));		  // 随机数种子为当前时间time(NULL)
	a = rand() % (p - 1) + 1; // 私钥0<a<p
	b = Powmod(g, a, p);	  // 公钥

	printf("/*******************Elgamal算法加解密****************/\n");
	printf("素数p:%d，生成元g：%d，私钥α:%d，公钥β：%d\n", p, g, a, b);

	char text[100] = "Elgamal algorithm not only used for data encryption can be used for signatures.";
	char cipher[200] = {0};
	printf("\n原文为：%s\n", text);
	Elgamal_Byte_Encrypto(text, cipher, b);
	printf("\n密文为：%s\n", cipher);
	Elgamal_Byte_DEcrypto(cipher, text, a);
	printf("\n解密原文为：%s\n", text);

	return 0;
}
