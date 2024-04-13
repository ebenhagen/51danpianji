/**************************************************************************************
深圳市普中科技有限公司（PRECHIN 普中）
技术支持：www.prechin.net
PRECHIN
 普中

实验名称：LED点阵实验(显示数字)
接线说明：	
实验现象：下载程序后，8*8LED点阵显示数字0
注意事项：LED点阵旁的J24黄色跳线帽短接到GND一端																				  
***************************************************************************************/
#include "reg51.h"
#include <INTRINS.H>

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;

//定义74HC595控制管脚
sbit SRCLK=P3^6;	//移位寄存器时钟输入
sbit RCLK=P3^5;		//存储寄存器时钟输入
sbit SER=P3^4; 		//串行数据输入

sbit k1 = P3^1; //独立按键

#define LEDDZ_COL_PORT	P0	//点阵列控制端口


u8 gled_row0[8]={0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00};//LED点阵显示数字0的行数据

u8 gled_row1[8]={0x00,0x00,0x00,0x40,0xFF,0x00,0x00,0x00};//LED点阵显示数字1的行数据

u8 gled_row2[8]={0x00,0x41,0x83,0x85,0x89,0x71,0x00,0x00};//LED点阵显示数字2的行数据

u8 gled_row3[8]={0x00,0x42,0x89,0x89,0x89,0x76,0x00,0x00};//LED点阵显示数字3的行数据

u8 gled_row4[8]={0x00,0x70,0x08,0x08,0x08,0x7E,0x00,0x00};//LED点阵显示数字4的行数据

u8 gled_row5[8]={0x00,0x32,0x49,0x49,0x49,0x46,0x00,0x00};//LED点阵显示数字5的行数据

u8 gled_row6[8]={0x00,0x7E,0x89,0x89,0x89,0x46,0x00,0x00};//LED点阵显示数字6的行数据

u8 gled_row7[8]={0x00,0x40,0x40,0x40,0x40,0x7F,0x00,0x00};//LED点阵显示数字7的行数据

u8 gled_row8[8]={0x00,0x76,0x89,0x89,0x89,0x76,0x00,0x00};//LED点阵显示数字8的行数据

u8 gled_row9[8]={0x00,0x72,0x89,0x89,0x89,0x7E,0x00,0x00};//LED点阵显示数字9的行数据

u8 gled_col[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED点阵列数据

u16 time = 0;



					 /***********生日快乐**************************************************************************/
void Delay(unsigned char xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}


void MetrixLED_Init()
{
	RCLK=0;
	SRCLK=0;
}

//74HC595输出要显示哪一行 Row:0~7
void _74HC595_WriteRow(unsigned char Row)
{
	unsigned char i = 0;
	Row=0x80>>Row;
	for(i=0;i<8;i++)
	{
		SER=Row&(0x80>>i);
		SRCLK=1;
		SRCLK=0;
	}
	RCLK=1;
	RCLK=0;
}

//Data: 1亮0灭
void MertixLED_ShowRow(unsigned char Row,Data)
{
	_74HC595_WriteRow(Row);
	P0=~Data;
	Delay(2);
	_74HC595_WriteRow(0xff);
}


unsigned char code Animation[]={
    0x02,0x1C,0x28,0x48,0x7E,0x08,0x2C,0x5A,
 	0x44,0x44,0x4F,0xE5,0x4F,0x44,0x4A,0x51,
    0x00,0x7C,0x44,0x44,0x7C,0x44,0x44,0x7C,  
	0x14,0x24,0x7F,0x84,0x04,0x3F,0x04,0x3F
};

/***********生日快乐********************************************************************************************/
  


/*******************************************************************************
* 函 数 名       : delay_10us
* 函数功能		 : 延时函数，ten_us=1时，大约延时10us
* 输    入       : ten_us
* 输    出    	 : 无
*******************************************************************************/
void delay_10us(u16 ten_us)
{
	while(ten_us--);	
}

/*******************************************************************************
* 函 数 名         : hc595_write_data(u8 dat)
* 函数功能		   : 向74HC595写入一个字节的数据
* 输    入         : dat：数据
* 输    出         : 无
*******************************************************************************/
void hc595_write_data(u8 dat)
{
	u8 i=0;
	
	for(i=0;i<8;i++)//循环8次即可将一个字节写入寄存器中
	{
		SER=dat>>7;//优先传输一个字节中的高位
		dat<<=1;//将低位移动到高位
		SRCLK=0;
		delay_10us(1);
		SRCLK=1;
		delay_10us(1);//移位寄存器时钟上升沿将端口数据送入寄存器中	
	}
	RCLK=1;
	delay_10us(1);
	RCLK=0;//存储寄存器时钟上升沿将前面写入到寄存器的数据输出	
}


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	

  
unsigned char i,offset=24,count=0;
	MetrixLED_Init();

    while(1)
	{
		for(i=0;i<8;i++)
		{
			MertixLED_ShowRow(i,Animation[i+offset]);
		}
		count++;
		if(count>5)
		{
			count=0;

			offset--;
			if(offset < 1)  //向下移动
				offset=24;
		/*	offset++;
			if(offset>32)  //向上移动
				offset=0;*/
			
		}
		if(k1 == 0)
	    {
				  u8 i=0;  

	while(1)
	{			
	
	    time = 50;
		while(time) {
		   time--;
		   for(i=0;i<8;i++)//循环8次扫描8行、列
		   {
		  	LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row9[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		   }		
	    }

	    time = 50;
		while(time) {

		time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row8[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}


	    time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
		
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row7[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
	
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row6[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
	
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row5[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
	
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row4[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
	
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row3[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
		
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row2[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
		
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row1[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}

		 time = 50;
		while(time) {
			time--;
		for(i=0;i<8;i++)//循环8次扫描8行、列
		{
		
			LEDDZ_COL_PORT=gled_col[i];//传送列选数据
			hc595_write_data(gled_row0[i]);//传送行选数据
			delay_10us(100);//延时一段时间，等待显示稳定
			hc595_write_data(0x00);//消影	
		}
		}


						
	}	
	}	
	    }
	}
