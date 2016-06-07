#include <reg52.h>
#include "nrf24l01.h"
#include "wsjiance.h"
#include "cejv.h"
#include "yalijiance.h"
#include "kaisuo.h"
#include "eerom.h"
#define uchar unsigned char
#define uint unsigned int
#define RI 1

//const uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
//const uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
sbit CE=P2^0;
sbit CSN=P2^1;
sbit IRQ=P2^2;
sbit MOSI=P3^1;
sbit MISO=P2^4;

sbit SCK=P3^0;		  
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
 sbit led3=P1^3;	  //  ÒªÉ¾È¥µ±È»
sbit ledk=P2^5;		 //×Ô¼ºÐÞ¸ÄµÄ¶î
sbit out=P3^6;
void delayms(uint i)
{
	uint x,y;
	for(x=i;x>0;x--)
		for(y=110;y>0;y--);
}
/**************************************************
º¯Êý: init_io()

ÃèÊö:
    ³õÊ¼»¯IO
/**************************************************/
void init_io(void)
{
	CE  = 0;        // ´ý»ú
	CSN = 1;        // SPI½ûÖ¹
	SCK = 0;        // SPIÊ±ÖÓÖÃµÍ
	IRQ = 1;        // ÖÐ¶Ï¸´Î»
}
/**************************************************/


/**************************************************
º¯Êý£ºSPI_RW()

ÃèÊö£º
    ¸ù¾ÝSPIÐ­Òé£¬Ð´Ò»×Ö½ÚÊý¾Ýµ½nRF24L01£¬Í¬Ê±´ÓnRF24L01
	¶Á³öÒ»×Ö½Ú
/**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar i;
   	for(i=0; i<8; i++)          // Ñ­»·8´Î
   	{
   		MOSI = (byte & 0x80);   // byte×î¸ßÎ»Êä³öµ½MOSI
   		byte <<= 1;             // µÍÒ»Î»ÒÆÎ»µ½×î¸ßÎ»
   		SCK = 1;                // À­¸ßSCK£¬nRF24L01´ÓMOSI¶ÁÈë1Î»Êý¾Ý£¬Í¬Ê±´ÓMISOÊä³ö1Î»Êý¾Ý
   		byte |= MISO;       	// ¶ÁMISOµ½byte×îµÍÎ»
   		SCK = 0;            	// SCKÖÃµÍ
   	}
    return(byte);           	// ·µ»Ø¶Á³öµÄÒ»×Ö½Ú
}
/**************************************************/

/**************************************************
º¯Êý£ºSPI_RW_Reg()

ÃèÊö£º
    Ð´Êý¾Ývalueµ½reg¼Ä´æÆ÷
/**************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
  	CSN = 0;                   // CSNÖÃµÍ£¬¿ªÊ¼´«ÊäÊý¾Ý
  	status = SPI_RW(reg);      // Ñ¡Ôñ¼Ä´æÆ÷£¬Í¬Ê±·µ»Ø×´Ì¬×Ö
  	SPI_RW(value);             // È»ºóÐ´Êý¾Ýµ½¸Ã¼Ä´æÆ÷
  	CSN = 1;                   // CSNÀ­¸ß£¬½áÊøÊý¾Ý´«Êä
  	return(status);            // ·µ»Ø×´Ì¬¼Ä´æÆ÷
}
/**************************************************/


/**************************************************
º¯Êý£ºSPI_Read()

ÃèÊö£º
    ´Óreg¼Ä´æÆ÷¶ÁÒ»×Ö½Ú
/**************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
  	CSN = 0;                    // CSNÖÃµÍ£¬¿ªÊ¼´«ÊäÊý¾Ý
  	SPI_RW(reg);                // Ñ¡Ôñ¼Ä´æÆ÷
  	reg_val = SPI_RW(0);        // È»ºó´Ó¸Ã¼Ä´æÆ÷¶ÁÊý¾Ý		  0
  	CSN = 1;                    // CSNÀ­¸ß£¬½áÊøÊý¾Ý´«Êä
  	return(reg_val);            // ·µ»Ø¼Ä´æÆ÷Êý¾Ý
}
/**************************************************/

/**************************************************
º¯Êý£ºSPI_Read_Buf()

ÃèÊö£º
    ´Óreg¼Ä´æÆ÷¶Á³öbytes¸ö×Ö½Ú£¬Í¨³£ÓÃÀ´¶ÁÈ¡½ÓÊÕÍ¨µÀ
	Êý¾Ý»ò½ÓÊÕ/·¢ËÍµØÖ·
/**************************************************/
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSNÖÃµÍ£¬¿ªÊ¼´«ÊäÊý¾Ý
  	status = SPI_RW(reg);       // Ñ¡Ôñ¼Ä´æÆ÷£¬Í¬Ê±·µ»Ø×´Ì¬×Ö
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    // Öð¸ö×Ö½Ú´ÓnRF24L01¶Á³ö
  	CSN = 1;                    // CSNÀ­¸ß£¬½áÊøÊý¾Ý´«Êä
  	return(status);             // ·µ»Ø×´Ì¬¼Ä´æÆ÷
}
/**************************************************/

/**************************************************
º¯Êý£ºSPI_Write_Buf()

ÃèÊö£º
    °ÑpBuf»º´æÖÐµÄÊý¾ÝÐ´Èëµ½nRF24L01£¬Í¨³£ÓÃÀ´Ð´Èë·¢
	ÉäÍ¨µÀÊý¾Ý»ò½ÓÊÕ/·¢ËÍµØÖ·
/**************************************************/
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSNÖÃµÍ£¬¿ªÊ¼´«ÊäÊý¾Ý
  	status = SPI_RW(reg);       // Ñ¡Ôñ¼Ä´æÆ÷£¬Í¬Ê±·µ»Ø×´Ì¬×Ö
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);        // Öð¸ö×Ö½ÚÐ´ÈënRF24L01
  	CSN = 1;                    // CSNÀ­¸ß£¬½áÊøÊý¾Ý´«Êä
  	return(status);             // ·µ»Ø×´Ì¬¼Ä´æÆ÷
}
/**************************************************/

//¼ì²â24L01ÊÇ·ñ´æÔÚ
//·µ»ØÖµ:0£¬³É¹¦;1£¬Ê§°Ü	
uchar NRF_Check(void)
{
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uchar i;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//Ð´Èë5¸ö×Ö½ÚµÄµØÖ·.	
	SPI_Read_Buf(TX_ADDR,buf,5); //¶Á³öÐ´ÈëµÄµØÖ·  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//¼ì²â24L01´íÎó	
	return 0;		 //¼ì²âµ½24L01
}

void TX_Mode(void)
{														 
	CE=0;	    
  	SPI_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//Ð´TX½ÚµãµØÖ· 
    SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)TX_ADDRESS,TX_ADR_WIDTH); //ÉèÖÃTX½ÚµãµØÖ·,Ö÷ÒªÎªÁËÊ¹ÄÜACK	  

  	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);     //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð    
  	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01); //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  
  	SPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);//ÉèÖÃ×Ô¶¯ÖØ·¢¼ä¸ôÊ±¼ä:500us + 86us;×î´ó×Ô¶¯ÖØ·¢´ÎÊý:10´Î
  	SPI_RW_Reg(WRITE_REG+RF_CH,40);       //ÉèÖÃRFÍ¨µÀÎª40
  	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);  //ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	SPI_RW_Reg(WRITE_REG+CONFIG,0x0e);    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½,¿ªÆôËùÓÐÖÐ¶Ï
	CE=1;//CEÎª¸ß,10usºóÆô¶¯·¢ËÍ
}
//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊý¾Ý
//txbuf:´ý·¢ËÍÊý¾ÝÊ×µØÖ·
//·µ»ØÖµ:·¢ËÍÍê³É×´¿ö
uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar sta; 
	CE=0;
  	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//Ð´Êý¾Ýµ½TX BUF  32¸ö×Ö½Ú
 	CE=1;//Æô¶¯·¢ËÍ	   
	while(IRQ!=0);//µÈ´ý·¢ËÍÍê³É
	sta=SPI_Read(STATUS);  //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ	   
	SPI_RW_Reg(WRITE_REG+STATUS,sta); //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾
	if(sta&MAX_TX)//´ïµ½×î´óÖØ·¢´ÎÊý
	{
		SPI_RW_Reg(FLUSH_TX,0xff);
		return MAX_TX; 
	}
	if(sta&TX_OK)
	{
		return TX_OK;
	}
	return 1;
}

void wenshi(uchar shujv[])
{	 
		uint i=0;
		RH(); 
		shujv[0]=U8RH_data_H/10+48;	
		shujv[1]=U8RH_data_H%10+48;
		shujv[2]=U8T_data_H/10+48;
		shujv[3]=U8T_data_H%10+48;
		for(i=0;i<3000;i++);
}
void main()			   
{													  
//	uchar shujv[8];		
	uchar stop=0;
	uchar cejv=1; 
	uchar flag=0;
	uchar helpcejv=0;	 
	init_io();
	led0=1;led1=1;ledk=1; 
	while(NRF_Check())	 
	{
		led0=0;
		delayms(1000);
		led0=1;
		delayms(1000);								  
	}	
	TX_Mode();		   //ÉèÖÃÎª·¢ËÍÄ£Ê½
	while(1)
	{
		uchar sta1;
		int TxBuf[8]={1,2,3,4,5,6,7,8};
		delayms(1000);
		NRF24L01_TxPacket(TxBuf);
		delayms(1000);
		while(IRQ!=0);//µÈ´ý·¢ËÍÍê³É
		sta1=SPI_Read(STATUS);  //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ	   
		SPI_RW_Reg(WRITE_REG+STATUS,sta1); //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾	
		
	}
	

}