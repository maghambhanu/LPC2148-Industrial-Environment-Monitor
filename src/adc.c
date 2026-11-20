#include<lpc21xx.h>      // Provides the LPC21xx microcontroller register definitions.
#include"types.h"       // Provides project-specific data types such as u8, u16, and f32.
#include"delay.h"       // Declares the delay_us() function used to allow ADC settling/conversion time.
#include"adc_defines.h"  // Provides ADC bit positions, clock-divider values, and pin-function constants.

void Init_ADC()
{
	// Clear the two PINSEL1 bits that select the function of the relevant ADC pin (P0.29).
	PINSEL1&=~(0x3<<((29-16)*2));

	// Select the pin function defined by AIN2_FUNC so the pin can be used as ADC channel 2.
	// The other channel selections are left commented out; they are not active.
	PINSEL1|=AIN2_FUNC;//|AIN1_FUNC|AIN2_FUNC|AIN3_FUNC;

	// Power up the ADC and configure its conversion clock using the divider in CLKDIV_VAL.
	ADCR|=(1<<PDN_BIT)|(CLKDIV_VAL<<CLKDIV_BITS);
}

void Read_ADC(u8 chNO,u16 *dVal,f32*eAR)
{
	// Clear the lower 8 control bits of ADCR before selecting the channel and starting a conversion.
	ADCR&=0xFFFF00;

	// Set the conversion-start field and select the ADC input channel specified by chNO.
	ADCR|=(1<<ADC_CONV_START_BIT)|(1<<chNO);

	// Wait briefly to allow the ADC input/conversion process to settle.
	delay_us(3);

	// Keep checking the ADC DONE flag until the hardware reports that conversion is complete.
	while(((ADDR>>DONE_BIT)&1)==0);

	// Clear the start bits so the ADC is no longer being commanded to start this conversion.
	ADCR&=~(1<<ADC_CONV_START_BIT);

	// Extract the 10-bit conversion result from ADDR and store it at the caller-provided address.
	*dVal=((ADDR>>DIGITAL_DATA_BITS)&1023);

	// Convert the 10-bit ADC reading (0-1023) into an input voltage using a 3.3 V reference.
	*eAR=*dVal*(3.3/1023);
}
