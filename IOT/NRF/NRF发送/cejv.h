#ifndef __cejv_H
#define __cejv_H
const unsigned char TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const unsigned char RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
sbit CCE=P0^0;
sbit CCSN=P0^1;
sbit CIRQ=P0^2;
sbit CMOSI=P0^6;
sbit CMISO=P0^4;
sbit CSCK=P0^7;

/**************************************************
����: init_io()

����:
    ��ʼ��IO
/**************************************************/
void Cinit_io(void)
{
	CCE  = 0;        // ����
	CCSN = 1;        // SPI��ֹ
	CSCK = 0;        // SPIʱ���õ�
	CIRQ = 1;        // �жϸ�λ
}
/**************************************************/


/**************************************************
������SPI_RW()

������
    ����SPIЭ�飬дһ�ֽ����ݵ�nRF24L01��ͬʱ��nRF24L01
	����һ�ֽ�
/**************************************************/
unsigned char CSPI_RW(unsigned char byte)
{
	unsigned i;
   	for(i=0; i<8; i++)          // ѭ��8��
   	{
   		CMOSI = (byte & 0x80);   // byte���λ�����MOSI
   		byte <<= 1;             // ��һλ��λ�����λ
   		CSCK = 1;                // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
   		byte |= CMISO;       	// ��MISO��byte���λ
   		CSCK = 0;            	// SCK�õ�
   	}
    return(byte);           	// ���ض�����һ�ֽ�
}
/**************************************************/

/**************************************************
������SPI_RW_Reg()

������
    д����value��reg�Ĵ���
/**************************************************/
unsigned char CSPI_RW_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
  	CCSN = 0;                   // CSN�õͣ���ʼ��������
  	status = CSPI_RW(reg);      // ѡ��Ĵ�����ͬʱ����״̬��
  	CSPI_RW(value);             // Ȼ��д���ݵ��üĴ���
  	CCSN = 1;                   // CSN���ߣ��������ݴ���
  	return(status);            // ����״̬�Ĵ���
}
/**************************************************/


/**************************************************
������SPI_Read()

������
    ��reg�Ĵ�����һ�ֽ�
/**************************************************/
unsigned char CSPI_Read(unsigned char reg)
{
	unsigned char reg_val;
  	CCSN = 0;                    // CSN�õͣ���ʼ��������
  	CSPI_RW(reg);                // ѡ��Ĵ���
  	reg_val=CSPI_RW(0);        // Ȼ��ӸüĴ���������		  0
  	CCSN = 1;                    // CSN���ߣ��������ݴ���
  	return(reg_val);            // ���ؼĴ�������
}
/**************************************************/

/**************************************************
������SPI_Read_Buf()

������
    ��reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ��
	���ݻ����/���͵�ַ
/**************************************************/
unsigned char CSPI_Read_Buf(unsigned char reg, unsigned char * pBuf, unsigned char bytes)
{
	unsigned char status, i;
  	CCSN = 0;                    // CSN�õͣ���ʼ��������
  	status = CSPI_RW(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i=0; i<bytes; i++)
    	pBuf[i] = CSPI_RW(0);    // ����ֽڴ�nRF24L01����
  	CCSN = 1;                    // CSN���ߣ��������ݴ���
  	return(status);             // ����״̬�Ĵ���
}
/**************************************************/

/**************************************************
������SPI_Write_Buf()

������
    ��pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢
	��ͨ�����ݻ����/���͵�ַ
/**************************************************/
unsigned char CSPI_Write_Buf(unsigned char reg, unsigned char * pBuf, unsigned char bytes)
{
	unsigned char status, i;
  	CCSN = 0;                    // CSN�õͣ���ʼ��������
  	status = CSPI_RW(reg);       // ѡ��Ĵ�����ͬʱ����״̬��
  	for(i=0; i<bytes; i++)
    	CSPI_RW(pBuf[i]);        // ����ֽ�д��nRF24L01
  	CCSN = 1;                    // CSN���ߣ��������ݴ���
  	return(status);             // ����״̬�Ĵ���
}
/**************************************************/

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
unsigned char CNRF_Check(void)
{
	unsigned char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	unsigned char i;
	CSPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	CSPI_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}

void CTX_Mode(void)
{														 
	CCE=0;	    
  	CSPI_Write_Buf(WRITE_REG+TX_ADDR,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
    CSPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(unsigned char*)TX_ADDRESS,TX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	CSPI_RW_Reg(WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	CSPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	CSPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	CSPI_RW_Reg(WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  	CSPI_RW_Reg(WRITE_REG+RF_SETUP,0x09);  //����TX�������,0db����,2Mbps,���������濪��   
  	CSPI_RW_Reg(WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CCE=1;//CEΪ��,10us����������
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
unsigned char CNRF24L01_TxPacket(unsigned char *txbuf)
{
	unsigned char sta; 
	CCE=0;
  	CSPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	CCE=1;//��������	   
	while(CIRQ!=0);//�ȴ��������
	sta=CSPI_Read(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	CSPI_RW_Reg(WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		CSPI_RW_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	else return 0xff;//����ԭ����ʧ��
}
#endif