extern unsigned char quiet=1;
extern unsigned char baoqi=1;
extern unsigned char zongkaiguan=1;	  //刚开始要关掉声音
#include <reg52.h>
#include <intrins.h>
#include "nrf24l01.h"
#include "12864.h"
#define uchar unsigned char
#define uint unsigned int

const uchar TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
sbit CE=P2^0;
sbit CSN=P2^1;
sbit IRQ=P2^2;
sbit MOSI=P1^1;		  //可以修改
sbit MISO=P2^4;
//sbit SCK=P2^5;	 
sbit SCK=P1^0; // 自己修改的	

uchar RxBuf[32];
uchar datedisplay[9];
/*void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}	*

/**************************************************
函数: init_io()

描述:
    初始化IO
/**************************************************/
void init_io(void)
{
	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低
	IRQ = 1;        // 中断复位
}
/**************************************************/


/**************************************************
函数：SPI_RW()

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
/**************************************************/
uchar SPI_RW(uchar byte)
{
	uchar i;
   	for(i=0; i<8; i++)          // 循环8次
   	{
   		MOSI = (byte & 0x80);   // byte最高位输出到MOSI
   		byte <<= 1;             // 低一位移位到最高位
   		SCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   		byte |= MISO;       	// 读MISO到byte最低位
   		SCK = 0;            	// SCK置低
   	}
    return(byte);           	// 返回读出的一字节
}
/**************************************************/

/**************************************************
函数：SPI_RW_Reg()

描述：
    写数据value到reg寄存器
/**************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
  	CSN = 0;                   // CSN置低，开始传输数据
  	status = SPI_RW(reg);      // 选择寄存器，同时返回状态字
  	SPI_RW(value);             // 然后写数据到该寄存器
  	CSN = 1;                   // CSN拉高，结束数据传输
  	return(status);            // 返回状态寄存器
}
/**************************************************/


/**************************************************
函数：SPI_Read()

描述：
    从reg寄存器读一字节
/**************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
  	CSN = 0;                    // CSN置低，开始传输数据
  	SPI_RW(reg);                // 选择寄存器
  	reg_val = SPI_RW(0XFF);        // 然后从该寄存器读数据		  0
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(reg_val);            // 返回寄存器数据
}
/**************************************************/

/**************************************************
函数：SPI_Read_Buf()

描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
/**************************************************/
uchar SPI_Read_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    // 逐个字节从nRF24L01读出
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Write_Buf()

描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
/**************************************************/
uchar SPI_Write_Buf(uchar reg, uchar * pBuf, uchar bytes)
{
	uchar status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);        // 逐个字节写入nRF24L01
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

//检测24L01是否存在
//返回值:0，成功;1，失败	
uchar NRF_Check(void)
{
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uchar i;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	SPI_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(void)
{
	CE=0;	  
  	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
  	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  	SPI_RW_Reg(WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  	SPI_RW_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	SPI_RW_Reg(WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	CE = 1; //CE为高,进入接收模式 
}	
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uchar NRF24L01_RxPacket(uchar *rxbuf)
{
	uchar sta;		    							   
	sta=SPI_Read(STATUS);  //读取状态寄存器的值    	 
	SPI_RW_Reg(WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		SPI_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		SPI_RW_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}
void main()
{
	uchar i; //自己加的
//	uchar datedisplay[7];
	init_io();
	P0=0x00;//屏蔽数码管
	 delay(10);                 //延时
    wela=0;
	dula=0; 
//	inte();
 lcd_init2();	
	/******************自己加的**************************/
	while(NRF_Check());	

	RX_Mode();//	
	interface();
	while(NRF24L01_RxPacket(RxBuf)!=0);
	while(1)
	{
		if(NRF24L01_RxPacket(RxBuf)==0)		
		{
			for(i=0;i<8;i++)
			datedisplay[i]=RxBuf[i];	
		}

	checkbutton();
	lcd12864(datedisplay);

	}		
}