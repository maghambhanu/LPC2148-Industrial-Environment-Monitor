#include "types.h"                // Custom data type definitions (u32, etc.)

/* RTC time variables */
extern u32 rhour;                 // External declaration for current/configured hours
extern u32 rmin;                  // External declaration for current/configured minutes
extern u32 rsec;                  // External declaration for current/configured seconds

/* RTC date variables */
extern u32 rdate;                 // External declaration for current/configured day of month
extern u32 rmonth;                // External declaration for current/configured month
extern u32 ryear;                 // External declaration for current/configured year

/* RTC function declarations */
void Init_RTC(void);              // Initializes the LPC214x on-chip RTC and clock prescaler
void RTC_GetTime(void);           // Reads current time (HH:MM:SS) from RTC hardware registers
void RTC_GetDate(void);           // Reads current date (DD/MM/YYYY) from RTC hardware registers
void RTC_SetTime(void);           // Writes updated time values into RTC hardware registers
void RTC_Set_Date(void);          // Writes updated date values into RTC hardware registers
void Set_RTC(void);               // Interactive LCD/keypad menu routine to set & validate time/date
