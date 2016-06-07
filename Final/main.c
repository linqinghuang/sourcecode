#include <reg52.h>
#include <intrins.h>
#include "nrf12864.h"
#include "wetmp.h"
#define uchar unsigned char
#define uint unsigned int


sbit CE=P3^0;
sbit SOUND =P1^4;
sbit PLAY = P1^5;
sbit WETEMP = P1^3;

void wenshi(int shujv[])
{	 
		uint i=0;
		RH(); 
		shujv[0]=U8RH_data_H/10+48;	
		shujv[1]=U8RH_data_H%10+48;
		shujv[2]=U8T_data_H/10+48;
		shujv[3]=U8T_data_H%10+48;
		for(i=0;i<3000;i++);
}

void main()
{
	wenshi(shujv);
	display();	
}