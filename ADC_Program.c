#define F_CPU  8000000
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "ADC_Register.h"
#include "ADC_Config.h"


void ADC_VidInit(void)
{
#if ADC_MOOD == SINGLE_CONVERSION_MOOD
	/* Reference selection bits
	 * Select the voltage reference AVCC at AREF pin */
	CLR_BIT(ADMUX, REFS1);
	SET_BIT(ADMUX, REFS0);
	/* select the Right adjustment*/
	CLR_BIT(ADMUX, ADLAR);

	/* select the prescaler value ----> 64 */
	CLR_BIT(ADCSRA, ADPS0);
	SET_BIT(ADCSRA, ADPS1);
	SET_BIT(ADCSRA, ADPS2);

	/* ADC Enable */
	SET_BIT(ADCSRA, ADEN);

	/* For single conversion mood */
	CLR_BIT(ADCSRA, ADATE);
	SET_BIT(ADCSRA, ADIF);
	CLR_BIT(ADCSRA, ADSC);
	CLR_BIT(ADCSRA, ADIE);

#elif ADC_MOOD == FREE_RUNNING_MOOD
	/* Reference selection bits
	 * Select the voltage reference AVCC at AREF pin*/
	CLR_BIT(ADMUX, REFS1);
	SET_BIT(ADMUX, REFS0);
	/* select the Right adjustment*/
	CLR_BIT(ADMUX, ADLAR);

	/*select the prescaler value ----> 64*/
	SET_BIT(ADCSRA, ADPS2);
	SET_BIT(ADCSRA, ADPS1);
	CLR_BIT(ADCSRA, ADPS0);

	/******* FOR free running mood ********/
	/* to select the source trigger */
	SET_BIT(ADCSRA, ADATE);
	/* enable the ADC to start conversion */
	SET_BIT(ADCSRA, ADEN);
	/* set the interrupt flag */
	CLR_BIT(ADCSRA, ADIF);
	/* select the trigger source as free running mood */
	CLR_BIT(SFIOR, ADTS2);
	CLR_BIT(SFIOR, ADTS1);
	CLR_BIT(SFIOR, ADTS0);
	CLR_BIT(SFIOR, RES);

	/* Start the first conversion in case free running mood*/
	SET_BIT(ADCSRA, ADSC);
#endif
}


u16 ADC_u16GetDigitalValue(u8 Copy_u8Channel)
{
#if ADC_MOOD == SINGLE_CONVERSION_MOOD

	/*to select the channel*/
	ADMUX &= 0b11100000 ;
	ADMUX |= Copy_u8Channel;
	/* Start conversion in case single conversion mood*/
	SET_BIT(ADCSRA, ADSC);
	/* to check if the flag still equal zero the conversion process not ended
	 * pooling on the flag*/
	while(0 == GET_BIT(ADCSRA, ADIF))
	{
		
	}
	/* to clear the flag */
	SET_BIT(ADCSRA, ADIF);
	/* TO return the digital value of the analoge input */
	return ADC;

#elif ADC_MOOD == FREE_RUNNING_MOOD
	/*to select the channel*/
	ADMUX &= 0b11100000 ;
	ADMUX |= Copy_u8Channel;

	/* to check if the flag still equal zero the conversion process not ended
	 * pooling on the flag*/
	while(0 == GET_BIT(ADCSRA, ADIF))
	{

	}
	/* to clear the flag */
	SET_BIT(ADCSRA, ADIF);
	/* TO return the digital value of the analoge input */
	return ADC;
#endif
}
