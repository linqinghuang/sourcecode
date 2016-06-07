#include<reg52.h>
#include<intrins.h>
#define uint unsigned int 
#define uchar unsigned char

void delayms(uint);
uchar aa;

void main()
{
	aa=0xfe;
	while(1)
	{
		P1=aa;
		delayms(500);		  //??500??
		aa=_crol_(aa, 1);

	}

}

void delayms(xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--) ;
}