/*	��Բ���߻�������ʵ��
	����������������Բ�����ϵĵ�
	���̣�y2=x3+ax+b mod p������a,b,p
*/
#include<stdio.h>
#include<math.h>
#include<string.h>

typedef unsigned char unit8;
typedef unsigned int  unit32;

unit32 Point[100][2]={0};//ȫ�ֱ��������������ĵ�ļ���
int a=1,b=4,P=251;//���ó�ʼ����
int G[2]={7,75};//����ԪG
int n=271;//��G�Ľ�

int Ex_Euclid(int f,int d)//��dģf����Ԫ
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
      	   y2=y2+f;//��Ϊ���������f��Ϊ��������
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

int Get_ECC_Point(int a,int b,int p,unit32 (*point)[2])//����������������е㣬����Point�У������ص����Ŀ
{
	if ((4*a*a*a+27*b*b)%p==0)
	{
		printf("��������4*a^3+27*b^2 modp=0!\n");
		return 0;
	}
	int n=0;//�����Ŀ,��������ֵ
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
	if(p[0]==q[0])//��p=q
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



