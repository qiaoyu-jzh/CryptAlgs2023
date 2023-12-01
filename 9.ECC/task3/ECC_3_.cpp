/*	��Բ���߻�������ʵ��
	����������������Բ�����ϵĵ�
	���̣�y2=x3+ax+b mod p������a,b,p
*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "func.h"

int Point[100][2] = {0};  // ȫ�ֱ��������������ĵ�ļ���
int a = 1, b = 1, P = 23; // ���ó�ʼ����
int G[2] = {3, 10};		  // ����ԪG

void ECC_Add(int p[2], int q[2], int r[2])
{
	int lamda = 0;
	if (p[0] != q[0])
	{
		lamda = (q[1] - p[1]+P) % P * Ex_Euclid(P, (q[0] - p[0]+P));
	}
	else
	{
		lamda = (3 * p[0] * p[0] + a) % P * Ex_Euclid(P, 2 * p[1]);
	}
	// �������r��xֵ��yֵ
	r[0] = (lamda * lamda - p[0] - q[0]) % P;  // x
	r[1] = (lamda * (p[0] - r[0]) - p[1]) % P; // y
	if (r[1] < 0)
	{
		r[1] += P;
	}
}

void ECC_Multi(int p[2], int n, int q[2])
{
	for (int i = 0; i < n - 1; i++)
	{
		ECC_Add(p, p, q);
	}
}

void ECC_Encrypto(int m[2], int puk[2], int c[2][2]) // ���ġ���Կ������
{
	int k = 6; // ���������
	// �������ģ�c=(c1,c2)��c1=k*G,c2=M+k*puk����Բ�����ϵļӷ��ͳ˷�
	ECC_Multi(G, k, c[0]);//c1
	ECC_Multi(puk, k, c[1]);//k*puk
	ECC_Add(m, c[1], c[1]);//c2
}

void ECC_Decrypto(int c[2][2], int m[2], int d) // ���ġ����ġ�˽Կ
{
	// ���ܣ�c=(c1,c2)��m=c2-d*c1����Բ����p(x,y)�ļӷ���Ԫ-p=(x,-y)
	int temp[2] = {0};
	ECC_Multi(c[0], d, temp);//d*c1
	temp[1] = (P - temp[1]) % P;
	ECC_Add(c[1], temp, m);
}

int main()
{
	int d = 7; // ˽Կ
	int PA[2] = {0};
	ECC_Multi(G, d, PA); // ��ԿPA=d*G
	printf("\n��Բ���� y^2=x^3+%dx+%d mod %d\n", a, b, P);
	printf("����ԪG=(%d,%d),˽Կd=%d,��ԿPA=(%d,%d)\n", G[0], G[1], d, PA[0], PA[1]);
	int M[2] = {1, 7};
	int C[2][2] = {0};
	printf("����M=(%d,%d)\n", M[0], M[1]);
	ECC_Encrypto(M, PA, C);
	printf("����C=((%d,%d),(%d,%d))\n", C[0][0], C[0][1], C[1][0], C[1][1]);
	int temp_out[2] = {0};
	ECC_Decrypto(C, temp_out, d);
	printf("����ԭ��M=(%d,%d)\n", temp_out[0], temp_out[1]);

	return 0;
}
