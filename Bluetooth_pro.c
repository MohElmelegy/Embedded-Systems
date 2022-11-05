/*
 *
 *
 *
 *  Author: Mohamed Sami Elmelegy
 */
#define F_CPU  8000000
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "UART_Interface.h"
#include "KEYPAD_Interface.h"
#include "ADC_Interface.h"
#include <avr/io.h>
#include <avr/delay.h>
#define clear 0b00000001
#define         RL_VALUE                     (10)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet

#define         LPG                      (0)         // Gas identity no.
#define         SMOKE                    (1)


float           LPGCurve[3]  = {2.3,0.20,-0.45};   //two points from LPG curve are taken point1:(200,1.6) point2(10000,0.26)
                                                    //take log of each point (lg200, lg 1.6)=(2.3,0.20)  (lg10000,lg0.26)=(4,-0.58)
                                                    //find the slope using these points. take point1 as reference
                                                    //data format:{ x, y, slope};

float           SmokeCurve[3] ={2.3,0.53,-0.43};    //two points from smoke curve are taken point1:(200,3.4) point2(10000,0.62)
                                                    //take log of each point (lg200, lg3.4)=(2.3,0.53)  (lg10000,lg0.63)=(4,-0.20)
                                                    //find the slope using these points. take point1 as reference
                                                    //data format:{ x, y, slope};


float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms


int  GetPercentage(float rs_ro_ratio, float *pcurve);
int GetGasPercentage(float rs_ro_ratio, int gas_id);
float ReadSensor();
float ResistanceCalculation(int raw_adc);
float SensorCalibration();


	void ftoa(float n, char *res, int afterpoint)
	{
		// Extract integer part
		int ipart = (int)n;

		// Extract floating part
		float fpart = n - (float)ipart;

		// convert integer part to string
		int i = itoa(ipart, res, 0);

		// check for display option after point
		if (afterpoint != 0)
		{
			res[i] = '.';  // add dot

			// Get the value of fraction part upto given no.
			// of points after dot. The third parameter is needed
			// to handle cases like 233.007
			fpart = fpart * pow(10, afterpoint);

			itoa((int)fpart, res + i + 1, afterpoint);
		}
	}



void Seperate_Result (u32 u32Result, u8 * u8array_Result)
{

	u8array_Result[0]= ((u32Result / 10000) % 10) + 48;
	u8array_Result[1]= ((u32Result / 1000 ) % 10) + 48;
	u8array_Result[2]= ((u32Result / 100  ) % 10) + 48;
	u8array_Result[3]= ((u32Result / 10   ) % 10) + 48;
	u8array_Result[4]=  (u32Result % 10   ) + 48;

}

int main(void)
{
	//DDRA &=0b00000111;

	u8 Data_in = 0  ;
	char str1;
	u32 Password [3] = {'1', '2', '3'};
	u32 try_Password [3];
	u8 pw1 =1;
	u8 pw2 =2;
	u8 pw3 =3;
	u8 counter =3;
	u8 flag =0;
	u8 flag2 =0;

	u8 SpecialChar_DangerSign[8]={0x01, 0x03, 0x07, 0x0D, 0x0F, 0x05, 0x08, 0x04};
	u8 new_sign[8]={0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00};
	u8 LOC_u8PressedNumber;
	u8 LOC_u8Data[20];
	u16 LOC_u16FirstNumber=0;
	u16 LOC_u16SecondNumber=0;
   u32 LOC_u32Password [3] = {1,2,3};
u32 LOC_u32try_Password [3];
u32 LOC_u32newtry_Password [3];
u32 control_montor;
u32 out_monitor;

u8  LOC_u8Flag=0;
u8 LOC_u8flagerror =0;
u8 LOC_u8flagerror2 =0;
u8 LOC_u8flagerror3 =0;

u16 x1,Volt_Channel0,x2,Volt_Channel1=0;
u8 Celisus=0;

KPD_VidInit();
LCD_VidInit();
UART_Init();
	u8 i=0;
	int adcValue;
	ADC_VidInit();

	DIO_VidSetPinDirection(PORTD, PIN3, OUTPUT);
	DIO_VidSetPinDirection(PORTD, PIN4, OUTPUT);
	DIO_VidSetPinDirection(PORTD, PIN5, OUTPUT);
	DIO_VidSetPinDirection(PORTD, PIN7, OUTPUT);
	DIO_VidSetPinDirection(PORTD, PIN6, OUTPUT);



    while(1)
    {

	LCD_VidSendCommand(clear);
	_delay_ms(2);
	LCD_VidSetPosition(0,0);
	LCD_VidWriteString("SYstem OFF...");
	LCD_VidSetPosition(1,0);
	LCD_VidWriteString("Press start...");
	UART_SendString("System OFF ");
	_delay_ms(1000);

	/*x1=ADC_u16GetDigitalValue(0);
	Volt_Channel0=(x1*5000UL)/1024;
	Celisus=Volt_Channel0/10;
	_delay_ms(50);
	LCD_VidSendCommand(clear);
	_delay_ms(2);
	LCD_VidSetPosition(1,0);
	LCD_VidWriteString("TEMP : ");
	LCD_VidWriteNumber(Celisus);
	_delay_ms(1000);*/

	DIO_VidSetPinValue(PORTA, PIN7, HIGH);
	_delay_ms(1000);
	DIO_VidSetPinValue(PORTA, PIN7, LOW);


	do {
		LOC_u8PressedNumber=KPD_u8GetPressedKey();
	}
	while(KPD_NO_PRESSED_KEY==LOC_u8PressedNumber);



	/*******************************************************************/
	/*To chose Enter password */

	if (LOC_u8PressedNumber== 'a')
	{
		LCD_VidSendCommand(clear);
		_delay_ms(2);
		LCD_VidWriteString("Enter PW: ");
		UART_SendString("\nEnter PW:");


		LOC_u8Flag=0;
		LOC_u8flagerror=0;
		for (u8 i=0; i<3; i++)
		{




			do {
				LOC_u32try_Password [i]=KPD_u8GetPressedKey();
			}
			while(KPD_NO_PRESSED_KEY==LOC_u32try_Password [i]);

			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidSetPosition(0,0);
			LCD_VidWriteString("Enter PW: ");
			for (u8 j=0; j<=i; j++)
			{
				LCD_VidSetPosition(1,j);

				LCD_VidWriteChar('*');
				UART_SendString("*");
			}

			if (LOC_u32try_Password [i] == LOC_u32Password [i] )
			{
				LOC_u8Flag++;

			}


			LOC_u8flagerror++;
		}






		if (LOC_u8Flag == 3)
		{
			_delay_ms(500);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Correct PW");
			UART_SendString("\nCorrect PW");
			_delay_ms(500);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("System ON...");
			UART_SendString("\nSystem ON...");
			_delay_ms(500);
			LOC_u8flagerror3 =1;
		}
		else if (LOC_u8flagerror==3 && LOC_u8flagerror2!=3)
		{
			_delay_ms(500);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Try again");
			UART_SendString("\nTry again");
			_delay_ms(3000);
			LOC_u8flagerror2 ++;
		}

		else if (LOC_u8flagerror2==3)
		{
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Error PW");
			UART_SendString("\nError PW");
			_delay_ms(3000);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Wait...");
			_delay_ms(5000);
			LOC_u8flagerror2=0;

		}

	}



	/*******************************************************************/
	/*To change password*/

	else if (LOC_u8PressedNumber== 'b')
	{
		LCD_VidSendCommand(clear);
		_delay_ms(2);
		LCD_VidWriteString("Enter old PW:");
		UART_SendString("\nEnter old PW");
		LOC_u8Flag=0;
		LOC_u8flagerror=0;

		for (u8 i=0; i<3; i++)
		{



			do {
				LOC_u32try_Password [i]=KPD_u8GetPressedKey();
			}
			while(KPD_NO_PRESSED_KEY==LOC_u32try_Password [i]);

			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidSetPosition(0,0);
			LCD_VidWriteString("Enter old PW: ");
			UART_SendString("\nEnter old PW:");
			for (u8 j=0; j<=i; j++)
			{
				LCD_VidSetPosition(1,j);
				LCD_VidWriteChar('*');
				UART_SendString("*");
			}

			if (LOC_u32try_Password [i] == LOC_u32Password [i] )
			{
				LOC_u8Flag++;

			}


			LOC_u8flagerror++;
		}



		if (LOC_u8Flag == 3)
		{
			_delay_ms(500);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Enter new PW:");
			UART_SendString("\nEnter new PW");

			for (u8 i=0; i<3; i++)
			{


				do {
					LOC_u32newtry_Password [i]=KPD_u8GetPressedKey();
				}
				while(KPD_NO_PRESSED_KEY==LOC_u32newtry_Password [i]);

				LOC_u32Password [i] = LOC_u32newtry_Password [i];

				LCD_VidSetPosition(1,i);
				LCD_VidWriteNumber(LOC_u32Password [i]);



			}

			_delay_ms(2000);
		}
		else if (LOC_u8flagerror==3)
		{
			_delay_ms(500);
			LCD_VidSendCommand(clear);
			_delay_ms(2);
			LCD_VidWriteString("Error PW");
			UART_SendString("\nError PW");
			_delay_ms(3000);
		}



	}


		/********************************************************************/
		                /*System ON*/


if (LOC_u8flagerror3 == 1)
  {

	_delay_ms(500);
	LCD_VidSendCommand(clear);
	_delay_ms(2);
	LCD_VidSetPosition(0,0);
	LCD_VidWriteString("Monitor /Control");
	UART_SendString("\nMonitor / Control");
	LCD_VidSetPosition(1,0);
	LCD_VidWriteString("   1   /     2  ");
	_delay_ms(3000);

	do {
		control_montor=KPD_u8GetPressedKey();
	}
	while(KPD_NO_PRESSED_KEY==control_montor);

if (control_montor== 1)
{
	for(;;)
	{
		LCD_VidSendCommand(clear);
		_delay_ms(2);
		LCD_VidSetPosition(0,0);
		LCD_VidWriteString("LPG:");
		LCD_VidSetPosition(0,13);
		LCD_VidWriteString("PPM");
		LCD_VidSetPosition(1,0);
		LCD_VidWriteString("SMOKE:");
		LCD_VidSetPosition(1,13);
		LCD_VidWriteString("PPM");
		_delay_ms(3000);
	 u16 SM =(GetGasPercentage(ReadSensor()/Ro,SMOKE));
	 LCD_VidSetPosition(0,6);
	 LCD_VidWriteNumber(GetGasPercentage(ReadSensor()/Ro,LPG) );
	 LCD_VidSetPosition(1,7);
	 LCD_VidWriteNumber(GetGasPercentage(ReadSensor()/Ro,SMOKE));
	 _delay_ms(2000);
	 LCD_VidSetPosition(0,6);
	 LCD_VidWriteString("        ");
	 LCD_VidSetPosition(1,7);
	 LCD_VidWriteString("      ");

	 if (SM> 50)
	 {
		 DIO_VidSetPinValue (PORTD, PIN3, HIGH);
		 DIO_VidSetPinValue (PORTD, PIN4, HIGH);
		 SET_BIT (PORTD, 3);
		 SET_BIT (PORTD, 4);
		 LCD_VidSendCommand(clear);
		 _delay_ms(2);
		 LCD_VidSetPosition(0,0);
		 LCD_VidWriteString("Gas leakage");
		 LCD_VidWriteSpecialChar(SpecialChar_DangerSign,0,0,12);
		 _delay_ms(3000);

	 }
	 else
	  CLR_BIT( PORTD, 3);
	  CLR_BIT( PORTD, 4);

	 out_monitor=KPD_u8GetPressedKey();

	 if (out_monitor == 'c')
	 {
		 break;
	 }
	}
}



if (control_montor== 2)
{

		//SET_BIT (PORTD, 5);




			    LCD_VidSendCommand(clear);
			    _delay_ms(2);
				LCD_VidSetPosition(0,0);
				LCD_VidWriteString("Control");
				_delay_ms(600);
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidSetPosition(1 , 5);
				LCD_VidWriteString("Control");
				_delay_ms(600);
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidSetPosition(0 , 9);
				LCD_VidWriteString("Control");
				_delay_ms(600);
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidSetPosition(0 , 5);
				LCD_VidWriteString("Control");
				//LCD_VidWriteSpecialChar(new_sign,0,0,12);
				_delay_ms(2000);

			for (;;)
			{

			Data_in = UART_ReceiveData();
			UART_SendData(Data_in);


			if (Data_in == '1')
			   {


				  TOGGIL_BIT( PORTD , 7);

			   }

			   if (Data_in == '2')
			   {

				   TOGGIL_BIT( PORTD , 6);

			   }


/*********************** Close system *************************************/

			if ((Data_in == 'f') )
			{
				LCD_VidSendCommand(clear);
				_delay_ms(2);
				LCD_VidWriteString("System OFF...");
				UART_SendString("\nSystem OFF...");
				CLR_BIT( PORTD, 5);
				CLR_BIT (PORTD, 7);
				CLR_BIT (PORTD, 6);
				break;
			}

		}


LOC_u8flagerror3 =0;


	}


}


	}
}




/******************************************************************************************************************/




float ResistanceCalculation(int raw_adc)
{                                                         // sensor and load resistor forms a voltage divider. so using analog value and load value
	return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));     // we will find sensor resistor.
}



float SensorCalibration()
{
	int i;                                   // This function assumes that sensor is in clean air.
	float val=0;

	for (i=0;i<50;i++) {                   //take multiple samples and calculate the average value

	val += ResistanceCalculation(ADC_u16GetDigitalValue(0));
	_delay_ms(500);
}
val = val/50;
val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro
//according to the chart in the datasheet

return val;
}


float ReadSensor()
{
  int i;
  float rs=0;

  for (i=0;i<5;i++) {                                 // take multiple readings and average it.
    rs += ResistanceCalculation(ADC_u16GetDigitalValue(0));   // rs changes according to gas concentration.
    _delay_ms(50);
  }

  rs = rs/5;

  return rs;
}


int GetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == LPG ) {
     return GetPercentage(rs_ro_ratio,LPGCurve);

  } else if ( gas_id == SMOKE ) {
     return GetPercentage(rs_ro_ratio,SmokeCurve);
  }

  return 0;
}


int  GetPercentage(float rs_ro_ratio, float *curve)
{                                                                          //Using slope,ratio(y2) and another point(x1,y1) on line we will find
  return (pow(10,( ((log(rs_ro_ratio)-curve[1])/curve[2]) + curve[0])));   // gas concentration(x2) using x2 = [((y2-y1)/slope)+x1]
                                                                          // as in curves are on logarithmic coordinate, power of 10 is taken to convert result to non-logarithmic.
}

