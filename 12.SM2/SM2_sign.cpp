/*SM2数字签名*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ECC.h"
#include "sm3.h"

//! sm2数字较大，避免用int类型，强制使用 unsigned int

// 系统参数，P,a,b,G,n,见ecc.h
unit8 ID[18] = {0x41, 0x4C, 0x49, 0x43, 0x45, 0x31, 0x32, 0x33, 0x40, 0x59, 0x41, 0x48, 0x4F, 0x4F, 0x2E, 0x43, 0x4F, 0x4D};
unit8 ENTLA[2] = {0x00, 0x90};
int dA = 0;
int PA[2] = {0};

void Initial() // 初始化生成私钥dA，公钥PA
{
	srand((unsigned)time(NULL));
	dA = rand() % (n - 1) + 1; // 随机私钥
	ECC_Multi(G, dA, PA);	   // 生成公钥
}

void Generate_ZA(int puk[2], unit8 *output)
{ // 根据参数生成ZA

	// 字节流的串联,字节长度为1+18+4*6=43
	unit8 Zadata[44] = {0};
	memcpy(Zadata, ENTLA, 2);
	memcpy(Zadata + 2, &ID, 18);
	memcpy(Zadata + 20, &a, 4);
	memcpy(Zadata + 24, &b, 4);
	memcpy(Zadata + 28, &G[0], 4);
	memcpy(Zadata + 32, &G[1], 4);
	memcpy(Zadata + 36, &puk[0], 4);
	memcpy(Zadata + 40, &puk[1], 4);
	sm3(Zadata, 44, output);
	// za长度为256bit 32byte
	// printf("zadata:\n");
	// for (int i = 0; i < 43; i++)
	// {
	// 	printf("%2x", Zadata[i]);
	// }
	printf("\n");
}

void Compress(unit8 *input, unit8 *za, unit8 *output) // 压缩函数，将消息M和ZA用SM3求哈希值输出到output中
{
	int length = 0;
	for (length = 0; input[length] != '\0'; length++)
		;
	memcpy(za + 32, input, length);
	sm3(za, length + 32, output); // output为e,e为hash值
}

void Sign(unit8 *input, int da, unit8 *output) // 明文，私钥，生成签名
{
	// get za
	unit8 ZA[32] = {0}; // 256bit
	Generate_ZA(PA, ZA);
	printf("sign函数的ZA_hash值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("%2x", ZA[i]);
	}
	printf("\n");

	// get e
	unit8 e[32] = {0}; // 256bit
	Compress(input, ZA, e);
	printf("sign函数的e_hash值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("%2x", e[i]);
	}
	printf("\n");

	// get r
	srand((unsigned)time(NULL));

	unit32 k = 0, em = 0, r = 0;
	int kG[2] = {0};
	k = rand() % (n - 1) + 1;
	bool flag = true;
	while (flag)
	{
		ECC_Multi(G, k, kG);
		memcpy(&em, e, 4);
		r = ((em + kG[0]) % n + n) % n; // kG[0]是int类型，4个字节
		flag = false;
		if (r == 0 || r == n - k)
		{
			flag = true;
		}
	}

	// get s
	unit32 y2 = 0, s = 0;
	y2 = Ex_Euclid(n, (da + 1));
	s = ((k - (r * da) % n) + n) % n;
	s = (y2 * s) % n; // 4个字节
	// s = (s % n + n) % n;
	printf("Sign函数里的r和s是：\n");
	printf("r=%2x,s=%2x\n", r, s);

	// output (r,s)
	memcpy(output, &r, 4);
	memcpy(output + 4, &s, 4);
}

bool Verify(unit8 *input, unit8 *sign, int pa[2]) // 明文，公钥，验证签名
{
	// get za
	unit8 ZA[32] = {0}; // 256bit
	Generate_ZA(PA, ZA);
	printf("Verify函数的ZA_hash值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("%2x", ZA[i]);
	}
	printf("\n");

	// get e
	unit8 e[32] = {0}; // 256bit
	Compress(input, ZA, e);
	printf("Verify函数的e_hash值：\n");
	for (int i = 0; i < 32; i++)
	{
		printf("%2x", e[i]);
	}
	printf("\n");

	unit32 em = 0;
	memcpy(&em, e, 4);
	// r,s
	unit32 r, s = 0;
	memcpy(&r, sign, 4);
	memcpy(&s, sign + 4, 4);
	printf("Verify函数里的r和s是：\n");
	printf("r=%2x,s=%2x\n", r, s);
	if ((r < 1 || r > (n - 1)) || (s < 1 || s > (n - 1)))
		return false;

	int t = (((r + s) % n) + n) % n;
	printf("\nt=%x\n", t);
	if (!t)
		return false;

	int sG[2], tPa[2], temp[2] = {0};
	ECC_Multi(G, s, sG);
	ECC_Multi(pa, t, tPa);
	ECC_Add(sG, tPa, temp);
	int R = 0;
	R = (em + temp[0]) % n;
	printf("R= %2x\n", R);
	if (R == r)
		return true;
	else
		return false;
}

int main()
{
	Initial(); // 初始化公私钥
	unit8 message[100] = "abc";
	printf("\n消息：%s\n", message);
	unit8 sign[8] = {0};
	Sign(message, dA, sign);
	printf("\n消息签名：\n");
	for (int i = 0; i < 8; i++)
		printf(" %02x", sign[i]);
	printf("\n\n");
	if (Verify(message, sign, PA))
	{
		printf("\n验证消息签名结果：\n");
		printf("True\n");
	}
	else
	{
		printf("\n验证消息签名结果：\n");
		printf("False\n");
	}

	return 0;
}
