/*多表代替密码矩阵实现
实现第10页【例1-2】多表代替
*/

#include <stdio.h>

#define N 3
#define NN 26

int A[N][N] = {
	11, 2, 19,
	5, 23, 25,
	20, 7, 17};

int A_[N][N] = {
	10, 23, 7,
	15, 9, 22,
	5, 9, 21};

int B[N] = {0, 0, 0};
int data_pre(char *input, int *M)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		// if (input[i] >= 'A' && input[i] <= 'Z')
		// {
		// 	input[i] += 32;
		// }
		M[i] = input[i] - 'A';
	}
	return 0;
}

int encrypt(char *input, char *output)
{
	// 待加密数据准备,把要加密的字符转换成数字
	int M[100] = {0};
	int C[100] = {0};
	data_pre(input, M);
	// 密文分组N个字符一组，加密
	for (int i = 0; input[i] != '\0'; i += 3)
	{
		C[i] = (A[i % 3][0] * M[i] + A[i % 3][1] * M[i + 1] + A[i % 3][2] * M[i + 2] + B[i % 3]) % NN;
		C[i + 1] = (A[i % 3 + 1][0] * M[i] + A[i % 3 + 1][1] * M[i + 1] + A[i % 3 + 1][2] * M[i + 2] + B[i % 3 + 1]) % NN;
		C[i + 2] = (A[i % 3 + 2][0] * M[i] + A[i % 3 + 2][1] * M[i + 1] + A[i % 3 + 2][2] * M[i + 2] + B[i % 3 + 2]) % NN;
	}
	for (int i = 0; input[i] != '\0'; i++)
	{
		output[i] = C[i] + 'A';
	}

	return 0;
}

int decrypt(char *input, char *output)
{
	// 解密时矩阵位加密矩阵的逆,M=A_(C-B)(modNN)
	int M[100] = {0};
	int C[100] = {0};
	data_pre(input, C);

	for (int i = 0; input[i] != '\0'; i += 3)
	{
		M[i] = (A_[i % 3][0] * (C[i] - B[i % 3]) + A_[i % 3][1] * (C[i + 1] - B[i % 3 + 1]) + A_[i % 3][2] * (C[i + 2] - B[i % 3])) % NN;
		M[i + 1] = (A_[i % 3 + 1][0] * (C[i] - B[i % 3]) + A_[i % 3 + 1][1] * (C[i + 1] - B[i % 3 + 1]) + A_[i % 3 + 1][2] * (C[i + 2] - B[i % 3])) % NN;
		M[i + 2] = (A_[i % 3 + 2][0] * (C[i] - B[i % 3]) + A_[i % 3 + 2][1] * (C[i + 1] - B[i % 3 + 1]) + A_[i % 3 + 2][2] * (C[i + 2] - B[i % 3])) % NN;
	}
	for (int i = 0; input[i] != '\0'; i++)
	{
		output[i] = M[i] + 'A';
	}

	return 0;
}

int main()
{
	char text[1024] = "YOURPINNOISFOURONETWOSIX";
	char cipher_text[1024] = "";
	char plain_text[1024] = "";

	printf("------------多表代替密码-----------\n");
	printf("原文:%s\n", text);
	encrypt(text, cipher_text);
	printf("密文为:%s\n", cipher_text);
	decrypt(cipher_text, plain_text);
	printf("解密后明文为:%s\n", plain_text);
	return 0;
}
