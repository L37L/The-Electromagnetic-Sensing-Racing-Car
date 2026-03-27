#ifndef _WIRELESS_H_
#define _WIRELESS_H_

#include "headfile.h"
#include "SEEKFREE_WIRELESS.h"
/*
要点提示:
1. float和unsigned long具有相同的数据结构长度
2. union据类型里的数据存放在相同的物理空间
*/
typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;

typedef union
{
	float fds;
	uint8 byt[4];
}u_float;

extern uint8 DatRead[4];
//extern void Float_to_Byte(float f, uint8 byte[]);
extern void JustFloat(float a, float b);

#endif /* _WIRELESS_H_ */