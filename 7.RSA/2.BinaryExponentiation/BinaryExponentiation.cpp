// ����ָ��
#include <stdio.h>
int FE(int a, int t, int n) // ����ֵΪa��t�η�ģn��ֵ
{
	int x = 1;
	while (t)
	{
		if (t & 1)
		{
			int x = x * a % n;
		}
		x = x * x % n;
		t >>= 1;
	}
	return x;
}
int main()
{
	int a, t, n;

	printf("���a��t�η�ģn��ֵ��");
	scanf("%d,%d,%d", &a, &t, &n);
	printf("\n\n");

	int ans = 0;
	ans = FE(a, t, n);
	printf("%d��%d�η�ģ%d��ֵΪ��%d", a, t, n, ans); // 66��77��119�����Ϊ19
	return 0;
}
