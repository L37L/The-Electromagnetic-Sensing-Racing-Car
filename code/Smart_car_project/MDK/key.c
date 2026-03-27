#include "key.h"

uint8 key_value1;
uint8 key_value2;
uint8 key_value3;
uint8 key_value4;
uint8 key_value5;
uint8 key_value6;
uint8 key_value7;
uint8 key_value8;

uint8 key_last_value5;
uint8 key_last_value6;
uint8 key_last_value7;
uint8 key_last_value8;

uint8 sw_state1;		//拨码开关状态变量
uint8 sw_state2;

Key_Type_e press_temp = Key_None;

void key_init(void)
{
	gpio_mode(KEY_PIN1, GPIO);	gpio_pull_set(KEY_PIN1, PULLUP); KEY_PIN1 = 1; 
	gpio_mode(KEY_PIN2, GPIO);	gpio_pull_set(KEY_PIN2, PULLUP); KEY_PIN2 = 1;
	gpio_mode(KEY_PIN3, GPIO);	gpio_pull_set(KEY_PIN3, PULLUP); KEY_PIN3 = 1;
	gpio_mode(KEY_PIN4, GPIO);	gpio_pull_set(KEY_PIN4, PULLUP); KEY_PIN4 = 1;
	gpio_mode(KEY_PIN5, GPIO);	gpio_pull_set(KEY_PIN5, PULLUP); KEY_PIN5 = 1;
	gpio_mode(KEY_PIN6, GPIO);	gpio_pull_set(KEY_PIN6, PULLUP); KEY_PIN6 = 1; 
	gpio_mode(KEY_PIN7, GPIO);	gpio_pull_set(KEY_PIN7, PULLUP); KEY_PIN7 = 1;
	gpio_mode(KEY_PIN8, GPIO);	gpio_pull_set(KEY_PIN8, PULLUP); KEY_PIN8 = 1;
	
	sw_state1 = SWITCH_1;
	sw_state2 = SWITCH_2;
}
/*
 * 键盘
 * 1  2  3  A
 * 4  5  6  B
 * 7  8  9  C
 * X  0  J  D
 */

Key_Type_e key_scan(void)
{

	press_temp = Key_None;
	
	KEY_PIN1 = 0;
	KEY_PIN2 = 1;
	KEY_PIN3 = 1;
	KEY_PIN4 = 1;
	key_last_value5 = key_value5;
	key_last_value6 = key_value6;
	key_last_value7 = key_value7;
	key_last_value8 = key_value8;
	key_value5 = KEY_PIN5;
	key_value6 = KEY_PIN6;
	key_value7 = KEY_PIN7;
	key_value8 = KEY_PIN8;
	if(key_value5 && !key_last_value5)
	{		
		press_temp = Number1;
	}
	if(key_value6 && !key_last_value6) 
	{
		press_temp = Number2;
	}
	if(key_value7 && !key_last_value7) 
	{
		press_temp = Number3;
	}
	if(key_value8 && !key_last_value8) 
	{
		press_temp = LetterA;
	}
	
	KEY_PIN1 = 1;
	KEY_PIN2 = 0;
	KEY_PIN3 = 1;
	KEY_PIN4 = 1;
	key_last_value5 = key_value5;
	key_last_value6 = key_value6;
	key_last_value7 = key_value7;
	key_last_value8 = key_value8;
	key_value5 = KEY_PIN5;
	key_value6 = KEY_PIN6;
	key_value7 = KEY_PIN7;
	key_value8 = KEY_PIN8;
	if(key_value5  && !key_last_value5) 
	{
		press_temp = Number4;
	}
	if(key_value6  && !key_last_value6) 
	{
		press_temp = Number5;
	}
	if(key_value7  && !key_last_value7) 
	{
		press_temp = Number6;
	}
	if(key_value8  && !key_last_value8) 
	{
		press_temp = LetterB;
	}
	
	KEY_PIN1 = 1;
	KEY_PIN2 = 1;
	KEY_PIN3 = 0;
	KEY_PIN4 = 1;
	key_last_value5 = key_value5;
	key_last_value6 = key_value6;
	key_last_value7 = key_value7;
	key_last_value8 = key_value8;
	key_value5 = KEY_PIN5;
	key_value6 = KEY_PIN6;
	key_value7 = KEY_PIN7;
	key_value8 = KEY_PIN8;
	if(key_value5  && !key_last_value5) 
	{
		press_temp = Number7;
	}
	if(key_value6  && !key_last_value6) 
	{
		press_temp = Number8;
	}
	if(key_value7  && !key_last_value7) 
	{
		press_temp = Number9;
	}
	if(key_value8  && !key_last_value8) 
	{
		press_temp = LetterC;
	}
	
	KEY_PIN1 = 1;
	KEY_PIN2 = 1;
	KEY_PIN3 = 1;
	KEY_PIN4 = 0;
	key_last_value5 = key_value5;
	key_last_value6 = key_value6;
	key_last_value7 = key_value7;
	key_last_value8 = key_value8;
	key_value5 = KEY_PIN5;
	key_value6 = KEY_PIN6;
	key_value7 = KEY_PIN7;
	key_value8 = KEY_PIN8;
	if(key_value5  && !key_last_value5) 
	{
		press_temp = Signx;
	}
	if(key_value6  && !key_last_value6) 
	{
		press_temp = Number0;
	}
	if(key_value7  && !key_last_value7) 
	{
		press_temp = Signj;
	}
	if(key_value8  && !key_last_value8) 
	{
		press_temp = LetterD;
	}

	KEY_PIN1 = 1;
	KEY_PIN2 = 1;
	KEY_PIN3 = 1;
	KEY_PIN4 = 1;
	return (press_temp);
}
