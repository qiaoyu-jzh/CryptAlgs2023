/*SM3杂凑函数实现*/

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef unsigned char unit8;
typedef unsigned int unit32;

#define L2B32(Little) (((Little & 0xff) << 24) | (((Little) & 0xff00) << 8) | (((Little) & 0xff0000) >> 8) | ((Little >> 24) & 0xff)) // 32bit data change to bigend

unit32 RotL(unit32 a, unit32 k)
{
	k = k % 32;
	return (((a) << k) | ((a) >> (32 - k)));
}

unit32 T_Table[2] = {0x79CC4519, 0x7A879D8A};
unit32 T(int j) // T init
{
	if (j >= 0 && j <= 15)
		return T_Table[0];
	else if (j >= 16 && j <= 63)
		return T_Table[1];
	else
		return 0;
}
unit32 IV[8] = {0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600,
				0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E};

unsigned char PADDING[] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // data to pad

unit32 FF(unit32 X, unit32 Y, unit32 Z, int j)
{
	if (j >= 0 && j < 16)
		return X ^ Y ^ Z;
	if (j >= 16 && j < 64)
		return ((X & Y) | (X & Z) | (Y & Z));
	return 0;
}

unit32 GG(unit32 X, unit32 Y, unit32 Z, int j)
{
	if (j >= 0 && j < 16)
		return X ^ Y ^ Z;
	if (j >= 16 && j < 64)
		return ((X & Y) | (~X & Z));
	return 0;
}

unit32 P0(unit32 X)
{
	return X ^ RotL(X, 9) ^ RotL(X, 17);
}

unit32 P1(unit32 X)
{
	return X ^ RotL(X, 15) ^ RotL(X, 23);
}

int Message_Pad(unit8 *input, int m_length, unit8 *output)
{
	// 输入任意字符串，输出扩展后的字节流，并返回分组B的数量
	// 打印输出填充后的消息
	memcpy(output, input, m_length);

	unit8 padlen = (64 - m_length % 64 - 8) % 64; // paddata length
	int n = (m_length + padlen + 8) / 64;
	int temp = m_length * 8; // bitnumber
	// consider padlen less than 64bit
	if (padlen < 8)
	{
		padlen = 64 + padlen;
		memcpy(output + m_length, PADDING, padlen);
		unit32 bit_length = L2B32(temp);
		memcpy(output + m_length + padlen - 4, &bit_length, 4);
	}
	memcpy(output + m_length, PADDING, padlen + 4);
	unit32 bit_length = L2B32(temp);
	memcpy(output + m_length + padlen + 4, &bit_length, 4);

	printf("\n填充后的消息为：");
	for (int i = 0; i < n * 64; i++)
	{
		if (i % 32 == 0)
			printf("\n");
		printf("%02x", output[i]);
		if ((i + 1) % 4 == 0)
			printf("  ");
	}
	return n; // numbers of B
}

int Message_Ex(unit8 *input, unit32 w[132])
{
	// 输入填充后的字节流及其字节数，输出扩展后的字节流
	// 打印输出扩展后的消息
	for (int i = 0; i < 16; i++)
	{
		w[i] = (input[i * 4] << 24) & 0xff000000 | (input[i * 4 + 1] << 16) & 0x00ff0000 | (input[i * 4 + 2] << 8) & 0x0000ff00 | (input[i * 4 + 3]) & 0x000000ff;
	}
	for (int i = 16; i < 68; i++)
	{
		w[i] = P1(w[i - 16] ^ w[i - 9] ^ (RotL(w[i - 3], 15))) ^ (RotL(w[i - 13], 7)) ^ w[i - 6];
	}
	for (int i = 68; i < 132; i++)
	{
		w[i] = w[i - 68] ^ w[i - 64];
	}

	printf("\n\n扩展后的消息为：");
	for (int i = 0; i < 132; i++)
	{
		if (i % 8 == 0)
			printf("\n");
		printf("%08x  ", w[i]);
	}

	// for (int i = 0; i < 132; i++)
	// {
	// 	unit32 temp = w[i];
	// 	w[i] = L2B32(temp); // bigend
	// }
	return 0;
}

void CF(unit32 r[8], unit32 w[132], unit32 v[8])
{
	// 压缩函数，输入字r和Bi，其中r是A-H8个寄存器的值，输出v
	// 打印输出中间各轮迭代压缩中间值
	// A=r0,B=r1,C=r2,D=r3,E=r4,F=r5,G=r6,H=r7
	unit32 vi[8] = {0};
	for (int i = 0; i < 8; i++)
	{
		vi[i] = r[i]; // 记下初值
	}
	unit32 SS1, SS2, TT1, TT2;
	printf("\n压缩中间值：\nA	B 	C	D	E	F	G	H\n");
	for (int j = 0; j < 64; j++)
	{
		SS1 = RotL(r[0], 12) + r[4] + RotL(T(j), (j % 32));
		SS1 = RotL(SS1, 7);
		SS2 = SS1 ^ RotL(r[0], 12);
		TT1 = FF(r[0], r[1], r[2], j) + r[3] + SS2 + w[j + 68];
		TT2 = GG(r[4], r[5], r[6], j) + r[7] + SS1 + w[j];
		r[3] = r[2];
		r[2] = RotL(r[1], 9);
		r[1] = r[0];
		r[0] = TT1;
		r[7] = r[6];
		r[6] = RotL(r[5], 19);
		r[5] = r[4];
		r[4] = P0(TT2);
		for (int i = 0; i < 8; i++)
		{
			printf("%08x  ", r[i]);
		}
		printf("\n");
	}

	for (int i = 0; i < 8; i++)
	{
		v[i] = vi[i] ^ r[i];
	}
}

void sm3(unit8 *input, int m_length, unit32 output[8])
{
	// 输入明文字节流和明文长度，输出杂凑值

	// step1: 消息填充
	unit8 padlen = (64 - m_length % 64 - 8) % 64;
	unit8 m_output[m_length + padlen + 8];
	int n;										// 填充后的分组数
	n = Message_Pad(input, m_length, m_output); // 消息填充

	// step2: 扩展压缩
	unit8 B[64] = {0};
	unit32 W[132] = {0};

	// init A_H
	unit32 r[8] = {0};
	for (int i = 0; i < 8; i++)
	{
		// unit32 temp = IV[i];
		// r[i] = L2B32(temp);

		r[i] = IV[i]; // 寄存器赋初值
	}

	// unit32 V[8] = {0}; // CF result
	for (int i = 0; i < n; i++)
	{
		memcpy(B, m_output + i * 64, 64);
		Message_Ex(B, W);
		CF(r, W, output);
		for (int j = 0; j < 8; j++) // 上一次结果作为下一次输入，赋值A_H寄存器
		{
			r[j] = output[j];
		}
	}
}

int main()
{

	unit8 *text = (unit8 *)"abc";
	// unit8 *text=(unit8 *) "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
	unit32 output[8] = {0};
	printf("原文是：%s\n", text);

	unit8 temp_pad[1024] = {0};
	int m_length = 0;
	for (m_length = 0; text[m_length] != '\0'; m_length++)
		; // 明文长度

	sm3(text, m_length, output);
	printf("\n杂凑值:\n  ");
	for (int i = 0; i < 8; i++)
	{
		printf("%08x  ", output[i]);
	}
	printf("\n");

	return 0;
}
