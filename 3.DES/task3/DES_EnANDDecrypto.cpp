/*DES算法实现
DES是对称结构加解密算法是一样的，整个算法大的方面只包含密钥生成和加解密两部分
密钥生成：PC1+LS+PC2
加解密：IP+轮函数(F函数+异或)+ IP-1
F函数：E+异或+S-Boxes+P
*/
#include <stdio.h>
#include <memory.h> //memcpy所需头文件

//****************************固定参数********************//
// 扩展置换表E,32-48
int E_Table[48] = {31, 0, 1, 2, 3, 4,
				   3, 4, 5, 6, 7, 8,
				   7, 8, 9, 10, 11, 12,
				   11, 12, 13, 14, 15, 16,
				   15, 16, 17, 18, 19, 20,
				   19, 20, 21, 22, 23, 24,
				   23, 24, 25, 26, 27, 28,
				   27, 28, 29, 30, 31, 0};

// 置换表P,32-32
int P_Table[32] = {15, 6, 19, 20, 28, 11, 27, 16,
				   0, 14, 22, 25, 4, 17, 30, 9,
				   1, 7, 23, 13, 31, 26, 2, 8,
				   18, 12, 29, 5, 21, 10, 3, 24};

// S盒
int S[8][4][16] = {
	// S1
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
	// S2
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
	// S3
	{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
	 {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
	 {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
	 {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
	// S4
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
	// S5
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
	// S6
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
	// S7
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
	// S8
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

// 置换选择PC1,64-56
int PC1_Table[56] = {56, 48, 40, 32, 24, 16, 8,
					 0, 57, 49, 41, 33, 25, 17,
					 9, 1, 58, 50, 42, 34, 26,
					 18, 10, 2, 59, 51, 43, 35,
					 62, 54, 46, 38, 30, 22, 14,
					 6, 61, 53, 45, 37, 29, 21,
					 13, 5, 60, 52, 44, 36, 28,
					 20, 12, 4, 27, 19, 11, 3};

// 置换选择PC2,56-48
int PC2_Table[48] = {13, 16, 10, 23, 0, 4, 2, 27,
					 14, 5, 20, 9, 22, 18, 11, 3,
					 25, 7, 15, 6, 26, 19, 12, 1,
					 40, 51, 30, 36, 46, 54, 29, 39,
					 50, 44, 32, 46, 43, 48, 38, 55,
					 33, 52, 45, 41, 49, 35, 28, 31};

int MOVE_TIMES[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// 初始置换表IP
int IP_Table[64] = {57, 49, 41, 33, 25, 17, 9, 1,
					59, 51, 43, 35, 27, 19, 11, 3,
					61, 53, 45, 37, 29, 21, 13, 5,
					63, 55, 47, 39, 31, 23, 15, 7,
					56, 48, 40, 32, 24, 16, 8, 0,
					58, 50, 42, 34, 26, 18, 10, 2,
					60, 52, 44, 36, 28, 20, 12, 4,
					62, 54, 46, 38, 30, 22, 14, 6};
// 逆初始置换表IP^-1
int IP_1_Table[64] = {39, 7, 47, 15, 55, 23, 63, 31,
					  38, 6, 46, 14, 54, 22, 62, 30,
					  37, 5, 45, 13, 53, 21, 61, 29,
					  36, 4, 44, 12, 52, 20, 60, 28,
					  35, 3, 43, 11, 51, 19, 59, 27,
					  34, 2, 42, 10, 50, 18, 58, 26,
					  33, 1, 41, 9, 49, 17, 57, 25,
					  32, 0, 40, 8, 48, 16, 56, 24};

// 8个字节转换成64个二进制位存储在字节数组
int Char8_to_Bit64(char in[8], char out[64])
{
	for (int i = 0; i < 8; i++) // 8个字节
	{
		for (int j = 0; j < 8; j++) // 每个字节8位
		{
			out[8 * i + j] = (in[i] >> j) & 0x01; // 右移取最低位
		}
	}
	return 0;
}

int Bit64_to_Char8(char in[64], char out[8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			out[i] = out[i] | (in[8 * i + j]) << j;
		}
	}
	return 0;
}

//*******************轮函数扩展变换********************//
int E_Transform(char in[32], char out[48])
{ // E变换输入32bit扩展为48比特
	for (int i = 0; i < 48; i++)
	{
		out[i] = in[E_Table[i]];
	}

	return 0;
}

int Sbox(char in[48], char out[32])
{								// S盒变换，输出为48bit，输出为32bit
	for (int i = 0; i < 8; i++) // 输入in的48个元素，分成8组，每组6个。每组的第一个和最后一个元素构成S盒的行row，中间4个元素构成列col
	{
		int row = in[i * 6] * 2 + in[i * 6 + 5];
		int col = in[i * 6 + 1] * 8 + in[i * 6 + 2] * 4 + in[i * 6 + 3] * 2 + in[i * 6 + 4];
		int value = S[i][row][col];	  // 在S盒中取出值
		out[i * 4 + 3] = value & 0x1; // 将S盒的值转化成4位二进制数，利用位操作符
		out[i * 4 + 2] = (value & 0x2) >> 1;
		out[i * 4 + 1] = (value & 0x4) >> 2;
		out[i * 4] = (value & 0x8) >> 3;
	}

	return 0;
}

int P_Transform(char in[32], char out[32])
{ // P置换
	for (int i = 0; i < 32; i++)
	{
		out[i] = in[P_Table[i]];
	}

	return 0;
}

int F_Function(char in[32], char out[32], char subkey[48])
{ // F函数
	char E_re[48] = {0}, S_re[32] = {0};
	E_Transform(in, E_re);
	for (int i = 0; i < 48; i++)
	{
		E_re[i] = E_re[i] ^ subkey[i];
	}
	Sbox(E_re, S_re);
	P_Transform(S_re, out);
	return 0;
}

int Round_Function(char in[64], char out[64], char subkey[48])
{ // 轮函数
	char F_re[32] = {0}, R[32] = {0}, L[32] = {0};
	for (int i = 0; i < 32; i++)
	{
		R[i] = in[32 + i];
		L[i] = in[i];
	}
	F_Function(R, F_re, subkey);
	for (int i = 0; i < 32; i++)
	{
		out[i] = R[i];
		out[i + 32] = F_re[i] ^ L[i];
	}

	return 0;
}

//****************************秘钥扩展****************************//
int PC_1(char in[64], char out[56])
{ // PC1置换
	for (int i = 0; i < 56; i++)
	{
		out[i] = in[PC1_Table[i]];
	}

	return 0;
}

int PC_2(char in[56], char out[48])
{ // PC2置换
	for (int i = 0; i < 48; i++)
	{
		out[i] = in[PC2_Table[i]];
	}

	return 0;
}

// 循环左移，56bit分左右两部分循环移位time次
int ROL(char in[56], int time)
{ // 循环左移，56bit分左右两部分循环移位time次
	for (int i = 0; i <= 28 - time; i++)
	{
		in[i] = in[(i + time) % 28];
	}
	for (int i = 0; i <= 56 - time; i++)
	{
		in[i] = in[(i + time) % 56];
	}

	return 0;
}

// 秘钥扩展，8字节(64bit)主秘钥，生成16轮48bit子秘钥
int Key_Expand(char key[8], char subkey[16][48])
{
	char in[64], out[56];
	Char8_to_Bit64(key, in);

	PC_1(in, out);
	for (int i = 0; i < 16; i++)
	{
		ROL(out, MOVE_TIMES[i]);
		PC_2(out, subkey[i]);
	}
	return 0;
}

//**************************其他变换部分*********************//
int IP_Transform(char data[64])
{
	char temp[64];
	for (int i = 0; i < 64; i++)
	{
		temp[i] = data[IP_Table[i]];
	}
	memcpy(data, temp, 64);
	return 0;
}

int IP_1_Transform(char data[64]) // IP逆置换
{
	char temp[64];
	for (int i = 0; i < 64; i++)
	{
		temp[i] = data[IP_1_Table[i]];
	}
	memcpy(data, temp, 64);
	return 0;
}

int Swap(char left[32], char right[32]) // 交换
{
	char temp[32];
	memcpy(temp, left, 32);
	memcpy(left, right, 32);
	memcpy(right, temp, 32);
	return 0;
}
int swap64(char in[64],char out[64])
{
	// char temp[64];
	for (int i = 0; i < 64;i++)
	{
		out[i] = in[i];
	}
	return 0;
}

//***************************DES加密***************************//
int Encrypt(char in[8], char key[8], char out[8])
{//加密过程
	char M[64],C[64];
	char subkey[16][48]={0};
	Char8_to_Bit64(in, M);//明文64位二进制
	IP_Transform(M);//IP置换
	Key_Expand(key, subkey);
	for (int i = 0; i < 16;i++)
	{
		Round_Function(M, C, subkey[i]);//16轮 轮函数
		swap64(C, M);					// 要交换M，C的值
	}
	swap64(M, C);
	char left[32], right[32];
	for (int i = 0; i < 32;i++)
	{
		left[i] = C[i];
		right[i] = C[i + 32];
	}
	Swap(left, right);// 交换结果
	for (int i = 0; i < 32;i++)
	{
		C[i] = left[i];
		C[i + 32] = right[i];
	}
	IP_1_Transform(C);// IP逆置换
	Bit64_to_Char8(C, out);//输出结果
	return 0;
}
//***************************DES解密****************************//
int Decrypt(char in[8], char key[8], char out[8])
{//加密过程
	char M[64],C[64];
	char subkey[16][48]={0};
	Char8_to_Bit64(in, M);//明文64位二进制
	IP_Transform(M);//IP置换
	Key_Expand(key, subkey);
	for (int i = 0; i < 16;i++)
	{
		Round_Function(M, C, subkey[15-i]);//16轮 轮函数,密钥反序
		swap64(C, M);					// 要交换M，C的值
	}
	swap64(M, C);
	char left[32], right[32];
	for (int i = 0; i < 32;i++)
	{
		left[i] = C[i];
		right[i] = C[i + 32];
	}
	Swap(left, right);// 交换结果
	for (int i = 0; i < 32;i++)
	{
		C[i] = left[i];
		C[i + 32] = right[i];
	}
	IP_1_Transform(C);// IP逆置换
	Bit64_to_Char8(C, out);//输出结果

	return 0;
}

int main()
{
	char text[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
	char key[8] = {'2', '0', '2', '3', '1', '0', '2', '3'};
	char cipher[8] = {0};
	char plain[8] = {0};

	printf("-------------------DES算法演示------------------\n");
	printf("原文：\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%c ", text[i]);
	}
	printf("\n");
	printf("秘钥：\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%c ", key[i]);
	}
	printf("\n");

	// 加密
	Encrypt(text, key, cipher);
	printf("密文：\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%x ", cipher[i]);
	}
	printf("\n");
	// 解密
	Decrypt(cipher, key, plain);
	printf("解密明文：\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%c ", plain[i]);
	}
	printf("\n");
	return 0;
}
