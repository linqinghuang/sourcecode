
#ifndef __kaisuo_H
#define __kaisuo_H



unsigned char code FFW[8]={0x1f,0x3f,0x2f,0x6f,0x4f,0xcf,0x8f,0x9f};
unsigned char code REV[8]={0x9f,0x8f,0xcf,0x4f,0x6f,0x2f,0x3f,0x1f};
unsigned char rate ;    
sbit k4=P3^7;	 
/********************************************************/
/*                                                  
/* ÑÓÊ±
/* 11.0592MHzÊ±ÖÓ£¬                                    
/*                                                      
/********************************************************/
void delay_ms(unsigned int i)
{
	unsigned int x,y;
	for(x=i;x>0;x--)
		for(y=110;y>0;y--);
}
void delayus(unsigned int us)
{
	unsigned int i,j;
	for (j=0;j<us;j++)
		for (i=0;i<12;i++);
}

void kai()
{
unsigned char i,j;
for(j=130;j>0;j--)
	{
	for (i=0;i<8;i++)
	{
	P1=FFW[i];
	delayus(14); 
	}		
	}	  	
}
void guan()
{	unsigned char i,j;
	for(j=130;j>0;j--)
	{
		for (i=0;i<8;i++)
		{
 		P1=REV[i];
		delayus(14); 
		}		
	}	   		
}
unsigned char shache(unsigned char cejv,unsigned char stop)
{	

	if((cejv==0)&&(stop==0))	
			  
		{	delay_ms(150);
			guan();		  
			return 1;
		}
		 return 0;
}
unsigned char checkbutton(unsigned char cejv,unsigned char stop)
{
	if((cejv==1)&&(stop==1))
	
		{
			if(k4==0)
			{
				delay_ms(50);
				if(k4==0)
				{
					delay_ms(150);
					kai();
					return 0;
				}
				else return 1;
			
		}
		 else return 1;
	}
	 else return 1;
}
 #endif
