#include<reg51.h>
#include<intrins.h>
#include<stdlib.h>
#define uchar unsigned char
#define uint unsigned int

/*????*/
#define LCD_data P1
sbit LCD_RS=P1^0;//��������
sbit LCD_RW=P1^1;
sbit LCD_EN=P1^2;
sbit LCD_PSB =P3^6;	
sbit wela = P2^6;
sbit dula = P2^7;
uchar dis1[10];
uchar code dis2[]={"www.txmcu.com"};
uchar code dis3[]={"wwwddds"};
uchar code dis4[]={"............."};
void delay_1ms(uint x)
{
	uint i,j;
	for(j=0;j<x;j++)
		for(i=0;i<110;i++);
}
/*??????LCD*/
/*RS=L,RW=L,E=???,DO-D7=???*/
void write_cmd(uchar cmd)
{
	LCD_RS =1;
	LCD_RW =0;
	LCD_EN=0;
	P0=cmd;
	delay_1ms(5);
	LCD_EN=1;
	delay_1ms(5);
	LCD_EN=0;
}
/*??????LCD*/
/*RS=L,RW=L,E=???,DO-D7=??*/
void write_dat(uchar dat)
{
	LCD_RS =1;
	LCD_RW =0;
	LCD_EN=0;
	P1=dat;
	delay_1ms(5);
	LCD_EN=1;
	delay_1ms(5);
	LCD_EN=0;
}


/*??????*/

void lcd_pos(uchar X,uchar Y)
{
	uchar pos;
	if(X==0)
	{
		X=0x80;
	}
	else if(X==1)
	{
		X=0x90;
	}
	else if(X==2)
	{
		X=0x88;
	}
	else if(X==3)
	{
		X=0x98;
	}
	pos=X+Y;
	write_cmd(pos);//????	
}

void makerand()
{
	uint ran;
	ran=rand();
	dis1[0]=ran/10000+0x30;
	dis1[1]=ran%10000/1000+0x30;
	dis1[2]=ran%1000/100+0x30;
	dis1[3]=ran%100/10+0x30;
	dis1[4]=ran%10+0x30;
	ran=rand();
	dis1[5]=ran%10000+0x30;
	dis1[6]=ran%10000/1000+0x30;
	dis1[7]=ran%1000/100+0x30;
	dis1[8]=ran%100/10+0x30;
	dis1[9]=ran%10+0x30;
}

/*LCD?????*/
void lcd_init()
{
	LCD_PSB=1;
	write_cmd(0x30);
	delay_1ms(5);
	write_cmd(0x0C);
	delay_1ms(5);
	write_cmd(0x01);
	delay_1ms(5);
}

/*???*/
main()
{
	uchar i;
	wela=0;
	dula=0;
	delay_1ms(10);
	lcd_init();
	lcd_pos(1,0);
	i=0;
	while(dis2[i]!='\0')
	{
		write_dat(dis2[i]);
		i++;
	}
	lcd_pos(2,0);
	i=0;
	while(dis3[i]!='\0')
	{
		write_dat(dis3[i]);
		i++;
	}
	lcd_pos(3,0);
	i=0;
	while(dis4[i]!='\0')
	{
		write_dat(dis4[i]);
		i++;
	}
	while(1)
	{
		lcd_pos(0,0);
		makerand();
		for(i=0;i<10;i++)
		{
			write_dat(dis1[i]);
		}
	}
}