/*	椭圆曲线基本运算实现
	计算满足条件的椭圆曲线上的点
	方程：y2=x3+ax+b mod p，参数a,b,p
*/
#include<stdio.h>
#include<math.h>
#include<string.h>

typedef unsigned char unit8;
typedef unsigned int  unit32;

unit32 Point[100][2]={0};//全局变量，满足条件的点的集合
int a=1,b=4,P=251;//设置初始参数
int G[2]={7,75};//生成元G
int n=271;//点G的阶

int Ex_Euclid(int f,int d)//求d模f的逆元
{
   	int x1,x2,x3;
	int y1,y2,y3;
	int t1,t2,t3;
	int q;

	x1=1;
	x2=0;
	x3=f;

	y1=0;
	y2=1;
	y3=d;

	int flag = 1;
    while(flag)
    {
	  if(y3==1)
      {
      	flag=0;
      	if(y2<0)
      	{
      	   y2=y2+f;//若为负数，则加f变为整数即可
		}
	  }
	  if(y3>1)
	  {
	  	q=x3/y3;

	  	t1=x1-q*y1;
	  	t2=x2-q*y2;
	  	t3=x3-q*y3;

	  	x1=y1;
	  	x2=y2;
	  	x3=y3;

	  	y1=t1;
	  	y2=t2;
	  	y3=t3;
	  }
	}
	return y2;
}

int Get_ECC_Point(int a,int b,int p,unit32 (*point)[2])//计算符合条件的所有点，存入Point中，并返回点的数目
{
	if ((4*a*a*a+27*b*b)%p==0)
	{
		printf("参数错误，4*a^3+27*b^2 modp=0!\n");
		return 0;
	}
	int n=0;//点的数目,用作返回值
	for (int i=0;i<p;i++)
	{
		for(int j=0;j<p;j++)
		{
			if((j*j)%p==(i*i*i+a*i+b)%p)
			{
				point[n][0]=i;//x
				point[n][1]=j;//y
				n++;
			}
		}
	}
	return n;
}

void ECC_Add(int p[2],int q[2],int r[2])
{
	if(p[0]==0&&p[1]==0) {
		r[0]=q[0];r[1]=q[1];
		return;
	}
	if(q[0]==0&&q[1]==0) {
		r[0]=p[0];r[1]=p[1];
		return;
	}
	if((p[0]==q[0])&&((p[1]+q[1])%P==0)) {
		r[0]=0;r[1]=0;
		return;
	}
	int lamda=0;
	if(p[0]==q[0])//点p=q
	{
		lamda=((3*p[0]*p[0]+a)%P*Ex_Euclid(P,2*p[1]))%P;//(3x1^2+a)/2y1
	}
	else lamda=((q[1]-p[1]+P)%P*Ex_Euclid(P,(q[0]-p[0]+P)%P))%P;//(y2-y1)/(x2-x1)
	//printf("\nlamda=%d\n",lamda);
	r[0]=((lamda*lamda-p[0]-q[0])%P+P)%P;
	r[1]=((lamda*(p[0]-r[0])-p[1])%P+P)%P;
}

void ECC_Multi(int p[2],int n,int q[2])
{
	int temp[2]={0};
	q[0]=p[0];q[1]=p[1];
	for(int i=1;i<n;i++)
	{
		ECC_Add(q,p,temp);
		q[0]=temp[0];q[1]=temp[1];
	}
}



