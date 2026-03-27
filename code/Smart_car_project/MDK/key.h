#ifndef _KEY_H_
#define _KEY_H_

#include "headfile.h"
#include "SEEKFREE_18TFT.h"

#define CROSS_1_2 P3
#define CROSS_3_4 P6

#define KEY_PIN1 P31
#define KEY_PIN2 P30
#define KEY_PIN3 P67
#define KEY_PIN4 P65
#define KEY_PIN5 P47
#define KEY_PIN6 P44
#define KEY_PIN7 P63
#define KEY_PIN8 P61

#define SWITCH_1 P76		//拨码开关
#define SWITCH_2 P75

/*SWITCH1对应1，SWITCH2对应2*/

typedef enum
{
    Number0,Number1,Number2,Number3,Number4,
    Number5,Number6,Number7,Number8,Number9,Signx,Signj,
    LetterA,LetterB,LetterC,LetterD,Key_None
}Key_Type_e;

extern void key_init(void);
extern Key_Type_e key_scan(void);
extern Key_Type_e press_temp;
extern uint8 sw_state1, sw_state2;

extern void switch_init();

#endif /* _KEY_H_ */