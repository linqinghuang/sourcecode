extern unsigned char quiet=1;
extern unsigned char baoqi=1;
extern unsigned char zongkaiguan=1;	  //¸Õ¿ªÊ¼Òª¹ØµôÉùÒô
#include <reg52.h>
#include <intrins.h>
#include "nrf24l01.h"
#include "12864.h"
#define uchar unsigned char
#define uint unsigned int

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
const uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
sbit CE=P2^0;
sbit CSN=P2^1;
sbit IRQ=P2^2;
sbit MOSI=P1^1;		  //¿ÉÒÔÐÞ¸Ä
sbit MISO=P2^4;
sbit flowled=P1^2;
//sbit SCK=P2^5;	 
sbit SCK=P1^0; // ×Ô¼ºÐÞ¸ÄµÄ	

uchar RxBuf[32];
uchar datedisplay[9];
/*void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}	*

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
  	reg_val = SPI_RW(0XFF);        // È»ºó´Ó¸Ã¼Ä´æÆ÷¶ÁÊý¾Ý		  0
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
//¸Ãº¯Êý³õÊ¼»¯NRF24L01µ½RXÄ£Ê½
//ÉèÖÃRXµØÖ·,Ð´RXÊý¾Ý¿í¶È,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊý¾ÝÁË		   
void RX_Mode(void)
{
	CE=0;	  
  	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH);//Ð´RX½ÚµãµØÖ·
	  
  	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);    //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð    
  	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);//Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  	 
  	SPI_RW_Reg(WRITE_REG+RF_CH,40);	     //ÉèÖÃRFÍ¨ÐÅÆµÂÊ		  
  	SPI_RW_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//Ñ¡ÔñÍ¨µÀ0µÄÓÐÐ§Êý¾Ý¿í¶È 	    
  	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);//ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	SPI_RW_Reg(WRITE_REG+CONFIG, 0x0f);//ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½ 
  	CE = 1; //CEÎª¸ß,½øÈë½ÓÊÕÄ£Ê½ 
}	
//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊý¾Ý
//txbuf:´ý·¢ËÍÊý¾ÝÊ×µØÖ·
//·µ»ØÖµ:0£¬½ÓÊÕÍê³É£»ÆäËû£¬´íÎó´úÂë
uchar NRF24L01_RxPacket(uchar *rxbuf)
{
	uchar sta;		    							   
	sta=SPI_Read(STATUS);  //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ    	 
	SPI_RW_Reg(WRITE_REG+STATUS,sta); //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾
	if(sta&RX_OK)//½ÓÊÕµ½Êý¾Ý
	{
		SPI_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//¶ÁÈ¡Êý¾Ý
		SPI_RW_Reg(FLUSH_RX,0xff);//Çå³ýRX FIFO¼Ä´æÆ÷ 
		return 0; 
	}	   
	return 1;//Ã»ÊÕµ½ÈÎºÎÊý¾Ý
}
void main()
{
	uchar i; //×Ô¼º¼ÓµÄ
//	uchar datedisplay[7];
	aa=0xfe;
	init_io();
	P0=0x00;//ÆÁ±ÎÊýÂë¹Ü
	 delay(10);                 //ÑÓÊ±
    wela=0;
	dula=0; 
//	inte();
 lcd_init2();	
	/******************×Ô¼º¼ÓµÄ**************************/
	while(NRF_Check());	

	RX_Mode();//	
	interface();
	while(NRF24L01_RxPacket(RxBuf)!=0);
	while(1)
	{
		if(NRF24L01_RxPacket(RxBuf)==0)		
		{
	//		for(i=0;i<8;i++)
	//		datedisplay[i]=RxBuf[i];	
	//	}
		
			while(1)
			{
				P1=aa;
				delayms(500);		  //延时500毫秒
				aa=_crol_(aa, 1);

			}
		}
	checkbutton();
	lcd12864(datedisplay);

	}		
}