

#ifndef	ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

/*******************************************************************/
/***************************Public functions************************/
/*******************************************************************/

/********************************************************************
 * Description:	this function for initialization the ADC
 * Inputs     :	void
 * return     :	void
 * scope      :	public
 **********************************************************************/
void ADC_VidInit(void);

/********************************************************************
 * Description: this function for read the analog value and return it in digital form
 * Inputs     :	u8 -> Copy_u8Channel,	range : ADC0 ~ ADC7   (for single-ended mood)
 * return     :	u16,					range : 1 ~ 1023
 * scope      :	public
 **********************************************************************/
u16 ADC_u16GetDigitalValue(u8 Copy_u8Channel);





#endif
