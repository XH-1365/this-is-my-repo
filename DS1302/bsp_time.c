#include "bsp_time.h"


unsigned char time_Read[]	={0x84,0x82,0x80};
unsigned char time_write[]={0x85,0x83,0x81};


void Write_Ds1302_Byte(unsigned char temp)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		SCK=0;
		SDA=temp&0x01;
		temp>>=1;
		SCK=1;
	}	
}

void Write_Ds1302(unsigned char address,unsigned char dat)
{
	RST=0;
	_nop_();
	SCK=0;
	_nop_();
	RST=1;
	_nop_();
	Write_Ds1302_Byte(address);
	Write_Ds1302_Byte(dat);
	RST=0;
}

unsigned char Read_Ds1302(unsigned char address)
{
	unsigned char i,temp = 0x00;
	RST = 0;_nop_();
	SCK = 0;_nop_();
	RST = 1;_nop_();
	Write_Ds1302_Byte(address);
	for (i=0;i<8;i++)
	{
		SCK=0;
		temp>>=1;
		if(SDA)
		temp|=0x08;
		SCK=1;
	}
	RST = 0;_nop_();
	RST = 0;SCK = 0;_nop_();
	SCK = 1;_nop_();
	SDA = 0;_nop_();
	SDA = 1;
	return (temp);
}



//导出数据
void Set_Rtc(unsigned char* usRtc)
{
	unsigned char temp,i;
	Write_Ds1302(0x8e,0);
		for(i=1;i<3;i++)
		{
			temp = (((usRtc[i]/10<<4)|(usRtc[i]%10)));
			Write_Ds1302(time_Read[i],temp);
		}
	Write_Ds1302(0x8e,0x80);
}

//写入读取数据
void Set_write(unsigned char* usRtc)
{
	unsigned char temp,i;
	for(i=0;i<2;i++)
	{
		temp = Read_Ds1302(time_write[i]);
		usRtc[i] = (((temp>>4)*10) + (temp&0x0F));
	}
}
