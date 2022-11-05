#ifndef	ADC_REGISTER_H_
#define ADC_REGISTER_H_


/********************************************************
 ********************* register *************************
 ********************************************************/
#define ADMUX 	*((volatile u8*)0x27)
#define ADCSRA 	*((volatile u8*)0x26)

#define ADCL 	*((volatile u8*)0x24)
#define ADCH 	*((volatile u8*)0x25)
#define ADC 	*((volatile u16*)0x24)

#define SFIOR 	*((volatile u8*)0x50)



/*********************************************************
 ******************** register's pins ********************
 ********************************************************/
/* ADC Multiplexer Selection Register – ADMUX */
/* Reference selection bits */
#define REFS1		7
#define REFS0		6
/* ADC Left Adjust Result */
#define ADLAR		5
/* Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits */
#define MUX4		4
#define MUX3		3
#define MUX2		2
#define MUX1		1
#define MUX0		0


/* ADC Control and Status Register A – ADCSRA */
/* ADC Enable */
#define ADEN		7
/* ADC Start Conversion */
#define ADSC		6
/* ADC Auto Trigger Enable */
#define ADATE		5
/* ADC Interrupt Flag */
#define ADIF		4
/* ADC Interrupt Enable */
#define ADIE		3
/* Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits */
#define ADPS2		2
#define ADPS1		1
#define ADPS0		0


/* Special FunctionIO Register – SFIOR */
/* Bit 7:5 – ADTS2:0: ADC Auto Trigger Source */
#define ADTS2		7
#define ADTS1		6
#define ADTS0		4
/* Bit 4 – Res: Reserved Bit */
#define RES			5


#endif
