//KPM.c
#include "types.h"              // Custom data type aliases
#include<lpc21xx.h>             // LPC21xx microcontroller register definitions
#include"types.h"
#include"minikpm_defines.h"     // Pin definitions for keypad rows and columns
#include"minilcd.h"             // LCD driver functions for echoing user keystrokes
#include "delay.h"              // Software delay functions for key debouncing

// 4x4 Matrix Keypad Lookup Table mapping [row][column] indices to ASCII characters
u32 kpmLUT[4][4]={  {'1','2','3','A'},
					{'4','5','6','B'},
					{'7','8','9','C'},
					{'*','0','#','D'}};

// Initializes keypad row pins (P1.16–P1.19) as outputs (columns remain inputs by default)
void InitKPM(void)
{
	IODIR1|=15<<ROW0;           // Set 4 consecutive bits starting at ROW0 as outputs
}

// Checks if any key is currently pressed by reading the 4 column lines
// Returns 0 if a key is pressed (any column pulled LOW), or 1 if all columns are HIGH
u32 ColScan(void)
{
	if(((IOPIN1>>COL0)&15)<15)  // Mask 4 column bits; if value < 0xF, at least one column is LOW
		return 0;
	else
		return 1;
}

// Identifies which row has a pressed key by grounding one row at a time and scanning columns
u32 RowCheck(void)
{
	u32 rno;
	for(rno=0; rno<4; rno++)
	{
		// Drive only row 'rno' LOW (0) while keeping the other 3 rows HIGH (1)
		IOPIN1=(IOPIN1&~(15<<ROW0))|((~(1<<rno))<<ROW0);
		if(ColScan()==0)        // If a column reads LOW, the pressed key is in this row
		break;
	}
	
	IOCLR1=15<<ROW0;            // Reset all row pins back to LOW
	return rno;                 // Return the active row index (0 to 3)
}

// Identifies which column line (0 to 3) is pulled LOW during a key press
u32 ColCheck(void)
{
	u32 cno;
	for(cno=0; cno<4; cno++)
	{
		if(((IOPIN1>>(COL0+cno))&1)==0) // Check if column 'cno' is LOW (0)
			break;
	}
	return cno;                 // Return the active column index (0 to 3)
}

// Blocking function that waits for a complete key press-and-release cycle and returns the character
u32 KeyScan(void)
{
	u32 rno, cno,key;
	//wait for switch press
	while(ColScan());           // Wait until any key is pressed
	//find the rno
	rno=RowCheck();             // Detect active row
	//find the cno
	cno=ColCheck();             // Detect active column
	//get the value from LUT
	key=kpmLUT[rno][cno];       // Look up corresponding ASCII character
	//wait for switch release
	while(!ColScan());          // Wait until key is released
	delay_ms(200);              // Debounce delay to prevent multiple reads from a single press
	return key;
}

// Reads a multi-digit integer from the keypad until any non-numeric key is pressed
u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
	while(1)
	{
		key=KeyScan();
		if(key>='0' && key<='9') // Check if pressed key is a digit ('0'-'9')
		{
			sum=(sum*10)+(key-48); // Convert ASCII digit to integer and append to sum
		}
		else
			break;              // Exit loop on non-digit key
	}
	return sum;
}

// Reads an exact number of 'digits' from the keypad with LCD echo, backspace ('*'), and confirm ('#')
unsigned int Get_Number(unsigned int digits)
{
    unsigned int num = 0;
    unsigned int count = 0;
    unsigned int key;

    while (1)
    {
        key = KeyScan();
		delay_ms(200);
        if (key == '#')         // '#' acts as the Enter/Confirm key
        {
            if (count == digits) // Only accept if exact required digit count is entered
                return num;
        }
        else if (key >= '0' && key <= '9')
        {
            if (count < digits) // Accept digit if maximum length hasn't been reached
            {
                num = (num * 10) + (key - '0'); // Append digit to number
                count++;

                CharLCD(key);   // Display entered digit on LCD
            }
        }
		if((key=='*')&&(num>0)) // '*' acts as Backspace to delete the last entered digit
		{
			num=num/10;         // Remove last digit mathematically
			CmdLCD(0x10);       // Move LCD cursor one position left
			CharLCD(' ');       // Overwrite character with blank space
			CmdLCD(0X10);       // Move cursor left again
			count--;
		}

    }
}

// Reads a temperature setpoint up to 'digits' length (requires at least 2 digits before confirming with '#')
unsigned int Get_Temp(unsigned int digits)
{
    unsigned int num = 0;
    unsigned int count = 0;
    unsigned int key;

    while (1)
    {
        key = KeyScan();
		delay_ms(200);
        if (key == '#')         // '#' confirms temperature entry
        {
            if (count >1)       // Require at least 2 digits before returning
                return num;
        }
        else if (key >= '0' && key <= '9')
        {
            if (count < digits)
            {
                num = (num * 10) + (key - '0'); // Append digit to temperature value
                count++;

                CharLCD(key);   // Echo digit on LCD
            }
        }
		if((key=='*')&&(num>0)) // '*' deletes the last digit (backspace)
		{
			num=num/10;
			CmdLCD(0x10);       // Shift LCD cursor left
			CharLCD(' ');       // Clear digit on display
			CmdLCD(0X10);       // Shift cursor back into position
			count--;
		}

    }
}

/*
#include "types.h"
#include<lpc21xx.h>
#include"types.h"
#include"minikpm_defines.h"
#include"minilcd.h"
#include "delay.h"
u32 kpmLUT[4][4]={  {'1','2','3','A'},
					{'4','5','6','B'},
					{'7','8','9','C'},
					{'*','0','#','D'}};
void InitKPM(void)
{
	IODIR1|=15<<ROW0;
}
u32 ColScan(void)
{
	if(((IOPIN1>>COL0)&15)<15)
		return 0;
	else
		return 1;
}		
u32 RowCheck(void)          
{
	u32 rno;
	for(rno=0;rno<4;rno++)
	{
		IOPIN1&=~(15<<ROW0)|((~(1<<rno))<<ROW0);
		if(ColScan()==0)
		break;
	}
	
	IOCLR1=15<<ROW0;
	return rno;
}	
u32 ColCheck(void)
{
	   u32 cno;
	   for(cno=0;cno<4;cno++)
	   {
	   		if(((IOPIN1>>(COL0+cno))&1)==0)
			break;
		}
		return cno;
}
u32 KeyScan(void)
{
	u32 rno,cno,key;
	while(ColScan());
	rno=RowCheck();
	cno=ColCheck();
	key=kpmLUT[rno][cno];
	while(!ColScan());
	return key;
}
u32 ReadNum(void)
{
	u8 key;
	u32 sum=0;
	while(1)
	{
		key=KeyScan();
		delay_ms(200);
		if(key>='0' && key<='9')
		{
			sum=(sum*10)+(key-48);
		}
		else
			break;
	}
	return sum;
}

unsigned int Get_Number(unsigned int digits)
{
    unsigned int num = 0;
    unsigned int count = 0;
    unsigned int key;

    while (1)
    {
        key = KeyScan();
		delay_ms(200);
        if (key == '#')
        {
            if (count == digits)
                return num;
        }
        else if (key >= '0' && key <= '9')
        {
            if (count < digits)
            {
                num = (num * 10) + (key - '0');
                count++;

                CharLCD(key);
            }
        }
		if((key=='*')&&(num>0))
		{
			num=num/10;
			CmdLCD(0x10);
			CharLCD(' ');
			CmdLCD(0X10);
			count--;
		}
    }
}	 */
