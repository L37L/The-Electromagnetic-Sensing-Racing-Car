#include "ui.h"

#define Page0_Item_Max  (sizeof(Page0_Item)/sizeof(Page0_Item[0])-1)
#define Page1_Item_Max  (sizeof(Page1_Item)/sizeof(Page1_Item[0])-1)
#define Page2_Item_Max  (sizeof(Page2_Item)/sizeof(Page2_Item[0])-1)
#define Page3_Item_Max  (sizeof(Page3_Item)/sizeof(Page3_Item[0])-1)
#define Page4_Item_Max  (sizeof(Page4_Item)/sizeof(Page4_Item[0])-1)
#define Page5_Item_Max 	(sizeof(Page5_Item)/sizeof(Page5_Item[0])-1)
#define Page6_Item_Max 	(sizeof(Page6_Item)/sizeof(Page6_Item[0])-1)

int8 Shut_flag = 0;
uint16 Save_Addr;
uint16 Read_Addr;
int16 Page_Num, Item_Num;

Item_Lib const Page0_Item[] =
{

        {"ad0",&ad0,Float,RO},
        {"ad1",&ad1,Float,RO},
		{"ad2", &ad2, Float, RO},
        {"ad3",&ad3,Float,RO},
        {"ad4",&ad4,Float,RO},
		{"S_err",&s_error,Float,RO},

};

Item_Lib const Page1_Item[] =
{
		{"Huan_sp", &huan_speed, Int16_t, RW},
//		{"kp", &motor_pid_l.kp, Float, RW},
//		{"ki", &motor_pid_l.ki, Float, RW},
		{"Str_sp", &speed_strait, Int16_t, RW},
		{"Ben_sp", &speed_ben, Int16_t, RW},
//		{"Zhang_sp", &zhangai_speed, Int16_t, RW},
		{"Po_sp", &podao_speed, Int16_t, RW},
		{"Diff", &diff, Float, RW},
        {"gSp",&goalspeed,Int16_t,RO},
};

Item_Lib const Page2_Item[] =
{
        {"s_kp",&(smotor_pid.kp),Float,RW},
        {"s_kd",&(smotor_pid.kd),Float,RW},
//		{"kp_ben", &kp_ben, Float, RW},
//		{"kd_ben", &kd_ben, Float, RW},
		{"chu_kp", &chu_kp, Float, RW},
		{"chu_kd", &chu_kd, Float, RW},
		{"h_kp", &huan_kp, Float, RW},
		{"h_kd", &huan_kd, Float, RW},
//	    {"Sdty",&Duty_result,Int16_t,RW},
//		{"Cdty",&change_duty, Int16_t, RW},
		
};

Item_Lib const Page3_Item[] = 
{
		{"AD0", &RAW[0], Float, RO},
		{"AD1", &RAW[1], Float, RO},
		{"AD2", &RAW[2], Float, RO},
		{"AD3", &RAW[3], Float, RO},
		{"AD4", &RAW[4], Float, RO},
		{"ruku_time", &ruku_time, Int16_t, RW},

};

Item_Lib const Page4_Item[] = 
{
	{"MAX0", &MAX[0], Float, RW},
	{"MAX1", &MAX[1], Float, RW},
	{"MAX2", &MAX[2], Float, RW},
	{"MAX3", &MAX[3], Float, RW},
	{"MAX4", &MAX[4], Float, RW},


};

Item_Lib const Page6_Item[] = 
{
//	{"r_gyro", &rhuandao_gyro, Int8_t, RW},
//	{"l_gyro", &lhuandao_gyro, Int8_t, RW},
//	{"Horizon", &horizon, Int16_t, RW},
	{"err_heng", &error_heng, Float, RW},
	{"err_xie", &error_xie, Float, RW},
//	{"Lhuan", &lhuandao_flag, Int8_t, RW},
//	{"Rhuan", &rhuandao_flag, Int8_t, RW},
//	{"Zhang", &zhangai_flag, Int8_t, RW},
//	{"Po", &podao_flag, Int8_t, RW},
//	{"Zone", &zone_flag, Int8_t, RW},
};

Item_Lib const Page5_Item[] = 
{
	{"Point0", &Dlla_control[0], Int16_t, RW},
	{"Point1", &Dlla_control[1], Int16_t, RW},
	{"Point2", &Dlla_control[2], Int16_t, RW},
	{"Point3", &Dlla_control[3], Int16_t, RW},
	{"B", &b, Int16_t, RW},
	{"Rank", &rank, Int8_t, RW},
};

Page_Lib const Page[] =
{
    {"Track", (Item_LibPtr)Page0_Item, Page0_Item_Max},
	{"AD_RAW", (Item_LibPtr)Page3_Item, Page3_Item_Max},
	{"AD_MAX", (Item_LibPtr)Page4_Item, Page4_Item_Max},
    {"Flag" ,  (Item_LibPtr)Page6_Item, Page6_Item_Max},
	{"Serve", (Item_LibPtr)Page2_Item, Page2_Item_Max},
	{"Motor", (Item_LibPtr)Page1_Item, Page1_Item_Max},
	{"Point", (Item_LibPtr)Page5_Item, Page5_Item_Max},
};

UI_CMD_e UI_CMD;
Key_Type_e Key_Detected;
Active_Type Active;

int16 const Page_MAX = (sizeof(Page) / sizeof(Page[0]) - 1);

//void Read_Data(void)
//{	
//	uint8 byte[4]={0};		//float转化为4个字节数据
//	Read_Addr = 0x0000;
//	for(Page_Num = 0; Page_Num <= Page_MAX; Page_Num++)
//	{
//		for(Item_Num = 0; Item_Num <= Page[Page_Num].Item_Max; Item_Num++)
//		{
//			if(Read_Addr > 0x0200)
//				return;
//			iap_read_bytes(Read_Addr, &DatRead[0], 1);
//			*(float*)(Page[1].ItemPtr)->DataPtr = *((float*)byte);
//			date = byte[0];
//			Read_Addr+=4;
//		}
//	}
//}
		
void UI_Init(void)
{
	key_init(); 
	lcd_init();
//	iap_init();		//eeprom初始化
//	Read_Data();
	Shut_flag = 0;
    Active.Accuracy = 1;	//调整精度初始为1
    Active.PageNum = 0;
    Active.ItemNum = 0;
    Active.PagePtr = (Page_LibPtr) Page + Active.PageNum;
    Active.ItemPtr = (Active.PagePtr->ItemPtr + Active.ItemNum);
    Key_Detected = Key_None;
    UI_CMD = UI_None;
}

//void Save_Date(void)
//{
//	uint8 byte[4]={0};		//float转化为4个字节数据
//	Save_Addr = 0x0000;
//	iap_erase_page(0x0200);
//	for(Page_Num = 0;Page_Num <= Page_MAX;Page_Num++)
//	{
//		for(Item_Num = 0;Item_Num <= Page[Page_Num].Item_Max;Item_Num++)
//		{
//			if(Save_Addr > 0x0200)
//				{return;}
//			Float_to_Byte(*(float*)(Page[2].ItemPtr)->DataPtr, byte);
//			extern_iap_write_bytes(Save_Addr, (uint8*)(Page[2].ItemPtr)->DataPtr, 1);
//			iap_write_bytes(Save_Addr,(uint8*)(Page[Page_Num].ItemPtr+Item_Num)->DataPtr,4);
//			Save_Addr+=4;
//		}
//	}
//	lcd_clear(BLACK);
//}

//void Save_Date(void)
//{
//	uint8 byte[4]={0};		//float转化为4个字节数据
//	Save_Addr = 0x0000;
//	int8 i;
//	iap_erase_page(0x0200);
//	for(i = 0; i <= 60; i++)
//	{
//		flashBuffer[i] = 
//	}
//	lcd_clear(BLACK);
//}

void Page_Plus(void)        //下一页
{

    Active.PageNum++;
    if (Active.PageNum > Page_MAX)
        Active.PageNum = 0;
    Active.PagePtr = (Page_LibPtr) Page + Active.PageNum;
    Active.ItemNum = 0;
    Active.ItemPtr = Active.PagePtr->ItemPtr + Active.ItemNum;
}

void Page_Minus(void)       //上一页
{
    Active.PageNum--;
    if (Active.PageNum < 0)
        Active.PageNum = Page_MAX;
    Active.PagePtr = (Page_LibPtr) Page + Active.PageNum;
    Active.ItemNum = 0;
    Active.ItemPtr = Active.PagePtr->ItemPtr + Active.ItemNum;
}

void Item_Plus(void)        //下一项
{
    Active.ItemNum++;
    if (Active.ItemNum > Active.PagePtr->Item_Max)
        Active.ItemNum = 0;
    Active.ItemPtr = Active.PagePtr->ItemPtr + Active.ItemNum;
}

void Item_Minus(void)       //上一项
{
    Active.ItemNum--;
    if (Active.ItemNum < 0)
        Active.ItemNum = Active.PagePtr->Item_Max;
    Active.ItemPtr = Active.PagePtr->ItemPtr + Active.ItemNum;
}

void Accuracy_Plus(void)    //调整精度x10
{

    Active.Accuracy *= 10;
    if (Active.Accuracy > 1000)
        Active.Accuracy = 0.01;
}

void Accuracy_Minus(void)   //调整精度/10
{

    Active.Accuracy /= 10;
    if (Active.Accuracy < 0.001)
        Active.Accuracy = 1000;
}



void Data_Plus(void)
{

    switch (Active.ItemPtr->Data_Attribute.Type) {          //判断当前项对应数据类型
    case Double:
        //当前项对应数据(指针对应)+精度*1
        *(double*) Active.ItemPtr->DataPtr += (double) Active.Accuracy;
        break;
    case Float:
        *(float*) Active.ItemPtr->DataPtr += (float) Active.Accuracy;
        break;
    case Int32_t:
        *(int32*) Active.ItemPtr->DataPtr += (int32) Active.Accuracy;
        break;
    case Uint32_t:
        *(uint32*) Active.ItemPtr->DataPtr += (uint32) Active.Accuracy;
        break;
    case Int16_t:
        *(int16*) Active.ItemPtr->DataPtr += (int16) Active.Accuracy;
        break;
    case Uint16_t:
        *(uint16*) Active.ItemPtr->DataPtr += (uint16) Active.Accuracy;
        break;
    case Int8_t:
        *(int8*) Active.ItemPtr->DataPtr += (int8) Active.Accuracy;
        break;
    case Uint8_t:
        *(uint8*) Active.ItemPtr->DataPtr += (uint8) Active.Accuracy;
        break;
    default:
        break;
    }
}

void Data_Minus(void)
{

    switch (Active.ItemPtr->Data_Attribute.Type) {
    case Double:
        *(double*) Active.ItemPtr->DataPtr -= (double) Active.Accuracy;
        break;
    case Float:
        *(float*) Active.ItemPtr->DataPtr -= (float) Active.Accuracy;
        break;
    case Int32_t:
        *(int32*) Active.ItemPtr->DataPtr -= (int32) Active.Accuracy;
        break;
    case Uint32_t:
        *(uint32*) Active.ItemPtr->DataPtr -= (uint32) Active.Accuracy;
        break;
    case Int16_t:
        *(int16*) Active.ItemPtr->DataPtr -= (int16) Active.Accuracy;
        break;
    case Uint16_t:
        *(uint16*) Active.ItemPtr->DataPtr -= (uint16) Active.Accuracy;
        break;
    case Int8_t:
        *(int8*) Active.ItemPtr->DataPtr -= (int8) Active.Accuracy;
        break;
    case Uint8_t:
        *(uint8*) Active.ItemPtr->DataPtr -= (uint8) Active.Accuracy;
        break;
    default:
        break;
    }
}

//按键发车
void start_motor(void)
{
	Shut_flag = 1;
	start_flag = 1;
}


void UI_ShowData(uint8 y,Item_LibPtr ItemPtr)
{
    lcd_showstr(10,y,ItemPtr->Name);    //显示项目名
    switch (ItemPtr->Data_Attribute.Type) {     //根据项目对应数据类型显示数据
    case Float:
        lcd_showfloat(70, y, (float) *(float*) ItemPtr->DataPtr,5,2);
        break;
    case Int32_t:
        lcd_showint32(80, y, (int32) *(int32*) ItemPtr->DataPtr,5);
        break;
    case Uint32_t:
        lcd_showint32(80, y, (int32) *(int32*) ItemPtr->DataPtr,5);
        break;
    case Int16_t:
        lcd_showint16(80, y, (int32) *(int16*) ItemPtr->DataPtr);
        break;
    case Uint16_t:
        lcd_showuint16(80, y, (int32) *(uint16*) ItemPtr->DataPtr);
        break;
    case Int8_t:
        lcd_showint8(80, y, (int32) *(int8*) ItemPtr->DataPtr);
        break;
    }
}

void UI_GetCmd(void)
{
    static Key_Type_e Key_Pressed_New = Key_None;
    static Key_Type_e Key_Pressed_Old = Key_None;

    Key_Pressed_New = key_scan();        //通过按键扫描获取当前按键状态

    if (Key_Pressed_New == Key_Pressed_Old)
    {
        UI_CMD = UI_None;
    }
    else
    {
        UI_CMD = (UI_CMD_e) Key_Pressed_New;
        Key_Pressed_Old = Key_Pressed_New;
    }
}

void UI_Process(void)
{

    switch (UI_CMD) {
    case UI_Value_Plus:
        Data_Plus();
//      Save_Data();
        break;
    case UI_Value_Minus:
        Data_Minus();
//      Save_Data();
        break;
    case UI_Page_Plus:
        Page_Plus();
        break;
    case UI_Page_Minus:
        Page_Minus();
        break;
    case UI_Item_Plus:
        Item_Plus();
        break;
    case UI_Item_Minus:
        Item_Minus();
        break;
    case UI_Accuracy_Plus:
        Accuracy_Plus();
        break;
    case UI_Accuracy_Minus:
        Accuracy_Minus();
        break;
	case UI_Save:
//		Save_Date();
		break;
    case UI_Start:
        start_motor();
		break;
    default:
        break;
    }
}

void UI_PutMsg(void)
{

    if(UI_CMD != UI_None)
        lcd_clear(WHITE);                       //清屏

    lcd_showstr(0, 0, Active.PagePtr->Name);
    lcd_showfloat(60,0,Active.Accuracy,4,3);
    if (Active.ItemNum < 6)
	{
		lcd_showstr(0, (Active.ItemNum + 1), ">");
        UI_ShowData( 1,Active.PagePtr->ItemPtr);
		if (Active.PagePtr->Item_Max<1)
        {
			return;
        }
        UI_ShowData( 2,Active.PagePtr->ItemPtr + 1);
        if (Active.PagePtr->Item_Max<2)
        {
			return;
        }
        UI_ShowData(3,Active.PagePtr->ItemPtr + 2);
        if (Active.PagePtr->Item_Max<3)
        {
            return;
        }
        UI_ShowData( 4,Active.PagePtr->ItemPtr + 3);
        if (Active.PagePtr->Item_Max<4)
        {
            return;
        }
        UI_ShowData( 5,Active.PagePtr->ItemPtr + 4);
        if (Active.PagePtr->Item_Max<5)
        {
            return;
        }
        UI_ShowData( 6,Active.PagePtr->ItemPtr + 5);

	}
    else
    {
        lcd_showstr(3,1, ">");
        UI_ShowData( 1,Active.ItemPtr - 5);
        UI_ShowData( 2,Active.ItemPtr - 4);
        UI_ShowData( 3,Active.ItemPtr - 3);
        UI_ShowData( 4,Active.ItemPtr - 2);
        UI_ShowData( 5,Active.ItemPtr - 1);
        UI_ShowData( 6,Active.ItemPtr);
    }
}

void UI_Main(void)
{
		UI_GetCmd();
		UI_Process();
		UI_PutMsg();

}