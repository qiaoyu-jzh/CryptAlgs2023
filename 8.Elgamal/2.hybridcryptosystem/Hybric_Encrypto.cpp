//����������Ƽӽ���
//Elgamal+SM4�� 
//���ͷ��� M->ZUC_E_key(M),key->Elgamal_E_Puk(key),Elgamal_E_Puk(key)||ZUC_E_key(M)
//���շ��� Elgamal_E_Puk(key)->key,ZUC_E_key(M)->M
// 
#include "ZUC.h"
#include "Elgamal.h"
unit32 a=0,b=0;//ȫ�ֱ��� 

int Hybric_Encrypto(char *input,char *output)
{
	//����Ϊ����input�����Ϊ��ϼ��ܺ���ַ���output 
	char master_key[16]={'1','2','3','4','5','6','7','8','9','0','a','b' ,'c','d','e','f'};//ZUC����Կ 
	
	return 0; 
}

void Hybric_Decrypto(char *input,char *output)
{//�����ϼ������ģ�������ܺ������ 
	
}

int main()
{
	srand(time(NULL));
	a=rand()%p;//˽Կ0<a<p 
	b=Powmod(g,a,p);//��Կ 	
	
	char plaintext[1024]="Elgamal algorithm not only used for data encryption can be used for signatures.";
	char ciphertext[1024]={0};	
	char temp[1024]={0}; 
	printf("\nԭ��Ϊ��%s\n",plaintext);
	Hybric_Encrypto(plaintext,ciphertext);
	printf("\n����Ϊ��%s\n",ciphertext);
	Hybric_Decrypto(ciphertext,temp);
	printf("\n����ԭ��Ϊ��%s\n",temp);
	
	return 0;
}
