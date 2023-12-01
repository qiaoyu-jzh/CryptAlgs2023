/*置换密码
按照给定数组顺序进行变换操作

要求：
1.可对任意字符进行加解密（字母、数字、符号）；
2.密文和解密后的明文中所有英文字母全部为小写，其他字符保持不变；
*/

#include <stdio.h>
#include <string.h>

int encrypto(char *input, char *output, int T[])
{
	// 输入：明文和加密变换表T   输出：置换变换后的密文

	for (int i = 0; input[i] != '\0'; i++)
	{
		if (input[i] >= 'A' && input[i] <= 'Z')
		{
			input[i] += 32;
		}
		output[i] = input[T[i % 10] + (i / 10) * 10];
	}
	return 0;
}

int decrypto(char *input, char *output, int T[])
{
	// 输入：密文和加密变换表T   输出：明文
	// 解密的变换表T‘需要根据加密变换表T来构造

	for (int i = 0; input[i] != '\0'; i++)
	{
		if (input[i] >= 'A' && input[i] <= 'Z')
		{
			input[i] += 32;
		}
		output[i] = input[T[i % 10] + (i / 10) * 10];
	}
	return 0;
}
int T_decryp(int T[], int len)
{
	// 求解密置换表
	int T1[len];
	for (int i = 0; i < len; i++)
	{
		T1[T[i]] = i;
	}
	for (int i = 0; i < len; i++)
	{
		T[i] = T1[i];
	}
	return 0;
}

void fk(char input[], char output[]) // 补齐空格
{
	int i, j;
	int len = 0;
	for (i = 0; input[i] != '\0'; i++)
	{
		len++;
	}
	if (len % 10 != 0)
	{
		for (j = 0; j < (len / 10 + 1) * 10; j++)
		{
			if (j < len)
				output[j] = input[j];
			else
				output[j] = ' ';
		}
	}
	else
		for (j = 0; j < len; j++)
		{
			output[j] = input[j];
		}
}
int main()
{
	char text[1024] = "There are two main methods of classical cipher transformation, substitution and transposition.";
	char text_space[1024] = "";
	char cipher_text[1024] = "";
	char plain_text[1024] = "";
	int T[10] = {3, 4, 7, 6, 2, 9, 1, 8, 0, 5};

	printf("------------置换密码-----------\n");
	printf("原文:%s\n", text);
	printf("换位操作顺序:%");
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", T[i]);
	}
	printf("\n\n");

	fk(text, text_space);

	encrypto(text_space, cipher_text, T);
	printf("密文为:%s\n", cipher_text);

	T_decryp(T, 10); // 解密
	printf("\n解密操作顺序:%");
	for (int i = 0; i < 10; i++)
	{
		printf("%d  ", T[i]);
	}
	printf("\n");
	decrypto(cipher_text, plain_text, T);
	printf("解密后明文为:%s\n", plain_text);
	return 0;
}
