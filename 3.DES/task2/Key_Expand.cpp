/*DES算法实现
DES是对称结构加解密算法是一样的，整个算法大的方面只包含密钥生成和加解密两部分
密钥生成：PC1+LS+PC2
加解密：IP+轮函数(F函数+异或)+ IP-1
F函数：E+异或+S-Boxes+P
*/
#include<stdio.h>
#include<memory.h>//memcpy所需头文件

//****************************固定参数********************//
//置换选择PC1,64-56
int PC1_Table[56] = {56,48,40,32,24,16,8,
                0,57,49,41,33,25,17,
                9,1,58,50,42,34,26,
                18,10,2,59,51,43,35,
                62,54,46,38,30,22,14,
                6,61,53,45,37,29,21,
                13,5,60,52,44,36,28,
                20,12,4,27,19,11,3
               };

//置换选择PC2,56-48
int PC2_Table[48] = {13,16,10,23,0,4,2,27,
                14,5,20,9,22,18,11,3,
                25,7,15,6,26,19,12,1,
                40,51,30,36,46,54,29,39,
                50,44,32,46,43,48,38,55,
                33,52,45,41,49,35,28,31
               };

int MOVE_TIMES[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};//左移位数

//****************************秘钥扩展****************************//
int PC_1(char in[64],char out[56])
{//PC1置换
	for (int i = 0; i < 56;i++)
	{
		out[i] = in[PC1_Table[i]];
	}
	return 0;
}

int PC_2(char in[56],char out[48])
{//PC2置换
for (int i = 0; i < 48;i++)
	{
		out[i] = in[PC2_Table[i]];
	}
	return 0;
}

int ROL(char in[56],int time)
{//循环左移，56bit分左右两部分循环移位time次
	for (int i = 0; i <= 28 - time;i++)
	{
		in[i] = in[(i + time) % 28];
	}
	for (int i = 0; i <= 56 - time;i++)
	{
		in[i] = in[(i + time) % 56];
	}
	return 0;
}

int Key_Expand(char in[64],char subkey[16][48])
{//秘钥扩展，64bit主秘钥，生成16轮48bit子秘钥
	char out[56];
	PC_1(in, out);
	for (int i = 0; i < 16;i++)
	{
		ROL(out, MOVE_TIMES[i]);
		PC_2(out, subkey[i]);
	}
	return 0;
}

int main()
{
	char key[64]={0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x1,//主秘钥
				  0x0,0x1,0x1,0x0,0x0,0x1,0x0,0x0,
				  0x1,0x0,0x0,0x1,0x0,0x1,0x1,0x1,
				  0x0,0x1,0x0,0x0,0x1,0x1,0x1,0x1,
				  0x0,0x0,0x0,0x1,0x1,0x1,0x0,0x0,
				  0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,
				  0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
				  0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0
				  };
	char subkeys[16][48]={0};
	printf("主秘钥：\n");
	for(int i=0;i<64;i++)
	{
		printf("  %x",key[i]);
		if ((i+1)%16==0) printf("\n");
	}
	Key_Expand(key,subkeys);
	for(int i=0;i<16;i++)
	{
		printf("第%d轮子秘钥：\n",i+1);
		for(int j=0;j<48;j++)
		{
			printf("  %x",subkeys[i][j]);
			if ((j+1)%16==0) printf("\n");
		}
	}
	return 0;
}


