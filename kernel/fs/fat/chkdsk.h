#ifndef _CHKDSK_H_
#define _CHKDSK_H_
#include <plat-anyka/anyka_types.h>
#include "fat.h"


#define FAT16_MAX_USER_CLUSTER    0XFFF
#define FAT32_MAX_USER_CLUSTER    0XFFFFFF8
typedef    struct tag_Medium  T_MEDIUM;

typedef    struct tag_Medium *T_PMEDIUM;


typedef    T_U32 (*F_ReadSector)(T_PMEDIUM medium, T_U8** buf, T_U32 start, T_U32 size);
typedef    T_U32 (*F_WriteSector)(T_PMEDIUM medium, const T_U8 *buf, T_U32 start, T_U32 size);

struct tag_Medium
{
	struct super_block *sb;

    T_U8  SecBit;
    T_U8  PageBit;  // 2^n = SecPerPage.
    T_U8  SecPerPg; //PageBit -SecBit .
};


#define CLUSTER_MAP_NUMBER  100
typedef struct _CLUSTER_MAP_ARRAY_INFO *PSECTOR_MAP_ARRAY_INFO;

typedef struct _CLUSTER_MAP_INFO
{
	T_U32 sectorAddr;	//�˻���������Ӧ��SECOTR,��ΪFAT������̫��,���Զ���ΪT_U16�Ա��ʡ�ڴ�,�����ֵΪ-1,��ʾ���������û�б���ʹ��.
	T_U32 bitNum;		//��¼λͼ���ж��ٸ�λ����1��,���������CluserPerSector,����ʾλͼΪȫ1״̬,���ͷŴ˻�����.
	T_U8 *bitMap;		//λͼ����,������ָ����,����Ϊ��ʱ����1,����Ϊ0,���ȫΪ1ʱ,���ͷ����������(��sectorAddr = T_U16_MAX),�Ա���������ʹ��.����������Ĵ�СΪBytesPerSector/8.
} CLUSTER_MAP_INFO, *PCLUSTER_MAP_INFO;

typedef struct _CLUSTER_MAP_ARRAY_INFO
{
	T_U32 index;		//�ػ�����,ÿ�η���100��.
	PSECTOR_MAP_ARRAY_INFO next;		//ָ����һ���󻺳���
	PCLUSTER_MAP_INFO pMapArray;		//�ػ�����,ÿ�η���100��.
	PSECTOR_MAP_ARRAY_INFO pCurSecMap;		//ָ��ǰ����ʹ�õ�С������
}SECTOR_MAP_ARRAY_INFO;


enum{MARK_FAT_OK, FAT_LINK_ERROR, MARK_MALLOC_ERROR, FAT_READ_ERROR};

#define TestBitMap(BitMap, item)    (((BitMap)[(item)>>3]&(1<<((item)&7))))
#define SetBitMap(BitMap, item)     ((BitMap)[(item)>>3] |= (1<<((item)&7)))
#define ClrBitMap(BitMap, item)     ((BitMap)[(item)>>3] &= ~(1<<((item)&7)))

typedef void F_ChkDskCallback(T_VOID *pData, T_U32 percent);
T_BOOL Fat_ChkDsk(struct super_block *sb, F_ChkDskCallback pCallBack, T_VOID *CallbackData);

//T_U32 FAT_GetFatLinkInfo(struct msdos_sb_info *sbi, T_U8 * pFatBuf, T_U16 offset);
//T_U32 FAT_GetFatLinkInfo_chkdsk(struct msdos_sb_info *sbi, T_U8 * pFatBuf, T_U16 offset);
//T_VOID FAT_SetFatLinkInfo(struct msdos_sb_info *sbi, T_U8 * pFatBuf, T_U16 offset, T_U32 newValue);


#endif

