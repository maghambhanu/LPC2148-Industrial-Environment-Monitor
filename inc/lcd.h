//lcd.h

#include "types.h"                        // Custom data type definitions
void WriteLCD(u8 byte);                   // Sends a raw byte (command or data) to the LCD data bus
void CmdLCD(u8 cmd);                      // Sends a control command to the LCD instruction register
void InitLCD(void);                       // Initializes the LCD hardware and display settings
void CharLCD(u8 asciiVal);                // Prints a single ASCII character on the LCD
void strLCD(s8 *);                        // Prints a null-terminated string on the LCD
void U32LCD(u32);                         // Prints a 32-bit unsigned integer on the LCD
void S32LCD(s32);                         // Prints a 32-bit signed integer on the LCD
void F32LCD(f32,u8 nDP);                  // Prints a floating-point number with 'nDP' decimal places
void BuildCGRAM(const u8 *p,u8 nBytes);   // Writes custom character patterns into LCD CGRAM
