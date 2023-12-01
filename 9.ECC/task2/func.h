//常用功能函数 
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
typedef unsigned int unit32;//
unit32 Powmod(unit32 a,unit32 t,unit32 n)//快速指数算法计算以a为底的t次幂模n的值 
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

unit32 Ex_Euclid(unit32 f,unit32 d)//扩展欧几里得算法，求d模f的逆元 
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

unit32 Eculid(unit32 f,unit32 d)//欧几里得判断，f和d是否互素，f>d 
{
	unit32 x,y,r;
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

bool witness(int a,int n)//对n是否为素数进行判断，随机数为a 
{
	 //将10进制的数转化为2进制 
	 int temp,i;
	 int b[64]={0};
	 temp=n-1;
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
	
	//Miller-Rabin素数测试法 
	int x=0;
	int y=1;
	//bool flag=true;

	for(i=k;i>=0;i--)
	{
      x=y;
      y=(y*y)%n;
      if((y==1)&&(x!=1)&&(x!=n-1))
      {
      	return false;
	  }
      if(b[i]==1)
      {
        y=(y*a)%n;	
	  }
	}
	if(y!=1)
	{
		return false;
	}
	return true;
}
