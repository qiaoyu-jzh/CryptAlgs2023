/*置换密码，按照给定数组顺序进行变化操作
要求：
1.可对任意字符进行加解密（字母、数字、符号）
2.密文和解密后的明文中所有英文字母全部为小写，其他字符保持不变
样例明文：There are two main methods of classical cipher transformation,substitution and transposition.
*/

#include<stdio.h>
#include<string.h>

//加密函数
void encrypto(char input[],char output[],int T1[])
{
	int i;
	for(i=0;input[i]!='\0';i++)
	{

		if(input[i]>='A' && input[i]<='Z')		//先将字符串中的大写字母转换为小写字母
			input[i]=input[i]+32;
		output[i]=input[T1[i%10]+(i/10)*10];
	}
	//输入：明文和加密变换表T  输出：置换变换后的密文

}

//解密函数
void decrypto(char input[],char output[],int T2[])
{
	int i;
	for(i=0;input[i]!='\0';i++)
	{
		if(input[i]>='A' && input[i]<='Z')		//先将字符串中的大写字母转换为小写字母
			input[i]=input[i]+32;
		output[i]=input[T2[i%10]+(i/10)*10];
	}
}

//补齐空格,使得文本长度为密钥长度(10)的整数倍
void fk(char input[],char output[])
{
	int i,j;
	int len=0;
	for(i=0;input[i]!='\0';i++)
	{
		len++;
	}
	if(len%10!=0)
	{
		for(j=0;j<(len/10+1)*10;j++)
		{
			if(j<len) output[j]=input[j];
			else output[j]=' ';
		}		
	}
	else
		for(j=0;j<len;j++)
		{
			output[j]=input[j];	
		}							
}

int main()
{
	char text[1024]="There are two main methods of classical cipher transformation,substitution and transposition.";
	char text_space[1024]="";		//用于存储补齐空格后的原文
//	char text[1024]="Therearetwk"; 
	char cipher_text[1024]="";
	char plain_text[1024]="";
	int T1[10]={3,4,7,6,2,9,1,8,0,5};
	int T2[10]={8,6,4,0,1,9,3,2,7,5};

	fk(text,text_space);

	printf("--------------置换密码------------------\n");
	printf("原文补齐空格后:%s\n",text_space);	
	printf("换位操作顺序：%");
	for(int i=0;i<10;i++)
	{
		printf("%d  ",T1[i]);
	}
	printf("\n\n");
	encrypto(text_space,cipher_text,T1);
	printf("密文:%s\n",cipher_text);
	printf("逆换位操作顺序：%");
	for(int i=0;i<10;i++)
	{
		printf("%d  ",T2[i]);
	}
	printf("\n\n");
	decrypto(cipher_text,plain_text,T2);
	printf("解密后的明文:%s\n",plain_text);
	
	return 0;

}