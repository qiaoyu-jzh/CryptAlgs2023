/*维吉尼密码
加密函数：Ci=Pi+K[i%key_length](mod 26)
解密函数：Pi=Ci-K[i%key_length](mod 26)
密钥Key数组是循环使用的，遇到非英文字符时跳过，保持明密文加解密密钥对应关系
样例明文：There are two main methods of classical cipher transformation,substitution and transposition.
*/

#include<stdio.h>
#include<string.h>


//加密函数
void encrypto(char input[],char output[],char key[])
{
	int i;
	int len;
	len=strlen(key);		//计算密钥数组长度；
	for(i=0;input[i]!='\0';i++)
	{		
		if(input[i]>='A' && input[i]<='Z')
			output[i]=(input[i]+32-'a'+key[i%len]-'a')%26+'a';		//+32目的统一将大写字母转换为小写字母，此处key[i%len]是小写字母，需减去'a'，与凯撒密码密钥直接是整数不一样
		else if(input[i]>='a' && input[i]<='z')
			output[i]=(input[i]-'a'+key[i%len]-'a')%26+'a';
		else output[i]=input[i];						//当为标点符号时，保持不变
	}
}

//解密函数
void decrypto(char input[],char output[],char key[])
{
	int i;
	int len;
	len=strlen(key);		//计算密钥数组长度；
	for(i=0;input[i]!='\0';i++)
	{
		if(input[i]>='A' && input[i]<='Z')
			output[i]=(input[i]+32-'a'+26-key[i%len]+'a')%26+'a';		//+32目的统一将大写字母转换为小写字母,此处key[i%len]是字母，同上
		else if(input[i]>='a' && input[i]<='z')
			output[i]=(input[i]-'a'+26-key[i%len]+'a')%26+'a';
		else output[i]=input[i];						//当为标点符号时，保持不变		
	}	
}



int main()
{
	char text[1024]="There are two main methods of classical cipher transformation,substitution and transposition.";			//原文
	char cipher_text[1024]="";		//密文
	char plain_text[1024]="";		//解密后的明文
	char k[1024]="cihper";			//密钥Key数组

	printf("原文:%s\n",text);
	encrypto(text,cipher_text,k);
	printf("密文:%s\n",cipher_text);
	decrypto(cipher_text,plain_text,k);
	printf("解密后的明文:%s\n",plain_text);
	
	return 0;

}