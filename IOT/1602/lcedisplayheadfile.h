#ifndef __LCEDISPLAYHEADFILE__
#define __LCEDISPLAYHEADFILE__
sbit wela    =  P2^6;
sbit dula    =  P2^7;
sbit rs=P3^5;
sbit rw=P3^6;
sbit lcden=P3^4;
sbit FM=P2^3;	 //�ӵ�
sbit key=P1^4;
/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=123;y>0;y--);
}

//����������
void voice()
{	FM=0;
	delay(30);
		FM=1;
	delay(50);
		FM=0;
	delay(30);
		FM=1;
	delay(20);
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
//******************************************************************��ʱ����******************************************************************************************************************************��ʱ����



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
	write_word("T:");		//�¶�
	delay(1);
	write_date(datedisplay[2]);
	write_date(datedisplay[3]);
	delay(1);
	if(zongkaiguan==0)write_word("  OFF");
	 if(zongkaiguan==1)write_word("  ON ");	   //����������ʾ
	if(datedisplay[6]=='1')
	{
		write_word(" NOISY");
		if((quiet==1)&&(zongkaiguan==1))
		{
			 voice();
			 quiet=0;
		}
	}
	if(datedisplay[6]=='0')
	{
		write_word(" QUIET");
		quiet=1; 
	} //�����볳��
		delay(1);
	write_com(0x80+0x40);
	delay(1);
	write_word("H:");
	write_date(datedisplay[0]);
	write_date(datedisplay[1]);	
	if(datedisplay[4]=='1')
	{
		write_word("  YES");
		baoqi=0;	
	}//�ڳ���
	if(datedisplay[4]=='0')
	{
		write_word("  NO ");
		if((baoqi==0)&&(zongkaiguan==1))
		{
			 	voice();
				baoqi=1;
		}  
	} //����  ������;
	if(datedisplay[5]=='0')write_word(" STOP   ");
	else write_word(" START  ");
}
void interface()
{
	write_com(0x80);
	write_word("    TURN ON");
	write_com(0x80+0x40);
	write_word("STROLLER'S POWER");	
}
#endif