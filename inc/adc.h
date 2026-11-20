#include"types.h" // Includes custom data type definitions (such as u8, u16, f32) used in the function prototypes

// Function prototype to initialize and configure the Analog-to-Digital Converter (ADC) hardware
void Init_ADC(void);

// Function prototype to read an analog value from a specific channel (chNO), 
// storing the raw 10-bit digital result in 'dVal' and the calculated equivalent analog voltage in 'eAR'
void Read_ADC(u8 chNO,u16 *dVal,f32*eAR);
