#include <stdio.h>
#define P 23
#define Q 17

int Eculid(int f,int d)//求正整数f,d的最大公因子 
{
	int x,y,r;
	x=f;
	y=d;
	
	int flag = 1;
    while(flag)
    {
      if(y==0)
      {
      	flag=0;
      	return x;
	  }
	  else
	  {
		  r=x%y;
		  x=y;
		  y=r; 
	  }
	}
     return 0;
}

//RSA加密运算 
int enc(int m, int e, int n)//公开的n，公钥e，明文m，返回值为密文c 
{
	int c;
	
	return c;
}

//RSA解密运算 
int dec(int c, int d, int n)//密文c，私钥d，返回值明文m 
{
	int m;

	return m;
}

int main()
{
  //初始化（消息m=4,p=23,q=17,e=15） 
  int n,n1,e,m,d,c;
  n=P*Q;
  n1=(P-1)*(Q-1);
  printf("n=pq=%d,n1=(p-1)(q-1)=%d",n,n1);
  printf("\n选取公钥e=");
  scanf("%d",&e);
  if(Eculid(n1,e)>1)
  {
    printf("公钥e与n1不互素，请重新选取公钥e=");
    scanf("%d",&e);
  } 
      
  printf("利用RSA算法加解密，消息m=");
  scanf("%d",&m);
  //生成私钥
  d=key(e,n1);
  printf("\n密钥生成：公钥e=%d, 私钥d=%d\n",e,d);
  //RSA加密运算
  c=enc(m,e,n);
  printf("加密过程：若明文m=%d, 则密文c=%d\n",m,c);
  //RSA解密运算
  m=dec(c,d,n); 
  printf("解密过程：若密文c=%d, 则明文m=%d\n",c,m);
}
