/*Elgamal算法实现*/
#include<stdio.h>

//typedef unsigned int unit32;//
int Powmod(int a,int t,int n)//快速指数算法计算以a为底的t次幂模n的值 
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
	  
	//取k值（二进制数组中最大非零的下标） 
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
	
	//快速指数算法
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

//int Primitive_root(int p)//求数p的本原根 
//{
//	int root=0;
//	for(int i=2;i<p;i++)
//	{
//		int ordi=0;
//		for(int j=1;j<p;j++)//计算i的阶 
//		{
//			if (Powmod(i,j,p)==1) 
//			{
//				ordi=j;
//				break;
//			}
//		}
//		if(ordi==p-1)//如果i的阶等于p-1即为本源根 
//		{
//			root=i;
//			break;
//		}
//	}
//	return root;
//}
//
//**********************************Elgamal算法参数和加解密****************************// 
//int p=127;//素数
//int g=Primitive_root(p);//本原根
//unit32 r=0,s=0;//密文全局变量 

//void Elgamal_Encrypto(unit32 m,unit32 puk,unit32 &r,unit32 &s)
//{
//	//E(m,k)=(r,s)
//	//r=gk mod p
//	//s=mβk mod p
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
//	unit32 a,b;//私钥a，加密随机数b 
//	unit32 m;//明文 
//	unit32 temp=0;
//
//	srand(time(NULL));
//	a=rand()%p;//私钥0<a<p 
//	b=Powmod(g,a,p);//公钥 
//
//	printf("/*******************Elgamal算法加解密****************/\n");
//	printf("素数p:%d，生成元g：%d，私钥α:%d，公钥β：%d\n",p,g,a,b);
//	
//	char text[100]="Elgamal algorithm not only used for data encryption can be used for signatures.";
//	char cipher[200]={0};
//	printf("\n原文为：%s\n",text);
//	Elgamal_Byte_Encrypto(text,cipher,b);
//	printf("\n密文为：%s\n",cipher);
//	Elgamal_Byte_DEcrypto(cipher,text,a);
//	printf("\n解密原文为：%s\n",text);
//	
//	return 0;
//}

