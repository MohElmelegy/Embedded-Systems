
#include "STD_Types.h"
#include "DIO_Interface.h"
#include "KEYPAD_Interface.h"

u8 KPD_u8GetPressedKey(void)
{
	u8 LOC_u8ColumnIndex;
	u8 LOC_u8RowIndex;
	u8 LOC_u8KPDArr[4][4]=KPD_ARR4x4;
	u8 LOC_u8PressedKey= KPD_NO_PRESSED_KEY;
	u8 LOC_u8PinState;

	u8 LOC_u8KPDRowArr[]={KPD_ROW0_PIN,KPD_ROW1_PIN,KPD_ROW2_PIN,KPD_ROW3_PIN};
	u8 LOC_u8KPDColArr[]={KPD_COL0_PIN,KPD_COL1_PIN,KPD_COL2_PIN,KPD_COL3_PIN};

	for(LOC_u8ColumnIndex=0;LOC_u8ColumnIndex<4;LOC_u8ColumnIndex++)
	{
		/*Activate the current column == low*/
		DIO_VidSetPinValue(KPD_PORT,LOC_u8KPDColArr[LOC_u8ColumnIndex],LOW);

		for(LOC_u8RowIndex=0;LOC_u8RowIndex<4;LOC_u8RowIndex++)
		{
			LOC_u8PinState=DIO_u8GetPinValue(KPD_PORT,LOC_u8KPDRowArr[LOC_u8RowIndex]);
			if(LOW==LOC_u8PinState)
			{
				LOC_u8PressedKey=LOC_u8KPDArr[LOC_u8RowIndex][LOC_u8ColumnIndex];
				/* wait for user to release the button */
				/* Busy waiting until the pressed key is released*/
				while(LOW==LOC_u8PinState)
				{
					LOC_u8PinState=DIO_u8GetPinValue(KPD_PORT,LOC_u8KPDRowArr[LOC_u8RowIndex]);
				}
				LOC_u8PressedKey=LOC_u8KPDArr[LOC_u8RowIndex][LOC_u8ColumnIndex];
				return LOC_u8PressedKey;
			}
		}
		DIO_VidSetPinValue(KPD_PORT,LOC_u8KPDColArr[LOC_u8ColumnIndex],HIGH);
	}
	return LOC_u8PressedKey;

}
void KPD_VidInit(void)
{
	DIO_VidSetPinDirection(KPD_PORT,KPD_ROW0_PIN,INPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_ROW1_PIN,INPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_ROW2_PIN,INPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_ROW3_PIN,INPUT);

	DIO_VidSetPinDirection(KPD_PORT,KPD_COL0_PIN,OUTPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_COL1_PIN,OUTPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_COL2_PIN,OUTPUT);
	DIO_VidSetPinDirection(KPD_PORT,KPD_COL3_PIN,OUTPUT);

	DIO_VidSetPortValue(KPD_PORT,0xFF);
}
