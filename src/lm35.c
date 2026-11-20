#include<lpc21xx.h>               // LPC21xx microcontroller register definitions
#include "miniadc.h"              // ADC driver functions (Init_ADC, Read_ADC)

// Reads single-ended LM35 temperature sensor connected to ADC channel 2 (AIN2)
void read_lm35(float *tdegc,float *tdegf)
{
	unsigned short int dval;      // Variable to store raw 10-bit ADC digital value
	int flag;                     // Local initialization flag
	float eAR;                    // Equivalent Analog Reading (voltage in Volts)
	if(flag==0)                   // Ensure ADC is initialized before reading
	{
	Init_ADC();                   // Initialize ADC hardware
	flag=1;
	}
	 Read_ADC(2,&dval,&eAR);      // Read analog voltage from ADC Channel 2
	*tdegc=(eAR*100);             // Convert voltage to Celsius (LM35 outputs 10 mV/°C -> V * 100 = °C)
	*tdegf=(*tdegc*(9/5)+32);     // Convert Celsius temperature to Fahrenheit
}

// Reads differential/non-polarized LM35 configuration using two ADC channels (AIN0 and AIN1)
void read_lm35np(float *tdegc,float *tdegf)
{
	unsigned short int dval1,dval2; // Raw 10-bit digital values for positive and negative sensor outputs
	float eAR1,eAR2;              // Equivalent analog voltages from Channel 0 and Channel 1
	static int flag;              // Static flag so ADC initialization happens only once across calls
	if(flag==0)
	{
		Init_ADC();               // Initialize ADC hardware on first call
		flag=1;
	}
	Read_ADC(0,&dval1,&eAR1);     // Read positive output voltage (+Vout) on ADC Channel 0
	Read_ADC(1,&dval2,&eAR2);     // Read reference/negative output voltage (-Vout) on ADC Channel 1
	*tdegc=((eAR1-eAR2)*100);     // Compute differential voltage and scale by 100 (10 mV/°C) for Celsius
	*tdegf=(*tdegc*(9/5)+32);     // Convert Celsius to Fahrenheit
}
