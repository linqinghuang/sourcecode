#ifndef __EEROM__
#define __EEROM__
#include<intrins.h> 
#define RdCommand 0x01		//定义ISP操作命令
#define PrgCommand 0x02
#define EraseCommand 0x03
#define Error 1
#define Ok 0
#define WaitTime 0x01  
sfr ISP_DATA=0xe2;
sfr ISP_ADDRH=0XE3;
sfr ISP_ADDRL=0XE4;
sfr ISP_CMD=0XE5;
sfr ISP_TRIG=0XE6;
sfr ISP_CONTR=0XE7;
void ISP_IAP_enable(void)
{
	EA=0;//关中断  
	ISP_CONTR=ISP_CONTR&0x18;
	ISP_CONTR=ISP_CONTR|WaitTime;
	ISP_CONTR=ISP_CONTR|0X80;
}
//关闭ISP/IAP功能
void ISP_IAP_disable(void)
{
	ISP_CONTR=ISP_CONTR&0X7F;
	ISP_TRIG=0X00;
	EA=0;
}

void ISPgoon(void)
{
	ISP_IAP_enable();
	ISP_TRIG=0X46;
	ISP_TRIG=0xb9;
	_nop_();
}

unsigned char byte_read(unsigned int byte_addr)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8);
	ISP_ADDRL=(unsigned char)(byte_addr&0x00ff);
	ISP_CMD=ISP_CMD&0XF8;
	ISP_CMD=ISP_CMD|RdCommand;
	ISPgoon();
	ISP_IAP_disable();
	return(ISP_DATA);
}
//扇区擦除
void SectorErase(unsigned int sector_addr)
{
	unsigned int iSectorAddr;
	iSectorAddr=(sector_addr&0xfe00);	
	ISP_ADDRH=(unsigned char)(iSectorAddr>>8);
	ISP_ADDRL=0X00;
	ISP_CMD=ISP_CMD&0XF8;	
	ISP_CMD=ISP_CMD|EraseCommand;
	ISPgoon();
	ISP_IAP_disable();
}

void byte_write(unsigned int byte_addr,unsigned char original_data)
{
	ISP_ADDRH=(unsigned char)(byte_addr>>8);
	ISP_ADDRL=(unsigned char)(byte_addr&0x00ff);
	ISP_CMD=ISP_CMD&0XF8;	
	ISP_CMD=ISP_CMD|PrgCommand;
	ISP_DATA=original_data;
	ISPgoon();
	ISP_IAP_disable();
}
#endif