#ifndef __LCEDISPLAYHEADFILE__
#define __LCEDISPLAYHEADFILE__
sbit smgdl=P2^6;
sbit smgwl=P2^7;
sbit rs=P3^5;
sbit rw=P3^6;
sbit lcden=P3^4;
//******************************************************************��ʱ����******************************************************************************************************************************��ʱ����

void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=123;y>0;y--);
}

//***********************************Һ������ʾ**********************************************************************************
void write_com(unsigned char com)
{
	rs=0;
    P0=com;
    lcden=1;
	delay(1);
    lcden=0;
	delay(1);
}
void write_date(unsigned char date)
{
	rs=1;
    P0=date;
    lcden=1;
	delay(1);
    lcden=0;
	delay(1);
}
void write_word(unsigned char *s)
{
	while(*s>0)
	{
		write_date(*s);
		s++;
	}
}	  
void inte()
{
	  lcden=0;
	  rw=0;
	  delay(15);//��ʱ15MS
	  write_com(0x38);//������ʾģʽ
	  delay(15);
	  write_com(0x0c);//����ʾ���Թ�꣬��겻��
	  delay(5);
	  write_com(0x01);//����
	  delay(5);
	  write_com(0x06);//��ַ�Զ���һ ����Զ���һ
	  delay(5);
}
/*void main()
{  
	smgdl=1;
	P0=0x00;//���������
	smgdl=0;
	smgwl=0;
	inte();	  
	while(1)
	{
	write_com(0x80);
	write_word("Temperature: ");
	delay(1);
	write_date(table[0]);
	write_date(table[1]);
	delay(1);
	write_com(0x80+0x40);
	delay(1);
	write_word("Humidity: ");
	write_date(table[2]);
	write_date(table[3]); 
	}
	}  */

void lcddisplay(unsigned char datedisplay[])
{
	write_com(0x80);
	write_word("Temperature: ");
	delay(1);
	write_date(datedisplay[2]);
	write_date(datedisplay[3]);
	delay(1);
	write_com(0x80+0x40);
	delay(1);
	write_word("H:");
	write_date(datedisplay[0]);
	write_date(datedisplay[1]);	
	if(datedisplay[4]=='1')write_word(" YES");
	else write_word(" NO ");
	if(datedisplay[5]=='0')write_word(" STO");
	else write_word(" STA");
}

#endif