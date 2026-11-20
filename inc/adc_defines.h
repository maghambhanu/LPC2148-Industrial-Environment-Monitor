// --- Clock Configuration Macros ---
#define FOSC 12000000             // Oscillator frequency: 12 MHz external crystal
#define CCLK (FOSC*5)             // CPU Clock (CCLK): 60 MHz (FOSC multiplied by PLL factor of 5)
#define PCLK (CCLK/4)             // Peripheral Clock (PCLK): 15 MHz (CCLK divided by default VPB divider of 4)
#define ADC_CLK 3000000           // Desired ADC Clock frequency: 3 MHz (must be <= 4.5 MHz for LPC21xx ADCs)

// --- ADCR (A/D Control Register) Bit Definitions ---
#define CLKDIV_BITS  8            // Starting bit position (bits 15:8) for the clock divider value in ADCR
#define PDN_BIT      21           // Power-Down bit position in ADCR (1 = ADC is operational, 0 = power-down mode)
#define ADC_CONV_START_BIT 24     // Starting bit position (bits 26:24) to trigger an A/D conversion
#define CLKDIV_VAL ((PCLK/ADC_CLK)-1) // Formula to calculate the clock divisor needed to scale PCLK down to ADC_CLK

// --- ADDR (A/D Data Register) Bit Definitions ---
#define DIGITAL_DATA_BITA 6       // Starting bit position (bits 15:6) where the 10-bit converted digital result is stored
#define DONE_BIT          31      // Status flag bit position in ADDR (becomes 1 when A/D conversion is complete)

// --- Pin Function Select (PINSEL1) Masks for ADC Channels ---
#define AIN0_FUNC 0x00400000      // Bitmask to configure P0.27 as ADC channel 0 (AIN0)
#define AIN1_FUNC 0x01400000      // Bitmask to configure P0.28 as ADC channel 1 (AIN1)
#define AIN2_FUNC 0x04400000      // Bitmask to configure P0.29 as ADC channel 2 (AIN2)
#define AIN3_FUNC 0x00000000      // Bitmask placeholder for ADC channel 3 (AIN3)
