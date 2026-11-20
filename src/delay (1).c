//delay.c
// Generates an approximate delay in microseconds (us) using a software busy-wait loop
void delay_us(unsigned int dlyUS)
{
	//for(dlyUS*=12;dlyUS>0;dlyUS--);
	dlyUS*=12;        // Scales loop count by 12 (calibrated for a 60 MHz CPU clock)
	while(dlyUS--);   // Decrements counter until it reaches zero to consume CPU cycles
}

// Generates an approximate delay in milliseconds (ms) using a software busy-wait loop
void delay_ms(unsigned int dlyMS)
{
	//for(dlyMS*=12000;dlyMS>0;dlyMS--);
	dlyMS*=12000;     // Scales loop count by 12,000 (~1 ms per unit at 60 MHz CCLK)
	while(dlyMS--);   // Decrements counter until zero
}

// Generates an approximate delay in seconds (s) using a software busy-wait loop
void delay_s(unsigned int dlyS)
{
	//for(dlyMS*=12000000;dlyS>0;dlyS--);
	dlyS*=12000000;   // Scales loop count by 12,000,000 (~1 second per unit at 60 MHz CCLK)
	while(dlyS--);    // Decrements counter until zero
}
