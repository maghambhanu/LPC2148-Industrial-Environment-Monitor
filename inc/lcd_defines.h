#define	CLEAR_LCD 0x01          // Clears entire display and returns cursor to home position (0,0)
#define	RET_CUR_HOME 0x02       // Returns cursor to home position without clearing display RAM
#define SHIFT_CUR_RIGHT 0x06    // Entry mode: auto-increment cursor right after writing a character
#define SHIFT_CUR_LEFT 0x07     // Entry mode: shift display left on character write
#define DSP_OFF 0x08            // Turns display, cursor, and blink OFF (retains DDRAM contents)
#define DSP_ON_CUR_OFF 0x0C     // Turns display ON with cursor and blink hidden
#define DSP_ON_CUR_ON 0x0E      // Turns display ON with steady underline cursor visible
#define DSP_ON_CUR_BLINK 0x0F   // Turns display ON with blinking block cursor visible
#define SHIFT_DISP_LEFT 0x10    // Shifts cursor position one space to the left
#define SHIFT_DISP_RIGHT 0x14   // Shifts cursor position one space to the right
#define MODE_8BIT_1LINE 0x30    // Function set: 8-bit interface, 1 display line, 5x7 font
#define MODE_4BIT_1LINE 0x20    // Function set: 4-bit interface, 1 display line, 5x7 font
#define MODE_8BIT_2LINE 0x38    // Function set: 8-bit interface, 2 display lines, 5x7 font
#define MODE_4BIT_2LINE 0x28    // Function set: 4-bit interface, 2 display lines, 5x7 font
#define GOTO_LINE1_POS0 0x80    // DDRAM address command for Line 1, Column 0
#define GOTO_LINE2_POS0 0xC0    // DDRAM address command for Line 2, Column 0
#define GOTO_LINE3_POS0 0x94    // DDRAM address command for Line 3, Column 0 (20x4 LCDs)
#define GOTO_LINE4_POS0 0xD4    // DDRAM address command for Line 4, Column 0 (20x4 LCDs)
#define GOTO_CGRAM_START 0x40   // Sets address counter to the start of CGRAM for custom characters
