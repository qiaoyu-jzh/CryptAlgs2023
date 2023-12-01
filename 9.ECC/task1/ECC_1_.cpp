/*	��Բ���߻�������ʵ��
	����������������Բ�����ϵĵ�
	���̣�y2=x3+ax+b mod p������a,b,p
*/
#include <stdio.h>
#include <math.h>

int Point[100][2] = {0}; // ȫ�ֱ��������������ĵ�ļ���

int Get_ECC_Point(int a, int b, int p, int (*point)[2]) // ����������������е㣬����Point�У������ص����Ŀ
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
	// ���ó�ʼ����
	int a = 1, b = 1, p = 23;
	int n = 0;
	printf("\n��Բ���� y^2=x^3+%dx+%d mod %d�ϵĵ㣺\n", a, b, p);
	n = Get_ECC_Point(a, b, p, Point);
	for (int i = 0; i < n; i++)
	{
		printf("\t(%d,%d)", Point[i][0], Point[i][1]);
		if ((i + 1) % 6 == 0)
			printf("\n");
	}
	return 0;
}
