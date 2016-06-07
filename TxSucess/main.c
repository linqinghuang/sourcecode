#include <reg52.h>
#include "nrf24l01.h"
//#include "wsjiance.h"
//#include "cejv.h"
//#include "yalijiance.h"
//#include "kaisuo.h"
//#include "eerom.h"
#define uchar unsigned char
#define uint unsigned int
#define RI 1

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
//const uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
sbit CE=P2^0;
sbit CSN=P2^1;
sbit IRQ=P2^2;
sbit MOSI=P3^1;
sbit MISO=P2^4;

sbit SCK=P3^0;		  
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;	  //  Ҫɾȥ��Ȼ
sbit ledk=P2^5;		 //�Լ��޸ĵĶ�
sbit out=P3^6;
void delayms(uint i)
{
	uint x,y;
	for(x=i;x>0;x--)
		for(y=110;y>0;y--);
}
/**************************************************
����: init_io()

����:
    ��ʼ��IO
/**************************************************/
void init_io(void)
{
	CE  = 0;        // ����
	CSN = 1;        // SPI��ֹ
	SCK = 0;        // SPIʱ���õ�
	IRQ = 1;        // �жϸ�λ
}
/**************************************************/


/**************************************************
������SPI_RW()

������
    ����SPIЭ�飬дһ�ֽ����ݵ�nRF24L01��ͬʱ��nRF24L01
	����һ�ֽ�
/**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar i;
   	for(i=0; i<8; i++)          // ѭ��8��
   	{
   		MOSI = (byte & 0x80);   // byte���λ�����MOSI
   		byte <<= 1;             // ��һλ��λ�����λ
   		SCK = 1;                // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
   		byte |= MISO;       	// ��MISO��byte���λ
   		SCK = 0;            	// SCK�õ�
   	}
    return(byte);           	// ���ض�����һ�ֽ�
}
/**************************************************/

/**************************************************
������SPI_RW_Reg()

������
    д����value��reg�Ĵ���
/**************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
  	CSN = 0;                   // CSN�õͣ���ʼ��������
  	status = SPI_RW(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	SPI_RW(value);             // Ȼ��д���ݵ��üĴ���
  	CSN = 1;                   // CSN���ߣ��������ݴ���
  	return(status);            // ����״̬�Ĵ���
}
/**************************************************/


/**************************************************
������SPI_Read()

������
    ��reg�Ĵ�����һ�ֽ�
/**************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
  	CSN = 0;                    // CSN�õͣ���ʼ��������
  	SPI_RW(reg);                // ѡ��Ĵ���
  	reg_val = SPI_RW(0);        // Ȼ��ӸüĴ���������		  0
  	CSN = 1;                    // CSN���ߣ��������ݴ���
  	return(reg_val);            // ���ؼĴ�������
}
/**************************************************/

/**************************************************
������SPI_Read_Buf()

������
    ��reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ��
	���ݻ����/���͵�ַ
/**************************************************/
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN�õͣ���ʼ��������
  	status = SPI_RW(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    // ����ֽڴ�nRF24L01����
  	CSN = 1;                    // CSN���ߣ��������ݴ���
  	return(status);             // ����״̬�Ĵ���
}
/**************************************************/

/**************************************************
������SPI_Write_Buf()

������
    ��pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢
	��ͨ�����ݻ����/���͵�ַ
/**************************************************/
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN�õͣ���ʼ��������
  	status = SPI_RW(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);        // ����ֽ�д��nRF24L01
  	CSN = 1;                    // CSN���ߣ��������ݴ���
  	return(status);             // ����״̬�Ĵ���
}
/**************************************************/

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
uchar NRF_Check(void)
{
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uchar i;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	SPI_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}

void TX_Mode(void)
{														 
	CE=0;	    
  	SPI_Write_Buf(WRITE_REG+TX_ADDR,(uchar *)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
    SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar *)TX_ADDRESS,TX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	SPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	SPI_RW_Reg(WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	SPI_RW_Reg(WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CE=1;//CEΪ��,10us����������
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uchar NRF24L01_TxPacket(uchar *txbuf)
{
	uchar sta; 
	CE=0;
  	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	CE=1;//��������	   
	while(IRQ!=0);//�ȴ��������
	sta=SPI_Read(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	SPI_RW_Reg(WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
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
	uchar shujv[]="111111111";
	uchar stop=0;
	uchar cejv=1; 
	uchar flag=0;
	uchar helpcejv=0;	 
	init_io();	
	TX_Mode();		   //����Ϊ����ģʽ
	/*************�Լ��ӵ�***********************/
//	stop=byte_read(0x2000);		//��ȡ��һ�α�����ֵ
		while(1)
		{
			NRF24L01_TxPacket(shujv);
		}
}