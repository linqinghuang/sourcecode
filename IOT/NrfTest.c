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

//const uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; 
//const uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; 
sbit CE=P2^0;
sbit CSN=P2^1;
sbit IRQ=P2^2;
sbit MOSI=P3^1;
sbit MISO=P2^4;

sbit SCK=P3^0;		  
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
 sbit led3=P1^3;	 
sbit ledk=P2^5;		
sbit out=P3^6;
void delayms(uint i)
{
	uint x,y;
	for(x=i;x>0;x--)
		for(y=110;y>0;y--);
}

void init_io(void)
{
	CE  = 0;        
	CSN = 1;        
	SCK = 0;        
	IRQ = 1;       
}
/**************************************************/


uchar SPI_RW(uchar byte)
{
	uchar i;
   	for(i=0; i<8; i++)         
   	{
   		MOSI = (byte & 0x80);   
   		byte <<= 1;             
   		SCK = 1;                
   		byte |= MISO;       	
   		SCK = 0;      
	}
    return(byte);           	
}



uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
  	CSN = 0;                   
  	status = SPI_RW(reg);      
  	SPI_RW(value);             
  	CSN = 1;                   
  	return(status);            
}
/**************************************************/



uchar SPI_Read(uchar reg)
{
	uchar reg_val;
  	CSN = 0;                  
  	SPI_RW(reg);               
  	reg_val = SPI_RW(0);      
  	CSN = 1;                  
  	return(reg_val);         
}



uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                   
  	status = SPI_RW(reg);       
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    
  	CSN = 1;                    
  	return(status);           
}


uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                  
  	status = SPI_RW(reg);      
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);       
  	CSN = 1;                  
  	return(status);          
}



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
  	SPI_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)TX_ADDRESS,TX_ADR_WIDTH); 	  

  	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);        
  	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);  
  	SPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);
  	SPI_RW_Reg(WRITE_REG+RF_CH,40);       
  	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);   
  	SPI_RW_Reg(WRITE_REG+CONFIG,0x0e);    
	CE=1;
}

uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar sta; 
	CE=0;
  	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
 	CE=1;  
	while(IRQ!=0);
	sta=SPI_Read(STATUS);    
	SPI_RW_Reg(WRITE_REG+STATUS,sta); 
	if(sta&MAX_TX)
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


void main()			   
{													  
	uchar shujv[8];		
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
	/*************×Ô¼º¼ÓµÄ***********************/
	while(CNRF_Check())	 //¼ì²énrfÊÇ·ñ´æÔÚ
	{
		led0=0;
		delayms(1000);
		led0=1;
		delayms(1000);								  
	}	
	CTX_Mode();
	/*************×Ô¼º¼ÓµÄ***********************/
	stop=byte_read(0x2000);		//¶ÁÈ¡ÉÏÒ»´Î±£ÁôµÄÖµ
	while(1)
	{
			wenshi(shujv); //Ð´ÈëÇ°4¸öÊý¾Ý
		//	yali(shujv);//Ð´ÈëµÚ5¸öÊý¾Ý
		//	shache(shujv);//Ð´ÈëµÚÁù¸öÊý¾Ý
	Get_weight();
	if (weight_shiwu/1000<=5)
	shujv[4]='0';
	else shujv[4]='1';


	if(stop==0)		 
		shujv[5]='1';
	if(stop==1)		 
		shujv[5]='0';
		if(out==0)	
		shujv[6]='1';
		else	
		shujv[6]='0';

		//////////////////////////////
			if(NRF24L01_TxPacket(shujv)==TX_OK)	
			{
				led1=0;
				delayms(300);
				led1=1;
				delayms(10);
				/////
				flag=1;
			}
			else flag=0;

	   //////////////////////////////
			if(flag==1)
			{
				if(CNRF24L01_TxPacket(shujv)==TX_OK)	
				{
					led3=0;
					delayms(300);
					led3=1;
					delayms(10);
					cejv=0;		
					helpcejv=0;
				}	
				else  
				{
					helpcejv++;
					delayms(100);
					if(helpcejv>2)
					{  			led2=0;
								delayms(300);
								led2=1;
								delayms(10);
						cejv=0;
						helpcejv=0;	
					}		 
				}
			/////////////////////////////////////////////////
							
			
			
			}			
	}
}