//KPM.h
#include "types.h"                          // Custom data type definitions
void InitKPM(void);                         // Initializes keypad GPIO row pins as outputs
u32 ColScan(void);                          // Checks if any keypad key is pressed
u32 RowCheck(void);                         // Determines which row has a pressed key
u32 ColCheck(void);                         // Determines which column has a pressed key
u32 KeyScan(void);                          // Waits for a key press/release and returns its ASCII character
u32 ReadNum(void);                          // Reads a multi-digit number until a non-digit key is pressed
unsigned int Get_Number(unsigned int digits); // Reads an exact number of digits with LCD echo and backspace
unsigned int Get_Temp(unsigned int digits);   // Reads a multi-digit temperature setpoint from the keypad
