#include <stdio.h>
#include <stdlib.h>
#define time 10
bool witness(int a, int n) // ��n�Ƿ�Ϊ���������жϣ������Ϊa
{
	int y = 1, b = n - 1;
	while (b)
	{
		if (b & 1)
		{
			y = y * a % n;
		}
		int x = a;
		a = a * a % n; // �ж� x^2 mod n =1��
		if (a == 1 && x != 1 && x != n - 1)
		{
			return false;
		}
		b >>= 1;
	}
	if (y != 1)
	{
		return false;
	}
	return true;
}
int main()
{
	int n;
	printf("��Ҫ���ԵĴ�����Ϊ��"); // �ж�25409�Ƿ�Ϊ����
	scanf("%d", &n);
	printf("\n\n");

	// �����aȡֵ
	int a = 0;
	int i;

	for (i = 0; i < time; i++)
	{
		a = rand() % n;
		printf("�����a=%d,", a);
		if (witness(a, n) == false)
			printf("%d�Ǻ���\n", n);
		else
			printf("%d������\n", n);
	}
	return 0;
}
