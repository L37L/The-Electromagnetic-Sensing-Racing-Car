#ifndef _UI_H_
#define _UI_H_

#include "headfile.h"
#include "key.h"
#include "SEEKFREE_18TFT.h"
#include "adc.h"
#include "motor.h"
#include "encoder.h"
#include "servomotor.h"
#include "isr.h"
#include "gyroscope.h"
#include "wireless.h"
#include "SEEKFREE_DL1B.h"

typedef enum
{
	UI_None         	= Key_None,
    UI_Value_Plus       = Number2,
    UI_Value_Minus      = Number1,
    UI_Item_Plus        = Number3,
    UI_Item_Minus       = LetterC,
    UI_Page_Plus        = Number5,
    UI_Page_Minus       = Number8,
    UI_Accuracy_Plus    = Number4,
    UI_Accuracy_Minus   = Number6,
    UI_Save         	= Signj,
    UI_Shut         	= Number0,
    UI_Start        	= Number9,
}UI_CMD_e,*UI_CMD_Ptr_e;
/*//s2上一项
//s5精度减小
//s8下一项目标
//s10下一页
//s9上一页
//s11关闭按键
//s12数值增加
//s13精度增加1变10
//s16数值减小
//s15数字减小
//s6发车
*/


/*************************
 **变量的参数，包括变量的类型和权限**
 *************************/
typedef struct 
{
    enum
	{
        Double,
        Float,
        Int32_t,
        Uint32_t,
        Int16_t,
        Uint16_t,
        Int8_t,
        Uint8_t,
        Function,   //表示显示图像的函数
    }Type;

    enum
	{
        RO,         //只读
        RW,         //读写
    }Authority;
	
}Variable_Attribute;

/********************************************
 ************一个Item所必备的元素*****************
 ********************************************/
typedef struct
{
    signed char*        Name;               //Item名，字符串类型
    void*               DataPtr;            //这个Item指向的数据地址
    Variable_Attribute  Data_Attribute;     //这个Item对应数据的参数
}Item_Lib,*Item_LibPtr;

/********************************************
 ************一个Page所必备的元素*****************
 ********************************************/
typedef struct{
    signed char*        Name;               //Page名，字符串类型
    Item_LibPtr         ItemPtr;            //这个Page的第一个Item的首地址
    int16            	Item_Max;           //这个Page对应的最大的Item个数
}Page_Lib,*Page_LibPtr;

/********************************************
 ***************保存当前的活动状态*****************
 ********************************************/
typedef struct{
    float               Accuracy;           //当前数据调整精度
    int16             	PageNum;            //当前的Page，类型：整形数
    Page_LibPtr         PagePtr;            //当前Page指向的Page信息首地址，类型：指针
    int16             	ItemNum;            //当前的Item，类型：整形数
    Item_LibPtr         ItemPtr;            //当前Item指向的Item信息首地址，类型：指针
}Active_Type,*Active_TypePtr;

extern void UI_Init(void);
extern void UI_Process(void);
extern void UI_ShowData(uint8 y,Item_LibPtr ItemPtr);
extern void UI_GetCmd(void);
extern void UI_PutMsg(void);
extern void UI_Main(void);
extern int8 Shut_flag;

#endif /* _UI_H_ */