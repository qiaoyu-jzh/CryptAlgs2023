#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

char ipad = 0x36;
char opad = 0x5c;

void hmac_md5(unsigned char *data, int dlen, unsigned char *key, int klen, unsigned char *out)
{
	/**输入：data 需要加密的数据，dlen 加密数据的长度，key 密钥，klen 密钥长度
	 *  输出：out  消息认证码
	 */

	int i = 0;
	unsigned char hash_temp[16]; // 存储hash结果
	//----------K+准备--------------------------
	unsigned char Kplus[64];

	for (i = 0; i < 64; i++) // 预制全为0的K+，准备填充（左边填充0）
	{
		Kplus[i] = 0;
	}
	if (klen > 64) // key长度大于512bit时，先进行hash，再左边填充
	{
		MD5(key, klen, hash_temp);
		for (i = 0; i < 16; i++)
		{
			Kplus[i] = hash_temp[i];
		}
	}
	else // 长度不足512bit时，直接填充
	{
		for (i = 0; i < klen; i++)
			Kplus[i] = key[i];
	}

	//-----------SI 和 SO--------------------
	unsigned char SI[64];
	unsigned char SO[64];
	for (i = 0; i < 64; i++)
	{
		SI[i] = Kplus[i] ^ ipad;
		SO[i] = Kplus[i] ^ opad;
	}

	//-----data  M拼接到SI后面-------------------------
	unsigned char hash_input[64 + dlen];
	for (i = 0; i < 64; i++)
	{
		hash_input[i] = SI[i];
	}
	for (; i < dlen + 64; i++)
	{
		hash_input[i] = data[i - 64];
	}

	//-------计算第一个hash值--------------------
	unsigned hash_out[16];
	MD5(hash_input, dlen + 64, hash_temp);

	//-----hash结果填充到512位后和SO拼接---------------------
	unsigned char hash_input2[64+16]={0};
	// for (i = 0; i < 64; i++) //
	// {
	// 	hash_input2[i] = 0;
	// }
	for (i = 0; i < 64; i++)
	{
		hash_input2[i] = SO[i];
	}
	for (; i < 64+16; i++)
	{
		hash_input2[i] = hash_temp[i - 64];
	}
	//---------计算最后的hash----------------
	MD5(hash_input2, 64+16, out);
}

int main()
{
	char *data = "Hash-based Message Authentication Code";
	char *key = "20221118";
	printf("数据：%s\n", data);
	printf("密钥：%s\n", key);

	unsigned char out[16] = {0};
	hmac_md5((unsigned char *)data, strlen(data), (unsigned char *)key, strlen(key), out);
	printf("HMAC值：");
	for (int i = 0; i < 16; i++)
	{
		printf("%02x ", out[i]);
	}
	return 0;
}
