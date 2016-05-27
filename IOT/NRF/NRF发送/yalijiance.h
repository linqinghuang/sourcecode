#ifndef __yalijiance_H
#define __yalijiance_H
sbit ADDO = P3^5;
sbit ADSK = P3^4;
unsigned long weight=0;
unsigned long weight_shiwu=0; 

//************************************************AD转换*********************************************************************************************
unsigned long ReadCount(void)
{ 
unsigned long Count; 
unsigned int ADval;
unsigned char i; 
		ADDO=1; 
		ADSK=0; 
		Count=0; 
		while(ADDO); 
		for(i=0;i<24;i++)   //读24个bit
			{ 
			ADSK=1; 
			Count=Count<<1; 
			ADSK=0; 
			if(ADDO)Count++; 
			} 
		ADSK=1; //发送第25个脉冲，表示下次转换使用A通道128db
		ADSK=0; 
		ADval =Count>>8;
	//	ADval = (int)(Count >>8);//取高十六位有效值 
		//ADval = (int)(Count >>8);
		ADDO=1;
return(ADval); 
} 

void Get_weight()
{

	weight=ReadCount();
	weight_shiwu=weight;
	weight_shiwu=(unsigned int)((float)weight_shiwu/4.50+0.0005);//将AD值转化成重力值，+0.05是为了四舍五入百分位

//	weight_shiwu = ReadCount();
	
}
  #endif



