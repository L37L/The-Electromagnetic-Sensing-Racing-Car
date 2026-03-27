#include "wireless.h"

/*
将浮点数f转化为4个字节数据存放在byte[4]中
*/
uint8 DatRead[4];
void Float_to_Byte(float f, uint8 byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

void JustFloat(float a, float b)
{
	
	uint8 byte[4]={0};		//float转化为4个字节数据
	uint8 tail[4]={0x00, 0x00, 0x80, 0x7f};	//帧尾
	
	//向上位机发送两个通道数据
	Float_to_Byte(a,byte);
	wireless_uart_send_buff(byte, 4);	//1转化为4字节数据 就是  0x00 0x00 0x80 0x3F
	
	Float_to_Byte(b,byte);
	wireless_uart_send_buff(byte, 4);	//2转换为4字节数据 就是  0x00 0x00 0x00 0x40 
	
	//发送帧尾
	wireless_uart_send_buff(tail,4);	//帧尾为 0x00 0x00 0x80 0x7f
}

void Write_Xram_Float(float write_data)
{
	u_float f;
	f.fds = write_data;

	DatRead[0] = f.byt[3];
	DatRead[1] = f.byt[2];
	DatRead[2] = f.byt[1];
	DatRead[3] = f.byt[0];
}

float Read_RAM_Float(uint8 write_data[4])
{
	u_float f;
	f.byt[3] = write_data[0];
	f.byt[2] = write_data[1];
	f.byt[1] = write_data[2];
	f.byt[0] = write_data[3];
	return (f.fds);
}