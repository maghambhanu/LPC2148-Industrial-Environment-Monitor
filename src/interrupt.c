#include<lpc21xx.h>             // Header file for LPC21xx register definitions
#include"types.h"               // Custom data type definitions (u32, etc.)
#define EINT0_PIN_FUNC 0x000000C0 // PINSEL0 bitmask to configure P0.3 as External Interrupt 1 (EINT1)
extern volatile unsigned char set_time_flag; // Global flag declared in main file to signal an RTC update request
void eint0_isr(void) __irq;     // Interrupt Service Routine (ISR) prototype with ARM __irq keyword
unsigned int count;             // General-purpose counter variable

// Configures external interrupt pin and Vectored Interrupt Controller (VIC)
void init_eint()
{
	PINSEL0&=(u32)~3<<6;        // Clear bits 7:6 in PINSEL0 for P0.3
	PINSEL0|=EINT0_PIN_FUNC;    // Select alternate function (EINT1) on P0.3
	//VICIntSelect=0  its already in 0 by default
	VICIntEnable=1<<15;//14 - channel number // Enable interrupt channel 15 (EINT1) in the VIC
	VICVectCntl0=(1<<5)|15;//(1<<5)for vicvectcnt register to enable // Assign channel 15 to VIC slot 0 and enable slot (bit 5)
	VICVectAddr0=(u32)eint0_isr; // Load the address of the ISR function into VIC Vector Address Register 0
	//EXTINT=0;//SET TO TRIGGER MODE
	EXTMODE=1<<1;//enablng edge triggering // Configure EINT1 (bit 1) to be edge-sensitive instead of level-sensitive
	//EXTPOLAR=1<<1;//enabling falling edge triggering
	//IODIR1=1<<16;//LED PIN; TO RISE INTERRUPT
}

// Interrupt Service Routine executed automatically when the external interrupt triggers
void eint0_isr(void) __irq
{
	//IOPIN1^=1<<16;
	set_time_flag=1;            // Set flag so main loop enters the RTC time-setting routine
	EXTINT=1<<1;                // Clear the EINT1 interrupt flag by writing 1 to bit 1 of EXTINT
	VICVectAddr=0;              // Acknowledge end of interrupt to the VIC hardware
}
