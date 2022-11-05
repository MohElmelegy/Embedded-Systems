#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

#define DATA_PORT		PORTB
#define CONTROL_PORT	PORTA


void LCD_VidInit(void);
void LCD_VidSendCommand(u8 LOC_u8Command);
void LCD_VidWriteChar(u8 LOC_u8Data);
void LCD_VidWriteString(u8 LOC_u8Data[]);
void LCD_VidWriteNumber(u32 LOC_u8Number);
void LCD_VidSetPosition(u8 LOC_u8Row , u8 LOC_u8Col);
void LCD_VidWriteSpecialChar(u8* Pattern, u8 BlockNumber,u8 Row,u8 Col);





#endif
