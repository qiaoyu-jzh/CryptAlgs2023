/*SM4算法实现
SM4是对称结构加解密算法一致，秘钥倒序
加解密：128bit->4*32bit，共32轮	F（Xi，Xi+1,Xi+2,Xi+3,rki） ，最后一轮交换位置
	F函数：异或-S盒-循环移位-异或
秘钥扩展：128bit->4*32bit，初始化-轮变换
	初始化：与FK异或
	轮变换：异或-S盒-循环移位-异或
*/

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

typedef unsigned char unit8;
typedef unsigned int unit32;

const unit8 S[256] = {
	0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
	0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
	0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
	0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
	0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
	0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
	0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
	0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
	0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
	0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
	0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
	0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
	0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
	0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
	0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
	0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48};

const unit32 ck[32] =
	{
		0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
		0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
		0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
		0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
		0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
		0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
		0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
		0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279};

int Swap(unit32 data[4]) // 4个32位分组位置反序
{
	unit32 temp;
	temp = data[0];
	data[0] = data[3];
	data[3] = temp;
	temp = data[2];
	data[2] = data[1];
	data[1] = temp;
	return 0;
}

int SBox(unit32 &data)
{
	data = S[data >> 24 & 0xff] << 24 | S[data >> 16 & 0xff] << 16 | S[data >> 8 & 0xff] << 8 | S[data & 0xff];
	return 0;
}

int RotL(unit32 data, int n) // data循环左移n位
// 使用引用地址参数后结果不一样
{
	data = (data << n) | (data >> (32 - n));
	return data;
}

int LS_1(unit32 &data) // data与自身循环左移2,10,18,24进行异或
{
	data = data ^ RotL(data, 2) ^ RotL(data, 10) ^ RotL(data, 18) ^ RotL(data, 24);
	return 0;
}

int LS_2(unit32 &data) // data与自身循环左移13,23进行异或
{
	data = data ^ RotL(data, 13) ^ RotL(data, 23);
	return 0;
}

int datato32(unit8 input[16], unit32 *outputdata)
{
	for (int i = 0; i < 4; i++)
	{
		outputdata[i] = input[i * 4] << 24 | input[i * 4 + 1] << 16 | input[i * 4 + 2] << 8 | input[i * 4 + 3];
		// printf("outputdata[%d]:%x",i,outputdata[i]);
	}
	return 0;
}

int datato8(unit32 input[4], unit8 *outputdata)
{
	for (int i = 0; i < 4; i++)
	{
		outputdata[i * 4] = (unit8)(input[i] >> 24);
		outputdata[i * 4 + 1] = (unit8)(input[i] >> 16);
		outputdata[i * 4 + 2] = (unit8)(input[i] >> 8);
		outputdata[i * 4 + 3] = (unit8)(input[i]);
	}
	return 0;
}
int Encrypto(unit8 input[16], unit8 output[16], unit32 rk[32]) // 输入输出，128bit，秘钥32轮子秘钥
{
	unit32 p[4], tempresult;
	datato32(input, p);
	// memcpy(p, input, 16);
	for (int i = 0; i < 32; i++)
	{
		tempresult = p[(i + 3) % 4] ^ p[(i + 2) % 4] ^ p[(1 + i) % 4] ^ rk[i];
		SBox(tempresult);
		LS_1(tempresult);
		p[i % 4] = p[i % 4] ^ tempresult;
	}
	Swap(p);
	datato8(p, output);
	// memcpy(output, p, 16);
	return 0;
}

int Decrypto(unit8 input[16], unit8 output[16], unit32 rk[32])
{
	unit32 p[4], tempresult;
	// memcpy(p, input, 16);
	datato32(input, p);
	for (int i = 0; i < 32; i++)
	{
		tempresult = p[(i + 3) % 4] ^ p[(i + 2) % 4] ^ p[(1 + i) % 4] ^ rk[31 - i];
		SBox(tempresult);
		LS_1(tempresult);
		p[i % 4] = p[i % 4] ^ tempresult;
	}
	Swap(p);
	datato8(p, output);
	// memcpy(output, p, 16);
	//  unit32 temp;
	//  for(int i=0;i<16;i++)
	//  {
	//  	temp=rk[i];
	//  	rk[i]=rk[31-i];
	//  	rk[31-i]=temp;
	//  }
	//  Encrypto(input,output,rk);
	return 0;
}

int KeyExt(unit8 key[16], unit32 rk[32]) // 输入128位主秘钥，输出32轮子秘钥
{
	unit32 k[4], tempresult;
	datato32(key, k);
	// memcpy(k, key, 16);
	//
	k[0] ^= 0xa3b1bac6;
	k[1] ^= 0x56aa3350;
	k[2] ^= 0x677d9197;
	k[3] ^= 0xb27022dc;
	for (int i = 0; i < 32; i++)
	{
		tempresult = k[(i + 3) % 4] ^ k[(i + 2) % 4] ^ k[(1 + i) % 4] ^ ck[i];
		SBox(tempresult);
		LS_2(tempresult);
		k[i % 4] = k[i % 4] ^ tempresult; // 一定要保持K的迭代
		rk[i] = k[i % 4];
		// printf("\nrk[%d]:%x\n", i, rk[i]);
	}
	return 0;
}
// int SBox(unit32 &data)
// {

// 	return 0;
// }

// int RotL(unit32 data,int n)//data循环左移n位
// {

// 	return data;
// }

// int LS_1(unit32 &data)
// {

// 	return 0;
// }

// int LS_2(unit32 &data)
// {

// 	return 0;
// }

// int Swap(unit32 data[4])//
// {

// 	return 0;
// }

// int Encrypto(unit8 input[16],unit8 output[16],unit32 rk[32])//输入输出，128bit，秘钥32轮子秘钥
// {

// 	return 0;
// }

// int KeyExt(unit8 key[16],unit32 rk[32])//输入128位主秘钥，输出32轮子秘钥
// {

// 	return 0;
// }

// int Decrypto(unit8 input[16],unit8 output[16],unit32 rk[32])//输入输出，128bit，秘钥32轮子秘钥
// {

// 	return 0;
// }

/// bmp 跳过54字节的文件格式信息，不要破坏头数据，可以直接从bmp头信息中获取有多个字节数，
/// 34-37 字节所存储的即是像素字节数大小
/// 也可以使用fread，fseek ，rewind 查出字节数长度

/*
BMP格式
这种格式内的数据分为三到四个部分，依次是：
文件信息头 （14字节）存储着文件类型，文件大小等信息
图片信息头 （40字节）存储着图像的尺寸，颜色索引，位平面数等信息
调色板 （由颜色索引数决定）【可以没有此信息】
位图数据 （由图像尺寸决定）每一个像素的信息在这里存储

一般的bmp图像都是24位，也就是真彩。每8位为一字节，24位也就是使用三字节来存储每一个像素的信息，三个字节对应存放r，g，b三原色的数据，
每个字节的存贮范围都是0-255。那么以此类推，32位图即每像素存储r，g，b，a（Alpha通道，存储透明度）四种数据。8位图就是只有灰度这一种信息，
还有二值图，它只有两种颜色，黑或者白。
*/
// 文件信息头结构体
typedef struct tagBITMAPFILEHEADER
{
	// unsigned short bfType;		// 19778，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778，否则不是bmp格式文件
	unsigned int bfSize;		// 文件大小 以字节为单位(2-5字节)
	unsigned short bfReserved1; // 保留，必须设置为0 (6-7字节)
	unsigned short bfReserved2; // 保留，必须设置为0 (8-9字节)
	unsigned int bfOffBits;		// 从文件头到像素数据的偏移  (10-13字节)
} BITMAPFILEHEADER;

// 图像信息头结构体
typedef struct tagBITMAPINFOHEADER
{
	unsigned int biSize;		 // 此结构体的大小 (14-17字节)
	long biWidth;				 // 图像的宽  (18-21字节)
	long biHeight;				 // 图像的高  (22-25字节)
	unsigned short biPlanes;	 // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
	unsigned short biBitCount;	 // 一像素所占的位数，一般为24   (28-29字节)
	unsigned int biCompression;	 // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
	unsigned int biSizeImage;	 // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
	long biXPelsPerMeter;		 // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
	long biYPelsPerMeter;		 // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
	unsigned int biClrUsed;		 // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
	unsigned int biClrImportant; // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} BITMAPINFOHEADER;

BITMAPFILEHEADER fileHeader; // 定义bmp文件头结构体对象
BITMAPINFOHEADER infoHeader; // 定义bmp信息头结构体对象

int ECB_Encrypto(const char *in, const char *out, unit32 rk[32]) // EBC模式加密，从in中读取文件数据加密后写入out文件
{
	FILE *input = fopen(in, "rb");
	FILE *output = fopen(out, "wb");

	if (!input)
	{
		perror("打开失败！\n");
		return -1;
	}
	unsigned short fileType;
	fread(&fileType, 1, sizeof(unsigned short), input);
	if (fileType == 0x4d42)
	{
		printf("打开的是bmp文件\n");

		fseek(input, 0, SEEK_END);
		unsigned int NUM = ftell(input) / sizeof(unit8); // 确定文件字节数量
		rewind(input);

		fread(&fileType, 1, sizeof(unsigned short), input);
		fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, input); // 读出bmp文件头结构体数据
		fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, input); // 读出bmp信息头结构体数据

		// unsigned int NUM = infoHeader.biSizeImage;// b.bmp文件把这个信息写成0了，所以不能这个方式确定要读出的数据

		int len = NUM;
		NUM = NUM + 16 - NUM % 16;
		infoHeader.biSizeImage = NUM;
		unit8 bmpdata[NUM] = {'\0'};
		fread(bmpdata, sizeof(char), NUM, input);

		for (int i = 0; i < 16 - len % 16; i++) // 补齐数据
		{
			bmpdata[NUM - i] = 0xff;
		}

		// printf("---------EBC模式 加密 开始----------");
		unit8 temp_in[16] = {'\0'}, temp_out[16] = {'\0'}; // 转成加密函数需要传入的格式数据，准备开始加密
		for (int i = 0; i < NUM / 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				temp_in[j] = bmpdata[i * 16 + j];
			}
			Encrypto(temp_in, temp_out, rk); // 加密函数，ECB模式，直接分组128bit加密
			for (int j = 0; j < 16; j++)
			{
				bmpdata[i * 16 + j] = temp_out[j]; // 把加密的数据重新填入bmpdata数组，待写入加密文件里
			}
		}
		// printf("---------EBC模式 加密 结束----------");

		printf("----开始写EBC加密文件--------------\n");
		fwrite(&fileType, 1, sizeof(unsigned short), output);
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bmpdata, sizeof(char), NUM, output);
		printf("----结束写EBC加密文件--------------\n");

		fclose(input);
		fclose(output);

		// printf("-----");
	}
	else
	{
		printf("打开的不是bmp文件\n");
		return -1;
	}

	return 0;
}

int ECB_Decrypto(const char *in, const char *out, unit32 rk[32]) // EBC模式解密文件
{
	FILE *input = fopen(in, "rb");
	FILE *output = fopen(out, "wb");

	if (!input)
	{
		perror("打开失败！\n");
		return -1;
	}
	unsigned short fileType;
	fread(&fileType, 1, sizeof(unsigned short), input);
	if (fileType == 0x4d42)
	{
		printf("打开的是bmp文件\n");

		fseek(input, 0, SEEK_END);
		unsigned int NUM = ftell(input) / sizeof(unit8); // 确定文件字节数量
		rewind(input);

		fread(&fileType, 1, sizeof(unsigned short), input);
		fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, input); // 读出bmp文件头结构体数据
		fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, input); // 读出bmp信息头结构体数据

		// unsigned int NUM = infoHeader.biSizeImage;// b.bmp文件把这个信息写成0了，所以不能这个方式确定要读出的数据

		int len = NUM;
		// NUM = NUM + 16 - NUM % 16;
		// infoHeader.biSizeImage = NUM;
		unit8 bmpdata[NUM] = {'\0'};
		fread(bmpdata, sizeof(char), NUM, input);

		// for (int i = 0; i < 16 - len % 16; i++) // 补齐数据
		// {
		// 	bmpdata[NUM - i] = 0x00;
		// }

		// printf("---------EBC模式  开始----------");
		unit8 temp_in[16] = {'\0'}, temp_out[16] = {'\0'}; // 转成函数需要传入的格式数据，准备开始
		for (int i = 0; i < NUM / 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				temp_in[j] = bmpdata[i * 16 + j];
			}
			Decrypto(temp_in, temp_out, rk); // 调用函数，ECB模式，直接分组128bit
			for (int j = 0; j < 16; j++)
			{
				bmpdata[i * 16 + j] = temp_out[j]; // 把数据重新填入bmpdata数组，待写入文件里
			}
		}
		// printf("---------EBC模式 结束----------");

		printf("----开始写EBC解密文件--------------\n");
		fwrite(&fileType, 1, sizeof(unsigned short), output);
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bmpdata, sizeof(char), NUM, output);
		printf("----结束写EBC解密文件--------------\n");

		fclose(input);
		fclose(output);

		// printf("-----");
	}
	else
	{
		printf("打开的不是bmp文件");
		return -1;
	}

	return 0;
}

int XOR_byte_16(unit8 a[16], unit8 b[16])
{
	for (int i = 0; i < 16; i++)
	{
		a[i] ^= b[i];
	}
	return 0;
}

int CBC_Encrypto(const char *in, const char *out, unit32 rk[32])
{
	unit8 iv[16] = {'\0'};

	FILE *input = fopen(in, "rb");
	FILE *output = fopen(out, "wb");

	if (!input)
	{
		perror("打开失败！\n");
		return -1;
	}
	unsigned short fileType;
	fread(&fileType, 1, sizeof(unsigned short), input);
	if (fileType == 0x4d42)
	{
		printf("打开的是bmp文件\n");

		fseek(input, 0, SEEK_END);
		unsigned int NUM = ftell(input) / sizeof(unit8); // 确定文件字节数量
		rewind(input);

		fread(&fileType, 1, sizeof(unsigned short), input);
		fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, input); // 读出bmp文件头结构体数据
		fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, input); // 读出bmp信息头结构体数据

		// unsigned int NUM = infoHeader.biSizeImage;// b.bmp文件把这个信息写成0了，所以不能这个方式确定要读出的数据

		int len = NUM;
		NUM = NUM + 16 - NUM % 16;
		infoHeader.biSizeImage = NUM;
		unit8 bmpdata[NUM] = {'\0'};
		fread(bmpdata, sizeof(char), NUM, input);

		for (int i = 0; i < 16 - len % 16; i++) // 补齐数据
		{
			bmpdata[NUM - i] = 0xff;
		}

		// printf("---------CBC模式 加密 开始----------");
		unit8 temp_in[16] = {'\0'}, temp_out[16] = {'\0'}; // 转成加密函数需要传入的格式数据，准备开始加密
		for (int i = 0; i < NUM / 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				temp_in[j] = bmpdata[i * 16 + j];
			}
			if (i == 0)
			{
				XOR_byte_16(temp_in, iv);
			}
			else
			{
				XOR_byte_16(temp_in, temp_out);
			}
			Encrypto(temp_in, temp_out, rk); // CBC 需要和前一个变量异或后再调用加密函数
			for (int j = 0; j < 16; j++)
			{
				bmpdata[i * 16 + j] = temp_out[j]; // 把加密的数据重新填入bmpdata数组，待写入加密文件里
			}
		}
		// printf("---------CBC模式 加密 结束----------");

		printf("----开始写CBC加密文件--------------\n");
		fwrite(&fileType, 1, sizeof(unsigned short), output);
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bmpdata, sizeof(char), NUM, output);
		printf("----结束写CBC加密文件--------------\n");

		fclose(input);
		fclose(output);
		return 0;
	}
	else
	{
		printf("打开的不是bmp文件");
		return -1;
	}
}

int CBC_Decrypto(const char *in, const char *out, unit32 rk[32])
{
	unit8 iv[16] = {'\0'};

	FILE *input = fopen(in, "rb");
	FILE *output = fopen(out, "wb");

	if (!input)
	{
		perror("打开失败！\n");
		return -1;
	}
	unsigned short fileType;
	fread(&fileType, 1, sizeof(unsigned short), input);
	if (fileType == 0x4d42)
	{
		printf("打开的是bmp文件\n");

		fseek(input, 0, SEEK_END);
		unsigned int NUM = ftell(input) / sizeof(unit8); // 确定文件字节数量
		rewind(input);

		fread(&fileType, 1, sizeof(unsigned short), input);
		fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, input); // 读出bmp文件头结构体数据
		fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, input); // 读出bmp信息头结构体数据

		// unsigned int NUM = infoHeader.biSizeImage;// b.bmp文件把这个信息写成0了，所以不能这个方式确定要读出的数据

		int len = NUM;
		// NUM = NUM + 16 - NUM % 16;
		// infoHeader.biSizeImage = NUM;
		unit8 bmpdata[NUM] = {'\0'};
		fread(bmpdata, sizeof(char), NUM, input);

		// for (int i = 0; i < 16 - len % 16; i++) // 补齐数据
		// {
		// 	bmpdata[NUM - i] = 0x00;
		// }

		// printf("---------CBC模式  开始----------");
		unit8 temp_in[16] = {'\0'}, temp_out[16] = {'\0'}; // 转成函数需要传入的格式数据，准备开始
		for (int i = 0; i < NUM / 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				temp_in[j] = bmpdata[i * 16 + j];
			}

			Decrypto(temp_in, temp_out, rk); // 调用函数，ECB模式，直接分组128bit

			XOR_byte_16(temp_out, iv);

			for (int j = 0; j < 16; j++)
			{
				bmpdata[i * 16 + j] = temp_out[j]; // 把数据重新填入bmpdata数组，待写入文件里
				iv[j] = temp_in[j];				   // 把这个分组的密文传到下次一循环里，进行异或
			}
		}
		// printf("---------CBC模式 结束----------");

		printf("----开始写CBC解密文件--------------\n");
		fwrite(&fileType, 1, sizeof(unsigned short), output);
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bmpdata, sizeof(char), NUM, output);
		printf("----结束写CBC解密文件--------------\n");

		fclose(input);
		fclose(output);

		// printf("-----");
	}
	else
	{
		printf("打开的不是bmp文件");
		return -1;
	}

	return 0;
}

int main()
{
	unit8 key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	unit32 rk[32] = {0};

	const char *text = "D:\\Study\\cpp\\4.SM4\\codes\\task3\\b.bmp";		 // 源文件
	const char *cipher_ecb = "D:\\Study\\cpp\\4.SM4\\codes\\task3\\p1e.bmp"; // ECB模式加密后的文件
	const char *cipher_cbc = "D:\\Study\\cpp\\4.SM4\\codes\\task3\\p1c.bmp"; // CBC模式加密后的文件
	const char *plain = "D:\\Study\\cpp\\4.SM4\\codes\\task3\\pp.bmp";
	const char *plain1 = "D:\\Study\\cpp\\4.SM4\\codes\\task3\\pp1.bmp"; // 解密后的文件
	printf("---------------ECB模式加解密------------------\n");
	ECB_Encrypto(text, cipher_ecb, rk);
	ECB_Decrypto(cipher_ecb, plain, rk);
	printf("---------------CBC模式加解密------------------\n");
	CBC_Encrypto(text, cipher_cbc, rk);
	CBC_Decrypto(cipher_cbc, plain1, rk);
	return 0;
}
