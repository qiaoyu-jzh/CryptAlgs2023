#include <stdio.h>
// 欧几里得算法
int Eculid(int f, int d) // 求正整数f,d的最大公因子
{
    // while (d != 0)
    // {
    //     int R = f % d;
    //     f = d;
    //     d = R;
    // }
    // return f;
    //递归
    return d == 0 ? f : Eculid(d, f % d);
}
int main()
{
    // 求f,d的最大公因子
    int f = 12075;
    int d = 4655;
    int t = 0;
    //	printf("输入两个正整数：");
    //	scanf("%d,%d",&f,&d);

    t = Eculid(f, d);
    printf("%d和%d的最大公因子为：%d", f, d, t);
    return 0;
}
