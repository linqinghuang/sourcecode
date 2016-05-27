#ifndef __cejv_H
#define __cejv_H
const unsigned char TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const unsigned char RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
sbit CCE=P0^0;
sbit CCSN=P0^1;
sbit CIRQ=P0^2;
sbit CMOSI=P0^6;
sbit CMISO=P0^4;
sbit CSCK=P0^7;

/**************************************************
函数: init_io()

描述:
    初始化IO
/**************************************************/
void Cinit_io(void)
{
	CCE  = 0;        // 待机
	CCSN = 1;        // SPI禁止
	CSCK = 0;        // SPI时钟置低
	CIRQ = 1;        // 中断复位
}
/**************************************************/


/**************************************************
函数：SPI_RW()

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
/**************************************************/
unsigned char CSPI_RW(unsigned char byte)
{
	unsigned i;
   	for(i=0; i<8; i++)          // 循环8次
   	{
   		CMOSI = (byte & 0x80);   // byte最高位输出到MOSI
   		byte <<= 1;             // 低一位移位到最高位
   		CSCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   		byte |= CMISO;       	// 读MISO到byte最低位
   		CSCK = 0;            	// SCK置低
   	}
    return(byte);           	// 返回读出的一字节
}
/**************************************************/

/**************************************************
函数：SPI_RW_Reg()

描述：
    写数据value到reg寄存器
/**************************************************/
unsigned char CSPI_RW_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
  	CCSN = 0;                   // CSN置低，开始传输数据
  	status = CSPI_RW(reg);      // 选择寄存器，同时返回状态字
  	CSPI_RW(value);             // 然后写数据到该寄存器
  	CCSN = 1;                   // CSN拉高，结束数据传输
  	return(status);            // 返回状态寄存器
}
/**************************************************/


/**************************************************
函数：SPI_Read()

描述：
    从reg寄存器读一字节
/**************************************************/
unsigned char CSPI_Read(unsigned char reg)
{
	unsigned char reg_val;
  	CCSN = 0;                    // CSN置低，开始传输数据
  	CSPI_RW(reg);                // 选择寄存器
  	reg_val=CSPI_RW(0);        // 然后从该寄存器读数据		  0
  	CCSN = 1;                    // CSN拉高，结束数据传输
  	return(reg_val);            // 返回寄存器数据
}
/**************************************************/

/**************************************************
函数：SPI_Read_Buf()

描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
/**************************************************/
unsigned char CSPI_Read_Buf(unsigned char reg, unsigned char * pBuf, unsigned char bytes)
{
	unsigned char status, i;
  	CCSN = 0;                    // CSN置低，开始传输数据
  	status = CSPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	pBuf[i] = CSPI_RW(0);    // 逐个字节从nRF24L01读出
  	CCSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Write_Buf()

描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
/**************************************************/
unsigned char CSPI_Write_Buf(unsigned char reg, unsigned char * pBuf, unsigned char bytes)
{
	unsigned char status, i;
  	CCSN = 0;                    // CSN置低，开始传输数据
  	status = CSPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	CSPI_RW(pBuf[i]);        // 逐个字节写入nRF24L01
  	CCSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

//检测24L01是否存在
//返回值:0，成功;1，失败	
unsigned char CNRF_Check(void)
{
	unsigned char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	unsigned char i;
	CSPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	CSPI_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}

void CTX_Mode(void)
{														 
	CCE=0;	    
  	CSPI_Write_Buf(WRITE_REG+TX_ADDR,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
    CSPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	CSPI_RW_Reg(WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	CSPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	CSPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	CSPI_RW_Reg(WRITE_REG+RF_CH,40);       //设置RF通道为40
  	CSPI_RW_Reg(WRITE_REG+RF_SETUP,0x09);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	CSPI_RW_Reg(WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	CCE=1;//CE为高,10us后启动发送
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
unsigned char CNRF24L01_TxPacket(unsigned char *txbuf)
{
	unsigned char sta; 
	CCE=0;
  	CSPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	CCE=1;//启动发送	   
	while(CIRQ!=0);//等待发送完成
	sta=CSPI_Read(STATUS);  //读取状态寄存器的值	   
	CSPI_RW_Reg(WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		CSPI_RW_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	else return 0xff;//其他原因发送失败
}
#endif