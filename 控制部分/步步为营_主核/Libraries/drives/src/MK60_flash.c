/***************************************/
/********内容原引自山外**************/
/***************************************/
/*更改于2017.11.8  By Alex*/

#include "MK60_flash.h"




#if (defined(MK60DZ10) || defined(MK60D10))
#define     FCMD        FTFL_FCCOB0         //FTFL 命令

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //注意一点，4字节排序，FDATA3是最低位，FDATA0是最高位
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD 命令
#define     RD1BLK    0x00   // 读整块Flash
#define     RD1SEC    0x01   // 读整个扇区
#define     PGMCHK    0x02   // 写入检查
#define     RDRSRC    0x03   // 读目标数据(4字节)
#define     PGM4      0x06   // 写入长字(4字节)
#define     ERSBLK    0x08   // 擦除整块Flash
#define     ERSSCR    0x09   // 擦除Flash扇区
#define     PGMSEC    0x0B   // 写入扇区
#define     RD1ALL    0x40   // 读所有的块
#define     RDONCE    0x41   // 只读一次
#define     PGMONCE   0x43   // 只写一次
#define     ERSALL    0x44   // 擦除所有块
#define     VFYKEY    0x45   // 验证后门访问钥匙
#define     PGMPART   0x80   // 写入分区
#define     SETRAM    0x81   // 设定FlexRAM功能

#elif (defined(MK66F18) || defined(MK60F15))
#define     FCMD        ((FTFE)->FCCOB0)         //FTFL 命令     
#define     FADDR2      ((FTFE)->FCCOB1)         //Flash address [23:16]
#define     FADDR1      ((FTFE)->FCCOB2)         //Flash address [15:8]
#define     FADDR0      ((FTFE)->FCCOB3)         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8字节对齐

#define     FDATA0      ((FTFE)->FCCOB4)         //Data Byte 0       //注意一点，4字节排序，FDATA3是最低位，FDATA0是最高位
#define     FDATA1      ((FTFE)->FCCOB5)         //Data Byte 1
#define     FDATA2      ((FTFE)->FCCOB6)         //Data Byte 2
#define     FDATA3      ((FTFE)->FCCOB7)         //Data Byte 3
#define     FDATA4      ((FTFE)->FCCOB8)         //Data Byte 4
#define     FDATA5      ((FTFE)->FCCOB9)         //Data Byte 5
#define     FDATA6      ((FTFE)->FCCOBA)         //Data Byte 6
#define     FDATA7      ((FTFE)->FCCOBB)         //Data Byte 7

//FCMD 命令
#define     RD1BLK    0x00   // 读整块Flash
#define     RD1SEC    0x01   // 读整个扇区
#define     PGMCHK    0x02   // 写入检查
#define     RDRSRC    0x03   // 读目标数据(8字节)
#define     PGM8      0x07   // 写入长字(8字节)
#define     ERSBLK    0x08   // 擦除整块Flash
#define     ERSSCR    0x09   // 擦除Flash扇区
#define     PGMSEC    0x0B   // 写入扇区
#define     RD1ALL    0x40   // 读所有的块
#define     RDONCE    0x41   // 只读一次
#define     PGMONCE   0x43   // 只写一次
#define     ERSALL    0x44   // 擦除所有块
#define     VFYKEY    0x45   // 验证后门访问钥匙
#define     PGMPART   0x80   // 写入分区
#define     SETRAM    0x81   // 设定FlexRAM功能

#endif




__ramfunc uint8 FLASH_CMD()
{
#if (defined(MK60DZ10) || defined(MK60D10))
    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFL_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );
    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // 等待命令完成
    return 'T';

#elif (defined(MK66F18) || defined(MK60F15))
    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFE_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // 等待命令完成

    return 'T';

#endif  
}

__ramfunc void FLASH_init()
{
#if (defined(MK60DZ10) || defined(MK60D10))
    // 清除Flash预读取缓冲区
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;      //
    FMC_PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;

    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));    // 等待命令完成

    FTFL_FSTAT =    (0
                     |  FTFL_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFL_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFL_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFL_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );
#elif (defined(MK66F18) || defined(MK60F15))
    // 清除Flash预读取缓冲区
    FMC_PFB01CR |= FMC_PFB01CR_S_B_INV_MASK;      //

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // 等待命令完成

    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFE_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );
#endif
}

__ramfunc uint8 FLASH_ERASE(uint16 No)
{
    uint32 addr = No * FLASH_SECTOR_SIZE;
    // 设置擦除命令
    FCMD = ERSSCR;
    // 设置目标地址
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];
    while(FLASH_CMD() != 'T');
    return 'T';
}

__ramfunc uint8 FLASH_WRITE(uint16 No, uint16 offset, FLASH_WRITE_TYPE data)
{
    uint32 addr = No * FLASH_SECTOR_SIZE  + offset ;
    uint32 tmpdata;
    // 设置目标地址
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];
    // 设置 低32位数据
    tmpdata = (uint32)data;
    FDATA0 = ((Dtype *)&tmpdata)->B[3];                    // 设置写入数据
    FDATA1 = ((Dtype *)&tmpdata)->B[2];
    FDATA2 = ((Dtype *)&tmpdata)->B[1];
    FDATA3 = ((Dtype *)&tmpdata)->B[0];  
#if (defined(MK60DZ10) || defined(MK60D10))
    FCMD = PGM4;
#elif (defined(MK66F18) || defined(MK60F15))
    // 设置擦除命令
    FCMD = PGM8;

    // 设置 高32位数据
    tmpdata = (uint32)(data>>32);

    FDATA4 = ((Dtype *)&tmpdata)->B[3];                 // 设置写入数据
    FDATA5 = ((Dtype *)&tmpdata)->B[2];
    FDATA6 = ((Dtype *)&tmpdata)->B[1];
    FDATA7 = ((Dtype *)&tmpdata)->B[0];
#endif
    while(FLASH_CMD() != 'T');
    return 'T';   
}

void FLASH_DELAY(uint32 ms)
{
  while(ms--);
}




