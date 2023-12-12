/*Elgamal�㷨ʵ��*/
#include<stdio.h>

//typedef unsigned int unit32;//
int Powmod(int a,int t,int n)//����ָ���㷨������aΪ�׵�t����ģn��ֵ 
{
 	int temp,i;
	int b[64]={0};
	temp=t;
	i=0;
	while(temp>0)
	{
		b[i] = temp%2;
	 	temp = temp/2;
	 	i++;
	}
	  
	//ȡkֵ����������������������±꣩ 
    int k=0;
	for(i=63;i>=0;i--)
	{
		if(b[i]==1)
		{
		 k=i;
		 break;	
		}
	}
	//printf("k=%d\n\n",k);
	
	//����ָ���㷨
	int j=0;
	int x=1;
	for(i=k;i>=0;i--)
	{
		j=2*j;
		x=(x*x)%n;
		if(b[i]==1)
		{
			j++;
			x=(x*a)%n;
		}
	}
    return x;
}

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

//int Primitive_root(int p)//����p�ı�ԭ�� 
//{
//	int root=0;
//	for(int i=2;i<p;i++)
//	{
//		int ordi=0;
//		for(int j=1;j<p;j++)//����i�Ľ� 
//		{
//			if (Powmod(i,j,p)==1) 
//			{
//				ordi=j;
//				break;
//			}
//		}
//		if(ordi==p-1)//���i�Ľ׵���p-1��Ϊ��Դ�� 
//		{
//			root=i;
//			break;
//		}
//	}
//	return root;
//}
//
//**********************************Elgamal�㷨�����ͼӽ���****************************// 
//int p=127;//����
//int g=Primitive_root(p);//��ԭ��
//unit32 r=0,s=0;//����ȫ�ֱ��� 

//void Elgamal_Encrypto(unit32 m,unit32 puk,unit32 &r,unit32 &s)
//{
//	//E(m,k)=(r,s)
//	//r=gk mod p
//	//s=m��k mod p
//	unit32 k=rand()%p;
//	unit32 temp=0;
//	r=Powmod(g,k,p);
//	temp=Powmod(puk,k,p);
//	s=(m*temp)%p;
//} 
//
//unit32 Elgamal_Decrypto(unit32 r,unit32 s,unit32 prk)
//{
//	//D(r,s)=s(ra)-1 mod p
//	unit32 temp=0;
//	temp=Powmod(r,prk,p);
//	temp=Ex_Euclid(p,temp);
//	temp=(s*temp)%p;
//	return temp;
//}
//
//void Elgamal_Byte_Encrypto(char *input,char *output,unit32 puk)
//{
//	//unit32 tempin=0,tempout=0;
//	unit32 r=0,s=0;
//	for(int i=0;input[i]!='\0';i++)
//	{
//		Elgamal_Encrypto(input[i],puk,r,s);
//		output[2*i]=r;
//		output[2*i+1]=s;
//	}
//}
//
//void Elgamal_Byte_DEcrypto(char *input,char *output,unit32 prk)
//{
//	//unit32 tempin=0,tempout=0;
//	unit32 r=0,s=0,temp=0;
//	for(int i=0;input[2*i]!='\0';i++)
//	{
//		r=input[2*i];
//		s=input[2*i+1];
//		temp=Elgamal_Decrypto(r,s,prk);
//		output[i]=temp&0xff;
//	}
//}
//
//int main()
//{
//	unit32 a,b;//˽Կa�����������b 
//	unit32 m;//���� 
//	unit32 temp=0;
//
//	srand(time(NULL));
//	a=rand()%p;//˽Կ0<a<p 
//	b=Powmod(g,a,p);//��Կ 
//
//	printf("/*******************Elgamal�㷨�ӽ���****************/\n");
//	printf("����p:%d������Ԫg��%d��˽Կ��:%d����Կ�£�%d\n",p,g,a,b);
//	
//	char text[100]="Elgamal algorithm not only used for data encryption can be used for signatures.";
//	char cipher[200]={0};
//	printf("\nԭ��Ϊ��%s\n",text);
//	Elgamal_Byte_Encrypto(text,cipher,b);
//	printf("\n����Ϊ��%s\n",cipher);
//	Elgamal_Byte_DEcrypto(cipher,text,a);
//	printf("\n����ԭ��Ϊ��%s\n",text);
//	
//	return 0;
//}

