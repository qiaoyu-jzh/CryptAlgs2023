#include <stdio.h>
#define P 23
#define Q 17
typedef unsigned int unit32;

int Eculid(int f, int d) // 求正整数f,d的最大公因子
{
  int x, y, r;
  x = f;
  y = d;

  int flag = 1;
  while (flag)
  {
    if (y == 0)
    {
      flag = 0;
      return x;
    }
    else
    {
      r = x % y;
      x = y;
      y = r;
    }
  }
  return 0;
}

unit32 Ex_Euclid(unit32 f, unit32 d) // 求d模f的逆元
{
  int x1, x2, x3;
  int y1, y2, y3;
  int t1, t2, t3;
  int q;

  x1 = 1;
  x2 = 0;
  x3 = f;

  y1 = 0;
  y2 = 1;
  y3 = d;

  int flag = 1;
  while (flag)
  {
    if (y3 == 1)
    {
      flag = 0;
      if (y2 < 0)
      {
        y2 = y2 + f; // 若为负数，则加f变为整数即可
      }
    }
    if (y3 > 1)
    {
      q = x3 / y3;

      t1 = x1 - q * y1;
      t2 = x2 - q * y2;
      t3 = x3 - q * y3;

      x1 = y1;
      x2 = y2;
      x3 = y3;

      y1 = t1;
      y2 = t2;
      y3 = t3;
    }
  }
  return y2;
}

int FE(int a, int t, int n) // 返回值为a的t次方模n的值
{
  int x = 1;
  while (t)
  {
    if (t & 1 == 1)
    {
      x = x % n * a % n;
    }
    a = a % n * a % n;
    t >>= 1;
  }
  return x;
}

// RSA加密运算
int enc(int m, int e, int n) // 公开的n，公钥e，明文m，返回值为密文c
{
  int c;
  c = FE(m, e, n);
  return c;
}

// RSA解密运算
int dec(int c, int d, int n) // 密文c，私钥d，返回值明文m
{
  int m;
  m = FE(c, d, n);
  return m;
}

int key(int e, int n)
{
  return Ex_Euclid(n, e);
}

int main()
{
  // 初始化（消息m=4,p=23,q=17,e=15）
  int n, n1, e, m, d, c;
  n = P * Q;
  n1 = (P - 1) * (Q - 1);
  printf("n=pq=%d,n1=(p-1)(q-1)=%d", n, n1);
  printf("\n选取公钥e=");
  scanf("%d", &e);
  if (Eculid(n1, e) > 1)
  {
    printf("公钥e与n1不互素，请重新选取公钥e=");
    scanf("%d", &e);
  }

  printf("利用RSA算法加解密，消息m=");
  scanf("%d", &m);
  // 生成私钥
  d = key(e, n1);
  printf("\n密钥生成：公钥e=%d, 私钥d=%d\n", e, d);
  // RSA加密运算
  c = enc(m, e, n);
  printf("加密过程：若明文m=%d, 则密文c=%d\n", m, c);
  // RSA解密运算
  m = dec(c, d, n);
  printf("解密过程：若密文c=%d, 则明文m=%d\n", c, m);
}
