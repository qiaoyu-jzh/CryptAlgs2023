#include <stdio.h>
#define P 23
#define Q 17

int Eculid(int f,int d)//��������f,d��������� 
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

//RSA�������� 
int enc(int m, int e, int n)//������n����Կe������m������ֵΪ����c 
{
	int c;
	
	return c;
}

//RSA�������� 
int dec(int c, int d, int n)//����c��˽Կd������ֵ����m 
{
	int m;

	return m;
}

int main()
{
  //��ʼ������Ϣm=4,p=23,q=17,e=15�� 
  int n,n1,e,m,d,c;
  n=P*Q;
  n1=(P-1)*(Q-1);
  printf("n=pq=%d,n1=(p-1)(q-1)=%d",n,n1);
  printf("\nѡȡ��Կe=");
  scanf("%d",&e);
  if(Eculid(n1,e)>1)
  {
    printf("��Կe��n1�����أ�������ѡȡ��Կe=");
    scanf("%d",&e);
  } 
      
  printf("����RSA�㷨�ӽ��ܣ���Ϣm=");
  scanf("%d",&m);
  //����˽Կ
  d=key(e,n1);
  printf("\n��Կ���ɣ���Կe=%d, ˽Կd=%d\n",e,d);
  //RSA��������
  c=enc(m,e,n);
  printf("���ܹ��̣�������m=%d, ������c=%d\n",m,c);
  //RSA��������
  m=dec(c,d,n); 
  printf("���ܹ��̣�������c=%d, ������m=%d\n",c,m);
}
