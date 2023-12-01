/*维吉尼亚密码
加密函数：Ci=Pi+K[i%key_length](mod 26)
解密函数：Pi=Ci-K[i%key_length](mod 26)
*/

#include<stdio.h>
#include<string.h>

//加密
int encrypt(char *input,char *output,char *k)
{
    int len = strlen(k);
    for (int i = 0; input[i] != '\0';i++)
    {
        if(input[i]>='A'&& input[i]<='Z')
            input[i] += 32;
        if(input[i]>='a'&& input[i]<='z')
        {
            output[i] = (input[i] - 'a' + k[i % len] - 'a') % 26 + 'a';
        }
        else
        {
            output[i] = input[i];
        }
    }
	return 0;
}

//解密
int decrypt(char *input,char *output,char *k)
{
	int len = strlen(k);
    for (int i = 0; input[i] != '\0';i++)
    {
        if(input[i]>='A'&& input[i]<='Z')
            input[i] += 32;
        if(input[i]>='a'&& input[i]<='z')
        {
            output[i] = (input[i] - 'a' + 26 - k[i % len] + 'a') % 26 + 'a';
        }
        else
        {
            output[i] = input[i];
        }
    }
	return 0;
}

int main()
{
	char text[1024]="There are two main methods of classical cipher transformation, substitution and transposition.";
    char cipher_text[1024]="";
    char plain_text[1024]="";
	char k[1024]="cipher";

    printf("------------Vigenere密码-----------\n");
    printf("原文:%s\n",text);
    encrypt(text,cipher_text,k);
    printf("密文为:%s\n",cipher_text);
    decrypt(cipher_text,plain_text,k);
    printf("解密后明文为:%s\n",plain_text);
    return 0;
}
