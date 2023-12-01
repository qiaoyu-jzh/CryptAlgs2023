//混合密码体制加解密
//Elgamal+SM4， 
//发送方： M->ZUC_E_key(M),key->Elgamal_E_Puk(key),Elgamal_E_Puk(key)||ZUC_E_key(M)
//接收方： Elgamal_E_Puk(key)->key,ZUC_E_key(M)->M
// 
#include "ZUC.h"
#include "Elgamal.h"
unit32 a=0,b=0;//全局变量 

int Hybric_Encrypto(char *input,char *output)
{
	//输入为明文input，输出为混合加密后的字符串output 
	char master_key[16]={'1','2','3','4','5','6','7','8','9','0','a','b' ,'c','d','e','f'};//ZUC主密钥 
	
	return 0; 
}

void Hybric_Decrypto(char *input,char *output)
{//输入混合加密密文，输出解密后的明文 
	
}

int main()
{
	srand(time(NULL));
	a=rand()%p;//私钥0<a<p 
	b=Powmod(g,a,p);//公钥 	
	
	char plaintext[1024]="Elgamal algorithm not only used for data encryption can be used for signatures.";
	char ciphertext[1024]={0};	
	char temp[1024]={0}; 
	printf("\n原文为：%s\n",plaintext);
	Hybric_Encrypto(plaintext,ciphertext);
	printf("\n密文为：%s\n",ciphertext);
	Hybric_Decrypto(ciphertext,temp);
	printf("\n解密原文为：%s\n",temp);
	
	return 0;
}
