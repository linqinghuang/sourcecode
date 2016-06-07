#define LCD_Data P2
#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ


sbit LCD_RS=P1^0;//�ⲿ����12864����������
sbit LCD_RW=P1^1;
sbit LCD_E=P1^2;
sbit PSB =P3^6;		//PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1

void WriteDataLCD(unsigned char WDLCD);
void WriteCommandLCD(unsigned char WCLCD,BuysC);
unsigned char ReadDataLCD(void);
unsigned char ReadStatusLCD(void);
void LCDInit(void);
void LCDClear(void);
void LCDFlash(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void DisplayImage (unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);

int datedisplay[9];
int x,y;
unsigned char wet[2];
unsigned char temp[2];

code unsigned char  code1[] = {"������"};
code unsigned char  code2[] = {"δ����"};
code unsigned char  code3[] = {"������"};
code unsigned char  code4[] = {"������"};
code unsigned char  code5[] = {"������"};
code unsigned char  code6[] = {"��δ��"};
code unsigned char  code7[] = {"����"};
code unsigned char  code8[] = {"���"};
code unsigned char  code9[] = {"�¶�:"};
code unsigned char  code10[] = {"���϶�"};
code unsigned char  code11[] = {"ʪ��:"};
code unsigned char  code12[] = {"%"};

void display(void)
{
 int datedisplay[9]={1,1,1,1,1,1,1,1,1};
 Delay400Ms(); //�����ȴ�����LCD���빤��״̬
 LCDInit(); //LCM��ʼ��
 Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
 		LCDClear();
		//Delay400Ms();
		//Delay400Ms();
		Delay400Ms();
		Delay400Ms();
		Delay400Ms();
		LCDClear();
	 /***************************************/
	 if(datedisplay[4]==1)
	 {
			DisplayListChar(0,1,code2);	
	 }
	 else//�ⲿ�����ж�С���Ƿ��ڳ���
	 {
			DisplayListChar(0,1,code1);
	 }
	 /****************************************/
	 
	 /***************************************/
	 if(datedisplay[5]==1)
	 {
			DisplayListChar(0,2,code6);	
	 }
	 else//�ⲿ�����жϳ��ǿ��Ļ���������
	 {
			DisplayListChar(0,2,code5);
	 }
	 /****************************************/
	 
	 /***************************************/
	 if(datedisplay[6]==1)
	 {
			DisplayListChar(4,2,code8);	
	 }
	 else//�ⲿ�����ж�С���Ƿ����
	 {
			DisplayListChar(4,2,code7);
	 }
	 /****************************************/
	 
	 /***************************************/
   //��ʪ��Ӧ����ôд��
	 //Ūһ���յ����飬Ȼ��datadisplay����ʪ���������ȥ
	 
	 for(x=0;x<2;x++)
	 {
			wet[x]=wet[x];
	 }
	 for(y=2;y<4;y++)
	 {
			temp[y-2]=temp[y];
	 }
	 DisplayListChar(0,3,code9);
	 DisplayListChar(3,3,temp);
	 DisplayListChar(5,3,code10);
	 DisplayListChar(0,4,code11);
	 DisplayListChar(4,4,wet);
	 DisplayListChar(6,4,code12);
	 
			
	 /****************************************/
		Delay400Ms();
		Delay400Ms();
	  Delay400Ms();
	//	Delay400Ms();
 	//	LCDFlash();	//��˸Ч��
		//}

}

//д����
void WriteDataLCD(unsigned char WDLCD)
{
 ReadStatusLCD(); //���æ 
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_Data = WDLCD;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;
}

//дָ��
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysCΪ0ʱ����æ���
{
 if (BuysC) ReadStatusLCD(); //������Ҫ���æ 
 LCD_RS = 0;
 LCD_RW = 0; 
 LCD_Data = WCLCD;
 LCD_E = 1; 
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;  
}

//������
unsigned char ReadDataLCD(void)
{
 LCD_RS = 1; 
 LCD_RW = 1;
 LCD_E = 0;
 LCD_E = 0;
 LCD_E = 1;
 return(LCD_Data);
}

//��״̬
unsigned char ReadStatusLCD(void)
{
 LCD_Data = 0xFF; 
 LCD_RS = 0;
 LCD_RW = 1; 
 LCD_E = 1;
 while (LCD_Data & Busy); //���æ�ź�
 LCD_E = 0;
 return(LCD_Data);
}

void LCDInit(void) //LCM��ʼ��
{
 WriteCommandLCD(0x30,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCD(0x01,1); //��ʾ����
 WriteCommandLCD(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCD(0x0C,1); // ��ʾ�����������
}
void LCDClear(void) //����
{
 WriteCommandLCD(0x01,1); //��ʾ����
 WriteCommandLCD(0x34,1); // ��ʾ����ƶ�����
 WriteCommandLCD(0x30,1); // ��ʾ�����������
}
void LCDFlash(void)	//��˸Ч��
{
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // ��ʾ�����������
 Delay400Ms();
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // ��ʾ�����������
 Delay400Ms();
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
}
//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //����X���ܴ���16��Y���ܴ���1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 WriteCommandLCD(X, 0); //���ﲻ���æ�źţ����͵�ַ��
 WriteDataLCD(DData);
}

//��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength,X2;
  ListLength = 0;
  X2=X;
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //����X���ܴ���16��Y��1-4֮��
 switch(Y){
	case 1:X2|=0X80;break;	//����������ѡ����Ӧ��ַ
	case 2:X2|=0X90;break;
	case 3:X2|=0X88;break;
	case 4:X2|=0X98;break;
	}
 WriteCommandLCD(X2, 1); //���͵�ַ��
  while (DData[ListLength]>=0x20) //�������ִ�β���˳�
  {
   if (X <= 0x0F) //X����ӦС��0xF
    {
     WriteDataLCD(DData[ListLength]); //
     ListLength++;
	 X++;
	 Delay5Ms();
    }
  }
}


//5ms��ʱ
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}

//400ms��ʱ
void Delay400Ms(void)
{
 unsigned char TempCycA = 5;
 unsigned int TempCycB;
 while(TempCycA--)
 {
  TempCycB=7269;
  while(TempCycB--);
 };
}
