/**********************************************/
/***********  Author : Mohamed Samy ***********/
/***********  Layer : HAL           ***********/
/***********  Component: Keypad     ***********/
/***********  Version: 1.00         ***********/
/***********                         ***********/
/**********************************************/


#ifndef _KEYPAD_INTERFACE_H_
#define _KEYPAD_INTERFACE_H_

#define KPD_ARR4x4     {{1,2,3,'a'},{4,5,6,'b'},{7,8,9,'c'},{'d',0,'e','m'}}
#define KPD_NO_PRESSED_KEY   0xFF


/* user define which port is connected to KPD */
#define KPD_PORT       PORTC

/* user define which pins are connected to KPD */
#define KPD_COL0_PIN      	PIN0
#define KPD_COL1_PIN		PIN1
#define KPD_COL2_PIN		PIN2
#define KPD_COL3_PIN		PIN3


#define KPD_ROW0_PIN		PIN4
#define KPD_ROW1_PIN		PIN5
#define KPD_ROW2_PIN		PIN6
#define KPD_ROW3_PIN		PIN7

u8 KPD_u8GetPressedKey(void);
void KPD_VidInit (void);





#endif
