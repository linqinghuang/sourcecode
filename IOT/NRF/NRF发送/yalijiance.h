#ifndef __yalijiance_H
#define __yalijiance_H
sbit ADDO = P3^5;
sbit ADSK = P3^4;
unsigned long weight=0;
unsigned long weight_shiwu=0; 

//************************************************ADת��*********************************************************************************************
unsigned long ReadCount(void)
{ 
unsigned long Count; 
unsigned int ADval;
unsigned char i; 
		ADDO=1; 
		ADSK=0; 
		Count=0; 
		while(ADDO); 
		for(i=0;i<24;i++)   //��24��bit
			{ 
			ADSK=1; 
			Count=Count<<1; 
			ADSK=0; 
			if(ADDO)Count++; 
			} 
		ADSK=1; //���͵�25�����壬��ʾ�´�ת��ʹ��Aͨ��128db
		ADSK=0; 
		ADval =Count>>8;
	//	ADval = (int)(Count >>8);//ȡ��ʮ��λ��Чֵ 
		//ADval = (int)(Count >>8);
		ADDO=1;
return(ADval); 
} 

void Get_weight()
{

	weight=ReadCount();
	weight_shiwu=weight;
	weight_shiwu=(unsigned int)((float)weight_shiwu/4.50+0.0005);//��ADֵת��������ֵ��+0.05��Ϊ����������ٷ�λ

//	weight_shiwu = ReadCount();
	
}
  #endif



