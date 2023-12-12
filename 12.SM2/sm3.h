/*SM3杂凑函数实现*/

#include<stdio.h>
#include<string.h>
#include<math.h>

typedef unsigned char unit8;
typedef unsigned int  unit32;

unit32 T_Table[2]={0x79CC4519,0x7A879D8A};
unit32 T(int j)
{
	if (j>=0&&j<=15) return T_Table[0];
	else if(j>=16&&j<=63) return  T_Table[1];
	else return 0;
}
unit32 IV[8]={0x7380166F,0x4914B2B9,0x172442D7,0xDA8A0600,
			  0xA96F30BC,0x163138AA,0xE38DEE4D,0xB0FB0E4E};

unit32 End_Swap(unit32 x) 
{
	return ((x&0x000000ff)<<24)|((x&0x0000ff00)<<8)|((x&0x00ff0000)>>8)|((x&0xff000000)>>24);
}

unit32 GetWord_BE(unit8 *input)
{
	unit32 w=0;
	return w=(unit32)input[0]<<24|(unit32)input[1]<<16|(unit32)input[2]<<8|(unit32)input[3];
}

void GetByte(unit32 input,unit8 output[])
{
	output[0]=(unsigned char) ((input)>>24);
	output[1]=(unsigned char) ((input)>>16);
	output[2]=(unsigned char) ((input)>>8);
	output[3]=(unsigned char) (input);
}

#define FF0(x,y,z) ( (x) ^ (y) ^ (z)) 
#define FF1(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x,y,z) ( (x) ^ (y) ^ (z)) 
#define GG1(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )


#define  SHL(x,n) (((x) & 0xFFFFFFFF) << n)
#define ROTL(x,n) (SHL((x),n) | ((x) >> (32 - n)))

#define P0(x) ((x) ^  ROTL((x),9) ^ ROTL((x),17)) 
#define P1(x) ((x) ^  ROTL((x),15) ^ ROTL((x),23)) 

unit32 FF(unit32 x,unit32 y,unit32 z,int j)
{
	if (j>=0&&j<=15) return FF0(x,y,z);//x^y^z;
	else if(j>=16&&j<=63) return FF1(x,y,z);//(x&y)|(x&z)|(y&z);
	else return 0;
}

unit32 GG(unit32 x,unit32 y,unit32 z,int j)
{
	if (j>=0&&j<=15) return GG0(x,y,z);//x^y^z;
	else if(j>=16&&j<=63) return GG1(x,y,z);//(x&y)|((~x)&z);
	else return 0;
}

int Message_Pad(unit8 *input,int m_length,unit8 *output)//输入任意字符串，输出扩展后的字节流，并返回分组B的数量 
{
	int pad_length=0;
	pad_length=(56-m_length%64+64)%64;//填充字节数
	int n=(m_length+pad_length+8)/64;//分组数 
	
	memcpy(output,input,m_length);
	memset(output+m_length,0x80,1);
	memset(output+m_length+1,0x00,pad_length-1+4);
	int bit_length=End_Swap(m_length*8);
	memcpy(output+m_length+pad_length+4,&bit_length,4);	 
//	
//	printf("\n填充后的消息为：");
//	for(int i=0;i<n*64;i++)
//	{
//		if(i%32==0) printf("\n");
//		printf("%02x",output[i]);
//		if((i+1)%4==0) printf("  ");
//	}
	return n;//填充后总字节数 
} 

int Message_Ex(unit8 *input,unit32 w[132])//输入填充后的字节流及其字节数，输出扩展后的字节流 
{

	for(int i=0;i<16;i++) {w[i]=GetWord_BE(input+i*4); }//前16个字构造 
	
	for(int j=16;j<=67;j++)
	{
		w[j]=P1(w[j-16]^w[j-9]^ROTL(w[j-3],15))^ROTL(w[j-13],7)^w[j-6];//16-67个字 
	} 
	for(int k=0;k<=63;k++)
	{
		w[k+68]=w[k]^w[k+4];//后面64个字	
	} 
//	printf("\n\n扩展后的消息为：");
//	for(int i=0;i<132;i++)
//	{
//		if(i%8==0) printf("\n");
//		printf("%08x  ",w[i]);
//	} 	
	return 0;
}

void CF(unit32 r[8],unit32 w[132],unit32 v[8])//压缩函数，输入字Vi和Bi，输出Vi+1 
{
	unit32 SS1,SS2,TT1,TT2;
	memcpy(v,r,32);
	
//	printf("\n\n迭代压缩中间值：");
//	printf("\n初始值：");for(int h=0;h<8;h++) printf("%08x  ",r[h]);
	for(int i=0;i<64;i++)
	{
//		printf("\nr[4]=%08x",r[4]);		
		SS1=ROTL((ROTL(r[0],12)+r[4]+ROTL(T(i),i)),7);
//		printf("\nRL(A,12)=%08x,r[4]=%08x,RL(Ti)=%08x",(ROTL(r[0],12),r[4],ROTL(T(i),i)));
//		printf("\n##SS1=%08x",SS1);	//SS1=((A<<<12)+E+(Tj<<<j))<<<7
		SS2=SS1^ROTL(r[0],12);//SS1xor(A<<<12)
		TT1=FF(r[0],r[1],r[2],i)+r[3]+SS2+w[68+i];//TT1=FFj(A,B,C)+D+SS2+wj`
		TT2=GG(r[4],r[5],r[6],i)+r[7]+SS1+w[i];//TT2=GGJ(E,F,G)+H+SS1+wj
		r[3]=r[2];//D=C
		r[2]=ROTL(r[1],9);//C=(B<<<9)
		r[1]=r[0];//B=A
		r[0]=TT1;//A=TT1
		r[7]=r[6];//H=G
		r[6]=ROTL(r[5],19);//G=(F<<<19)
		r[5]=r[4];//F=E
		r[4]=P0(TT2);//E=P0(TT2)
//		printf("\n第%02d轮：",i);for(int h=0;h<8;h++) printf("%08x  ",r[h]);		
	}	

	for(int j=0;j<8;j++) r[j]=v[j]^r[j];
	memcpy(v,r,32);
}

void sm3(unit8 *input,int m_length,unit8 *output)
{
	int i=0;
	unit8 temp[1024]={0};
	unit32 w[132]={0};
	unit32 r[8]={0};//A-H8个字寄存器 
	unit32 v[8]={0};//迭代输出的杂凑值
	memcpy(r,IV,32);//初始变量
	int n=Message_Pad(input,m_length,temp);//填充返回512比特长分组数
	for(i=0;i<n;i++)
	{
		Message_Ex(temp+i*64,w);
		CF(r,w,v);
	}
	//memcpy(output,v,32);
	for(i=0;i<8;i++) GetByte(v[i],output+i*4);
}

//
//int main()
//{
//	
//	unit8 *text=(unit8*)"abc";
//				//"abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
//	//unit32 output[8]={0};
//	unit8 output[64]={0};
//	printf("原文是：%s\n",text);
//	
//	//unit8 temp_pad[1024]={0};
//	int m_length=0;	for(m_length=0;text[m_length]!='\0';m_length++);//明文长度 
//
//	sm3(text,m_length,output);
//	printf("\n杂凑值:\n  ");
//	for(int i=0; i<32; i++)
//	{
//		printf("%02x",output[i]);
//		if((i+1)%4==0) printf("  ");
//	}   
//	printf("\n");
//	
//	return 0;
//}
