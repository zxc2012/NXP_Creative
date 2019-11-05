#ifndef FLASH_H
#define FLASH_H

#include "common.h"

typedef union
{
    uint32  DW;
    uint16  W[2];
    uint8   B[4];
    struct
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    };
} Dtype;    //sizeof(Dtype) 为 4


#if (defined(MK60DZ10) || defined(MK60D10))
#define     FLASH_SECTOR_SIZE       (2*1024)                //扇区大小 为 2k 字节
#define     FLASH_SECTOR_NUM        (256)                   //扇区数

#define     FLASH_ALIGN_ADDR        4                       //地址对齐整数倍
typedef    uint32                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型
#define     FTFL_FSTAT              ((FTFL)->FSTAT)
#elif defined(MK60F15)
#define     FLASH_SECTOR_SIZE       (4*1024)                //扇区大小 为 4k 字节
#define     FLASH_SECTOR_NUM        (128)                   //扇区数   /////如果是FN15则为256

#define     FLASH_ALIGN_ADDR        8                       //地址对齐整数倍
#define     FTFE_FSTAT              ((FTFE)->FSTAT)
#define     FMC_PFB01CR             ((FMC)->PFB01CR)
typedef     uint64                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型

#elif defined(MK66F18)
#define     FLASH_SECTOR_SIZE       (4*1024)                //扇区大小 为 4k 字节
#define     FLASH_SECTOR_NUM        (512)                   //扇区数

#define     FLASH_ALIGN_ADDR        8                       //地址对齐整数倍
typedef     uint32                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型
#endif


__ramfunc uint8 FLASH_CMD();
__ramfunc void FLASH_init();
__ramfunc uint8 FLASH_ERASE(uint16 No);
__ramfunc uint8 FLASH_WRITE(uint16 No, uint16 offset, FLASH_WRITE_TYPE data);
void FLASH_DELAY(uint32 ms);

#define  BOOTSECTOR     (FLASH_SECTOR_NUM - 1)
#define  FLASH_READ(No,offset,type)        (*(type *)((uint32)(((No)*FLASH_SECTOR_SIZE) + (offset))))          //读取扇区



#endif