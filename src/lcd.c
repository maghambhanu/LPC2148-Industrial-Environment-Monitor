//lcd.c
#include <LPC21xx.h>              // LPC21xx microcontroller register definitions
#include "lcd_defines.h"          // Standard HD44780 LCD command macros
#include "defines.h"              // Bit manipulation macros
#include "lcd_connection_pins.h"  // GPIO pin mappings for LCD data and control lines
#include "types.h"                // Custom data type definitions (u8, s8, u32, s32, f32)
#include "delay.h"                // Software delay functions

// Low-level routine to send a single byte (command or data) to the 8-bit LCD bus
void WriteLCD(u8 byte)
{
	//select write operation
	IOCLR0=1<<LCD_RW;             // Set RW pin LOW (0) to select Write mode

	//place any byte on data pins d0 to d7
	//IOPIN0&=~(0xFF<<LCD_DATA)|((byte&0xFF)<<LCD_DATA)//WRITEBYTE(IOPIN0,LCD_DATA,byte);

	  IOPIN0=((IOPIN0&~(0xFF<<LCD_DATA))|((0xFF&byte)<<LCD_DATA)); // Clear 8 data bits and write 'byte' onto P0.8–P0.15
	//provide high to low pulse for latching
	IOSET0=1<<LCD_EN;             // Set Enable (EN) pin HIGH to start latch pulse
	delay_us(1);                  // Hold EN HIGH for at least 1 microsecond
	IOCLR0=1<<LCD_EN;             // Set Enable (EN) pin LOW; falling edge latches data into LCD
	delay_ms(2);                  // Wait 2 ms for the LCD controller to finish processing the byte
}

// Sends an instruction/command byte to the LCD controller
void CmdLCD(u8 cmd)
{
	//select cmd register
	IOCLR0=1<<LCD_RS;             // Set RS pin LOW (0) to select the LCD Instruction/Command register
	//write any cmd to lcd
	WriteLCD(cmd);                // Transmit the command byte
}

// Initializes the 16x2 character LCD in 8-bit, 2-line mode using standard HD44780 reset sequence
void InitLCD(void)
{
	//cfg lcd connection gpio output pins
	//WRITEBYTE(IODIR0,LCD_DATA,0xFF);
	//SETBIT(IODIR0,LCD_RS);
	//SETBIT(IODIR0,LCD_RW);
	//SETBIT(IODIR0,LCD_EN);
	IODIR0|=(0xFF<<LCD_DATA)|(1<<LCD_RS)|
	        (1<<LCD_RW)|(1<<LCD_EN); // Configure all 8 data pins and 3 control pins (RS, RW, EN) as GPIO outputs
	//power on delay
	delay_ms(15);                 // Wait >15 ms after Vcc rises to 4.5V for LCD internal power-on reset
	CmdLCD(0x30);                 // First soft-reset / function set command (8-bit interface)
	delay_ms(4);                  // Wait >4.1 ms per datasheet timing
	delay_us(100);
	CmdLCD(0x30);                 // Second function set command
	delay_us(100);                // Wait >100 us per datasheet timing
	CmdLCD(0x30);                 // Third function set command to complete reset sequence
	CmdLCD(MODE_8BIT_2LINE);      // Configure LCD for 8-bit data bus, 2 display lines, 5x7 dot matrix
	CmdLCD(DSP_ON_CUR_OFF);       // Turn display ON and hide cursor
	CmdLCD(CLEAR_LCD);            // Clear entire display and reset cursor to home (0,0)
	CmdLCD(SHIFT_CUR_RIGHT);      // Set entry mode to auto-increment cursor to the right after each character
}

// Displays a single ASCII character at the current cursor position
void CharLCD(u8 asciiVal)
{
	//select data register
	IOSET0=1<<LCD_RS;             // Set RS pin HIGH (1) to select the LCD Data Register (DDRAM)
	//write to ddram via data register
	WriteLCD(asciiVal);           // Write ASCII byte to LCD display RAM
}

// Displays a null-terminated string on the LCD
void strLCD(s8 *s)
{
	while(*s)                     // Loop until null terminator ('\0') is reached
		CharLCD(*s++);            // Print current character and advance pointer
}

// Converts and displays a 32-bit unsigned integer on the LCD
void U32LCD(u32 n)
{
	s32 i=0;	u8 a[10];         // Buffer to store up to 10 decimal digits in reverse order
	if(n==0)
	{
		CharLCD('0');             // Handle zero explicitly
	}
  else
	{
		while(n>0)                // Extract digits from least significant to most significant
		{
			a[i++]=(n%10)+48;     // Convert digit (0-9) to ASCII ('0'-'9') and store in array
			n/=10;
		}
		for(--i;i>=0;i--)         // Print digits in reverse (most significant first)
		{
			CharLCD(a[i]);
		}
	}		
}

// Converts and displays a 32-bit signed integer on the LCD
void S32LCD(s32 n)
{
	if(n<0)                       // If negative, print minus sign and negate value
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);                    // Print magnitude using unsigned integer routine
}

// Displays a floating-point number ('fn') with a specified number of decimal places ('nDP')
void F32LCD(f32 fn,u8 nDP)
{
   u32 n,i;
	 if(fn<0.0)                   // Handle negative sign if value < 0
	 {	 
		 CharLCD('-');
		 fn=-fn;
	 }	 
   n=fn;                          // Extract integer part
   U32LCD(n);                     // Display integer part
   CharLCD('.');                  // Display decimal point
	 for(i=0;i<nDP;i++)           // Loop 'nDP' times to extract fractional digits
	 {
		fn=(fn-n) * 10;           // Shift next fractional digit into integer position
		n=fn;                     // Isolate single digit
    CharLCD(n+48);		          // Convert to ASCII and print
	 }		 
}

// Loads custom character bitmaps into the LCD's Character Generator RAM (CGRAM)
void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	CmdLCD(GOTO_CGRAM_START);     // Point LCD address counter to start of CGRAM (0x40)
	IOCLR0=1<<LCD_RW;             // Select Write mode
	IOSET0=1<<LCD_RS;             // Select Data register
	for(i=0;i<nBytes;i++)         // Write custom glyph pixel rows (8 bytes per 5x8 character)
	{
		//write to cgram via  data reg
		WriteLCD(p[i]);
	}
	//return back ddram
	CmdLCD(GOTO_LINE1_POS0);      // Switch address counter back to DDRAM (Line 1, Pos 0)
}
