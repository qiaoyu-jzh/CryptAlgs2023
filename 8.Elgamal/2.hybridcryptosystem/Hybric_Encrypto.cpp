// 混合密码体制加解密
// Elgamal+SM4，
// 发送方： M->ZUC_E_key(M),key->Elgamal_E_Puk(key),Elgamal_E_Puk(key)||ZUC_E_key(M)
// 接收方： Elgamal_E_Puk(key)->key,ZUC_E_key(M)->M
//
#include "ZUC.h"
#include "Elgamal.h"
unit32 a = 0, b = 0; // 全局变量

int Hybric_Encrypto(char *input, char *output)
{
	// 输入为明文input，输出为混合加密后的字符串output
	char master_key[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e', 'f'}; // ZUC主密钥
	// 计算输入字节数
	int length = 0;
	for (length = 0; input[length] != '\0'; length++)
		;

	// zuc加密明文
	char c[1024] = {'\0'}; // 密文
	Initialization(master_key);
	KeystreamGenerate(Keystream, length / 4 + 1); // 密钥长度位32位的，计算密钥长度按照32位每个计算，+1避免长度不够
	char key[1024] = {0};
	memcpy(key, Keystream, length); // 只需要length长度的密钥
	for (int i = 0; i < length; i++)
		c[i] = input[i] ^ key[i]; // zuc加密过程就是异或

	// Elgamal对密钥加密，a,b在main里已生成
	unit32 k = rand() % p;
	char encode_key[1024] = {'\0'}; // 加密后的密钥
	Elgamal_Byte_Encrypto(master_key, encode_key, b);

	// 组合起来数据
	memcpy(output, encode_key, 32);
	memcpy(output + 32, &length, 4);
	memcpy(output + 36, c, length);
	return 0;
}

void Hybric_Decrypto(char *input, char *output)
{ // 输入混合加密密文，输出解密后的明文
	// 拆开数据
	int length = 0;
	char en_key[1024] = {0}, C[1024] = {0};
	memcpy(en_key, input, 32); //
	memcpy(&length, input + 32, 4);
	memcpy(C, input + 36, length);

	// Elgamal解密
	char master_key[16] = {0};
	Elgamal_Byte_DEcrypto(en_key, master_key, a);

	// zuc解密,解密和加密过程一样
	Initialization(master_key);
	KeystreamGenerate(Keystream, length / 4 + 1); // 密钥长度位32位的，计算密钥长度按照32位每个计算，+1避免长度不够
	char key[1024] = {0};
	memcpy(key, Keystream, length); // 只需要length长度的密钥
	for (int i = 0; i < length; i++)
		output[i] = C[i] ^ key[i]; // zuc加密过程就是异或
}

int main()
{
	srand(time(NULL));
	a = rand() % p;		 // 私钥0<a<p
	b = Powmod(g, a, p); // 公钥

	char plaintext[1024] = "Elgamal algorithm not only used for data encryption can be used for signatures.";
	char ciphertext[1024] = {0};
	char temp[1024] = {0};
	printf("\n原文为：%s\n", plaintext);
	Hybric_Encrypto(plaintext, ciphertext);
	printf("\n密文为：%s\n", ciphertext);
	Hybric_Decrypto(ciphertext, temp);
	printf("\n解密原文为：%s\n", temp);

	return 0;
}
