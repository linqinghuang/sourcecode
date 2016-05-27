#ifndef __LCD__
#define __LCD__
/*TS12864A-3 端口定义*/
#define LCD_data  P0             //数据口
sbit LCD_RS  =  P3^5;            //寄存器选择输入 
sbit LCD_RW  =  P3^6;            //液晶读/写控制
sbit LCD_EN  =  P3^4;            //液晶使能控制
sbit LCD_PSB =  P3^7;            //串/并方式控制
sbit wela    =  P2^6;
sbit dula    =  P2^7;
sbit FM=P2^3;	 //加的
sbit key=P1^4;
unsigned char code dis0[] = {"在车内"};
unsigned char code dis1[] = {"被抱起"};
unsigned char code dis2[] = {"WWW.YUNLONGDZ.CN"};
unsigned char code dis3[] = {"温度："};
unsigned char code dis4[] = {"湿度："};
unsigned char code dis5[] = {"车已锁"};
unsigned char code dis6[] = {"车已开"};
unsigned char code dis7[] = {"摄氏度"};
unsigned char code dis8[] = {"%"};
unsigned char code dis9[] = {"  安静  "};
unsigned char code dis10[] = {"  吵闹  "};
unsigned char code dis11[] = {"  声音开"};
unsigned char code dis12[] = {"  声音关"};
unsigned char code disface1[] = {"    欢迎使用"};
unsigned char code disface2[] = {"  请开车上电源  "};
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
unsigned char IRDIS[2];
unsigned char IRCOM[4];
void delay0(unsigned char x);  //x*0.14MS
void beep();
void  dataconv();
void lcd_pos(unsigned char X,unsigned char Y);  //确定显示位置
/*******************************************************************/
/*                                                                 */
/*  延时函数                                                       */
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
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
/*                                                                 */
/*******************************************************************/
//蜂鸣器发声
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
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
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
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
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
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()		  //没有清屏操作，所以每次全覆盖上次的东西
{ 

    LCD_PSB = 1;         //并口方式
    
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(5);
    //lcd_wcmd(0x01);      //清除LCD的显示内容
    //delay(5);
}
void lcd_init2()		//带有清屏操作的初始化
{ 

    LCD_PSB = 1;         //并口方式
    
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}

/*********************************************************/
/*														 */
/* 延时x*0.14ms子程序									 */
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
/* 设定显示位置                                          */
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
   lcd_wcmd(pos);     //显示地址
}
 /*********************************************************/
/*														 */
/* void lcd12864()       									 */
/*                                                       */
/*********************************************************/
void lcd12864(unsigned char datedisplay[])
 {
    unsigned char i;
  	  
    lcd_init();                //初始化LCD             
  
    lcd_pos(0,0); 
	          //设置显示位置为第一行的第1个字符
    if(datedisplay[4]=='1')
	
	{
		while(dis0[i] != '\0')
	     {
	       lcd_wdat(dis0[i]);      //显示字符 "在车内"
	       i++;					   // baoqi=0；
	     } 
		 baoqi=0;
	 }  
	if(datedisplay[4]=='0') 
	{	while(dis1[i] != '\0')
	     {
	       lcd_wdat(dis1[i]);      //显示字符 被抱起
	       i++;
	     }  
		 if((baoqi==0)&&(zongkaiguan==1))
		 {
		 	voice();
			baoqi=1;
		 }   //抱起  开声音
	 } 	// if(baoqi==1&&zongkaiguan)	{voice();baoqi=1;}	   初始化baoqi=1；默认声音提醒开  !!!!!!!!!!!!!!!!!!!可能要定义为全局变量
	 i=0;
	 if(zongkaiguan==0)		//声音开
	 {
		 	while(dis12[i] != '\0')
	     {
	       lcd_wdat(dis12[i]);      
	       i++;
	     } 
	 }
	 i=0;
	 if(zongkaiguan==1)	 //声音关
	 {
		 	while(dis11[i] != '\0')
	     {
	       lcd_wdat(dis11[i]);      
	       i++;
	     } 
	 }
    lcd_pos(1,0);             //设置显示位置为第二行的第1个字符
     i = 0;								 //显示字符 车已锁
	 if(datedisplay[5]=='0')
	 {
    while(dis5[i] != '\0')
     {
       lcd_wdat(dis5[i]);      
       i++;
     } 	 }
	 i = 0;
	 if(datedisplay[5]=='1') 	  //显示字符  车已开
	 {
	  while(dis6[i] != '\0')
     {
       lcd_wdat(dis6[i]);      
       i++;
     } 	}
	 i=0;
	 if(datedisplay[6]=='1') 	     //显示字符  吵闹
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
	 }//if(quiet==1&&zongkaiguan)	{voice();quite=0;}	   初始化quiet=1；默认声音提醒开	!!!!!!!!!!!!!!!!!!!!!!!!
	 i=0;
	 if(datedisplay[6]=='0') 	   //显示字符  安静
	 {								
	  while(dis9[i] != '\0')
     {
       lcd_wdat(dis9[i]);      
       i++;			   //其后应该增加  quiet=1； !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
     } 
	 quiet=1;
	 	}
 	 lcd_pos(2,0);             //设置显示位置为第三行的第1个字符  温度
    
	 
	i = 0; 
	    while(dis3[i] != '\0')
	     {
	       lcd_wdat(dis3[i]);      //显示字符
	       i++;
	     }
		 lcd_wdat(datedisplay[2]);
		 lcd_wdat(datedisplay[3]);
		 i = 0;
	    while(dis7[i] != '\0')
	     {
	       lcd_wdat(dis7[i]);      //显示字符
	       i++;
	     }
	
	 lcd_pos(3,0);             //设置显示位置为第四行的第1个字符   湿度
    i = 0;  
	
	
		
	    while(dis4[i] != '\0')
	     {
	       lcd_wdat(dis4[i]);      //显示字符
	       i++;
	     }
		 lcd_wdat(datedisplay[0]);
		 lcd_wdat(datedisplay[1]);
		 lcd_wdat(dis8[0]);
		 
}
void interface()
{	 unsigned char i;
	lcd_pos(1,0);             //设置显示位置为第二行的第1个字符
     i = 0;								 //显示字符 车已锁
    while(disface1[i] != '\0')
     {
       lcd_wdat(disface1[i]);      
       i++;
     }
	 lcd_pos(2,0);             //设置显示位置为第二行的第1个字符
     i = 0;								 //显示字符 车已锁
    while(disface2[i] != '\0')
     {
       lcd_wdat(disface2[i]);      
       i++;
     }
}
#endif
