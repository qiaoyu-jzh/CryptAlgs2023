#include <stdio.h>
// ŷ������㷨
int Eculid(int f, int d) // ��������f,d���������
{
    // while (d != 0)
    // {
    //     int R = f % d;
    //     f = d;
    //     d = R;
    // }
    // return f;
    //�ݹ�
    return d == 0 ? f : Eculid(d, f % d);
}
int main()
{
    // ��f,d���������
    int f = 12075;
    int d = 4655;
    int t = 0;
    //	printf("����������������");
    //	scanf("%d,%d",&f,&d);

    t = Eculid(f, d);
    printf("%d��%d���������Ϊ��%d", f, d, t);
    return 0;
}
