#define LCD_Data P2
#define Busy    0x80 //用于检测LCD状态字中的Busy标识


sbit LCD_RS=P1^0;//这部分是12864的引脚声明
sbit LCD_RW=P1^1;
sbit LCD_E=P1^2;
sbit PSB =P3^6;		//PSB脚为12864-12系列的串、并通讯功能切换，我们使用8位并行接口，PSB=1

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

code unsigned char  code1[] = {"被抱起"};
code unsigned char  code2[] = {"未抱起"};
code unsigned char  code3[] = {"声音开"};
code unsigned char  code4[] = {"声音关"};
code unsigned char  code5[] = {"车已锁"};
code unsigned char  code6[] = {"车未锁"};
code unsigned char  code7[] = {"安静"};
code unsigned char  code8[] = {"啼哭"};
code unsigned char  code9[] = {"温度:"};
code unsigned char  code10[] = {"摄氏度"};
code unsigned char  code11[] = {"湿度:"};
code unsigned char  code12[] = {"%"};

void display(void)
{
 int datedisplay[9]={1,1,1,1,1,1,1,1,1};
 Delay400Ms(); //启动等待，等LCD讲入工作状态
 LCDInit(); //LCM初始化
 Delay5Ms(); //延时片刻(可不要)
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
	 else//这部分是判断小孩是否在车内
	 {
			DisplayListChar(0,1,code1);
	 }
	 /****************************************/
	 
	 /***************************************/
	 if(datedisplay[5]==1)
	 {
			DisplayListChar(0,2,code6);	
	 }
	 else//这部分是判断车是开的还是所锁的
	 {
			DisplayListChar(0,2,code5);
	 }
	 /****************************************/
	 
	 /***************************************/
	 if(datedisplay[6]==1)
	 {
			DisplayListChar(4,2,code8);	
	 }
	 else//这部分是判断小孩是否哭闹
	 {
			DisplayListChar(4,2,code7);
	 }
	 /****************************************/
	 
	 /***************************************/
   //温湿度应该怎么写呢
	 //弄一个空的数组，然后将datadisplay的温湿度数据填进去
	 
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
 	//	LCDFlash();	//闪烁效果
		//}

}

//写数据
void WriteDataLCD(unsigned char WDLCD)
{
 ReadStatusLCD(); //检测忙 
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_Data = WDLCD;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;
}

//写指令
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysC为0时忽略忙检测
{
 if (BuysC) ReadStatusLCD(); //根据需要检测忙 
 LCD_RS = 0;
 LCD_RW = 0; 
 LCD_Data = WCLCD;
 LCD_E = 1; 
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;  
}

//读数据
unsigned char ReadDataLCD(void)
{
 LCD_RS = 1; 
 LCD_RW = 1;
 LCD_E = 0;
 LCD_E = 0;
 LCD_E = 1;
 return(LCD_Data);
}

//读状态
unsigned char ReadStatusLCD(void)
{
 LCD_Data = 0xFF; 
 LCD_RS = 0;
 LCD_RW = 1; 
 LCD_E = 1;
 while (LCD_Data & Busy); //检测忙信号
 LCD_E = 0;
 return(LCD_Data);
}

void LCDInit(void) //LCM初始化
{
 WriteCommandLCD(0x30,1); //显示模式设置,开始要求每次检测忙信号
 WriteCommandLCD(0x01,1); //显示清屏
 WriteCommandLCD(0x06,1); // 显示光标移动设置
 WriteCommandLCD(0x0C,1); // 显示开及光标设置
}
void LCDClear(void) //清屏
{
 WriteCommandLCD(0x01,1); //显示清屏
 WriteCommandLCD(0x34,1); // 显示光标移动设置
 WriteCommandLCD(0x30,1); // 显示开及光标设置
}
void LCDFlash(void)	//闪烁效果
{
 WriteCommandLCD(0x08,1); //显示清屏
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // 显示开及光标设置
 Delay400Ms();
 WriteCommandLCD(0x08,1); //显示清屏
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // 显示开及光标设置
 Delay400Ms();
 WriteCommandLCD(0x08,1); //显示清屏
 Delay400Ms();
}
//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //限制X不能大于16，Y不能大于1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 WriteCommandLCD(X, 0); //这里不检测忙信号，发送地址码
 WriteDataLCD(DData);
}

//按指定位置显示一串字符
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength,X2;
  ListLength = 0;
  X2=X;
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //限制X不能大于16，Y在1-4之内
 switch(Y){
	case 1:X2|=0X80;break;	//根据行数来选择相应地址
	case 2:X2|=0X90;break;
	case 3:X2|=0X88;break;
	case 4:X2|=0X98;break;
	}
 WriteCommandLCD(X2, 1); //发送地址码
  while (DData[ListLength]>=0x20) //若到达字串尾则退出
  {
   if (X <= 0x0F) //X坐标应小于0xF
    {
     WriteDataLCD(DData[ListLength]); //
     ListLength++;
	 X++;
	 Delay5Ms();
    }
  }
}


//5ms延时
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}

//400ms延时
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
