#ifndef __LCD__
#define __LCD__
/*TS12864A-3 �˿ڶ���*/
#define LCD_data  P0             //���ݿ�
sbit LCD_RS  =  P3^5;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P3^6;            //Һ����/д����
sbit LCD_EN  =  P3^4;            //Һ��ʹ�ܿ���
sbit LCD_PSB =  P3^7;            //��/����ʽ����
sbit wela    =  P2^6;
sbit dula    =  P2^7;
sbit FM=P2^3;	 //�ӵ�
sbit key=P1^4;
unsigned char code dis0[] = {"�ڳ���"};
unsigned char code dis1[] = {"������"};
unsigned char code dis2[] = {"WWW.YUNLONGDZ.CN"};
unsigned char code dis3[] = {"�¶ȣ�"};
unsigned char code dis4[] = {"ʪ�ȣ�"};
unsigned char code dis5[] = {"������"};
unsigned char code dis6[] = {"���ѿ�"};
unsigned char code dis7[] = {"���϶�"};
unsigned char code dis8[] = {"%"};
unsigned char code dis9[] = {"  ����  "};
unsigned char code dis10[] = {"  ����  "};
unsigned char code dis11[] = {"  ������"};
unsigned char code dis12[] = {"  ������"};
unsigned char code disface1[] = {"    ��ӭʹ��"};
unsigned char code disface2[] = {"  �뿪���ϵ�Դ  "};
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
unsigned char IRDIS[2];
unsigned char IRCOM[4];
void delay0(unsigned char x);  //x*0.14MS
void beep();
void  dataconv();
void lcd_pos(unsigned char X,unsigned char Y);  //ȷ����ʾλ��
/*******************************************************************/
/*                                                                 */
/*  ��ʱ����                                                       */
/*                                                                 */
/*******************************************************************/
void delay(int ms)
{
    while(ms--)
	{
      unsigned char i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}		
/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/
//����������
void voice()
{	FM=0;
	delay(20);
		FM=1;
	delay(10);
/*		FM=0;
	delay(50);
		FM=1;
	delay(50);


	FM=0;
	delay(50);
		FM=1;
	delay(10);
		FM=0;
	delay(50);
		FM=1;
	delay(2);*/
	}
void checkbutton()
{
	if(key==0)
	{	delay(500);
		if(key==0)
		//zongkaiguan=~zongkaiguan;
		zongkaiguan=((zongkaiguan+1)%2);
	}
}
bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }
/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(unsigned char cmd)
{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(unsigned char dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()		  //û����������������ÿ��ȫ�����ϴεĶ���
{ 

    LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ�����
    delay(5);
    lcd_wcmd(0x30);      //����ָ�����
    delay(5);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(5);
    //lcd_wcmd(0x01);      //���LCD����ʾ����
    //delay(5);
}
void lcd_init2()		//�������������ĳ�ʼ��
{ 

    LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ�����
    delay(5);
    lcd_wcmd(0x30);      //����ָ�����
    delay(5);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(5);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(5);
}

/*********************************************************/
/*														 */
/* ��ʱx*0.14ms�ӳ���									 */
/*                                                       */
/*********************************************************/

void delay0(unsigned char x)    //x*0.14MS
{
  unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}
/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(unsigned char X,unsigned char Y)
{                          
   unsigned char  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     //��ʾ��ַ
}
 /*********************************************************/
/*														 */
/* void lcd12864()       									 */
/*                                                       */
/*********************************************************/
void lcd12864(unsigned char datedisplay[])
 {
    unsigned char i;
  	  
    lcd_init();                //��ʼ��LCD             
  
    lcd_pos(0,0); 
	          //������ʾλ��Ϊ��һ�еĵ�1���ַ�
    if(datedisplay[4]=='1')
	
	{
		while(dis0[i] != '\0')
	     {
	       lcd_wdat(dis0[i]);      //��ʾ�ַ� "�ڳ���"
	       i++;					   // baoqi=0��
	     } 
		 baoqi=0;
	 }  
	if(datedisplay[4]=='0') 
	{	while(dis1[i] != '\0')
	     {
	       lcd_wdat(dis1[i]);      //��ʾ�ַ� ������
	       i++;
	     }  
		 if((baoqi==0)&&(zongkaiguan==1))
		 {
		 	voice();
			baoqi=1;
		 }   //����  ������
	 } 	// if(baoqi==1&&zongkaiguan)	{voice();baoqi=1;}	   ��ʼ��baoqi=1��Ĭ���������ѿ�  !!!!!!!!!!!!!!!!!!!����Ҫ����Ϊȫ�ֱ���
	 i=0;
	 if(zongkaiguan==0)		//������
	 {
		 	while(dis12[i] != '\0')
	     {
	       lcd_wdat(dis12[i]);      
	       i++;
	     } 
	 }
	 i=0;
	 if(zongkaiguan==1)	 //������
	 {
		 	while(dis11[i] != '\0')
	     {
	       lcd_wdat(dis11[i]);      
	       i++;
	     } 
	 }
    lcd_pos(1,0);             //������ʾλ��Ϊ�ڶ��еĵ�1���ַ�
     i = 0;								 //��ʾ�ַ� ������
	 if(datedisplay[5]=='0')
	 {
    while(dis5[i] != '\0')
     {
       lcd_wdat(dis5[i]);      
       i++;
     } 	 }
	 i = 0;
	 if(datedisplay[5]=='1') 	  //��ʾ�ַ�  ���ѿ�
	 {
	  while(dis6[i] != '\0')
     {
       lcd_wdat(dis6[i]);      
       i++;
     } 	}
	 i=0;
	 if(datedisplay[6]=='1') 	     //��ʾ�ַ�  ����
	 {
		  while(dis10[i] != '\0')
	     {
	       lcd_wdat(dis10[i]);    
	       i++;
	     } 	
		 if((quiet==1)&&(zongkaiguan==1))
		 {
		 voice();
		 quiet=0;
		 }
	 }//if(quiet==1&&zongkaiguan)	{voice();quite=0;}	   ��ʼ��quiet=1��Ĭ���������ѿ�	!!!!!!!!!!!!!!!!!!!!!!!!
	 i=0;
	 if(datedisplay[6]=='0') 	   //��ʾ�ַ�  ����
	 {								
	  while(dis9[i] != '\0')
     {
       lcd_wdat(dis9[i]);      
       i++;			   //���Ӧ������  quiet=1�� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
     } 
	 quiet=1;
	 	}
 	 lcd_pos(2,0);             //������ʾλ��Ϊ�����еĵ�1���ַ�  �¶�
    
	 
	i = 0; 
	    while(dis3[i] != '\0')
	     {
	       lcd_wdat(dis3[i]);      //��ʾ�ַ�
	       i++;
	     }
		 lcd_wdat(datedisplay[2]);
		 lcd_wdat(datedisplay[3]);
		 i = 0;
	    while(dis7[i] != '\0')
	     {
	       lcd_wdat(dis7[i]);      //��ʾ�ַ�
	       i++;
	     }
	
	 lcd_pos(3,0);             //������ʾλ��Ϊ�����еĵ�1���ַ�   ʪ��
    i = 0;  
	
	
		
	    while(dis4[i] != '\0')
	     {
	       lcd_wdat(dis4[i]);      //��ʾ�ַ�
	       i++;
	     }
		 lcd_wdat(datedisplay[0]);
		 lcd_wdat(datedisplay[1]);
		 lcd_wdat(dis8[0]);
		 
}
void interface()
{	 unsigned char i;
	lcd_pos(1,0);             //������ʾλ��Ϊ�ڶ��еĵ�1���ַ�
     i = 0;								 //��ʾ�ַ� ������
    while(disface1[i] != '\0')
     {
       lcd_wdat(disface1[i]);      
       i++;
     }
	 lcd_pos(2,0);             //������ʾλ��Ϊ�ڶ��еĵ�1���ַ�
     i = 0;								 //��ʾ�ַ� ������
    while(disface2[i] != '\0')
     {
       lcd_wdat(disface2[i]);      
       i++;
     }
}
#endif
