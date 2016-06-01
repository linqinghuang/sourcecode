#include<reg52.h>
#include<intrins.h>
#defined uint unsigned int 
#defined uchar unsigned char

void delayms(uint)
uchar aa;

void main()
{
	aa=0xfe;
	while(1)
	{
		P1=aa;
		delayms(500);		  //ÑÓÊ±500ºÁÃë
		aa=_crol_(aa, 1);

	}

}

void delayms(xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--) ;
}