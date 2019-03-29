/*********************************************************************************
  *Copyright(C),中国国家密码管理局
  *FileName:祖冲之序列密码算法的完整性算法
  *Author:TFflyer
  *Version:V1.0
  *Date:2019-03-28
  *Description: demo and without main()，主要实现对信息完整性的保护，生成MAC值
  *Others: my email:myownflyer@foxmail.com
  *Function List:  
     1.
     2.
	 3.
  *History:  
     1.Date:
       Author:
	   Modification:
**********************************************************************************/


#include"ZUC_mian.h"
#include<malloc.h>



typedef unsigned char u8; 
typedef unsigned int u32;
void ZUC2( u8* k, u8 *iv,  u32* keystream,  int length) {
	Initialization(k, iv);
	GenerateKeystream(keystream, length);
}
/*see Annex 1*/ 
u32 GET_WORD(u32 * DATA, u32 i) 
{
	u32 WORD, ti;
	ti = i % 32;
	if (ti == 0) 
	{
		WORD = DATA[i / 32];
	}
	else { WORD = (DATA[i / 32] << ti) | (DATA[i / 32 + 1] >> (32 - ti)); 
	}
	return WORD;
}
u8 GET_BIT(u32 * DATA, u32 i) {
	return (DATA[i / 32] & (1 << (31 - (i % 32)))) ? 1 : 0;
}

void EIA3( u8* IK, extern u32 COUNT, extern u32 DIRECTION, u32 BEARER, u32 LENGTH, u32* M, u32* MAC)
{
	u32 *z, N, L, T, i; 
	u8 IV[16];
	IV[0] = (COUNT >> 24) & 0xFF; 
	IV[1] = (COUNT >> 16) & 0xFF; 
	IV[2] = (COUNT >> 8) & 0xFF; 
	IV[3] = COUNT & 0xFF;

	IV[4] = (BEARER << 3) & 0xF8; 
	IV[5] = IV[6] = IV[7] = 0;

	IV[8] = ((COUNT >> 24) & 0xFF) ^ ((DIRECTION & 1) << 7); 
	IV[9] = (COUNT >> 16) & 0xFF; 
	IV[10] = (COUNT >> 8) & 0xFF; 
	IV[11] = COUNT & 0xFF;

	IV[12] = IV[4];
	IV[13] = IV[5];
		
	IV[14] = IV[6] ^ ((DIRECTION & 1) << 7); 
	IV[15] = IV[7];
	N = LENGTH + 64; 
	L = (N + 31) / 32; 
	z = (u32 *)malloc(L * sizeof(u32));
	ZUC2(IK, IV, z, L);

	T = 0;
	for (i = 0; i < LENGTH; i++) {
		if (GET_BIT(M, i)) {
			T ^= GET_WORD(z, i);
		}
	} T ^= GET_WORD(z, LENGTH);
	*MAC = T ^ z[L - 1]; 

	free(z);
} 