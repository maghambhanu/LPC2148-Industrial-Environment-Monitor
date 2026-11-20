#include <LPC214x.h>              // LPC214x microcontroller register definitions (includes on-chip RTC)
#include "types.h"                // Custom data type definitions (u32, etc.)
#include "rtc.h"                  // RTC function prototypes and external time/date variables
#include"delay.h"                 // Software delay functions
#include"kpm.h"                   // Keypad driver functions (KeyScan, Get_Number)

/* LCD and keypad functions from other files */
extern void CmdLCD(unsigned char cmd);          // External function to send commands to LCD
extern void strLCD(char *str);                  // External function to display strings on LCD
extern unsigned int Get_Number(unsigned int);   // External function to read multi-digit numbers from keypad

/* RTC variables */
u32 rhour, rmin, rsec;            // Global variables holding current/configured hours, minutes, and seconds
u32 rdate, rmonth, ryear;         // Global variables holding current/configured day, month, and year

/* Initialize RTC */
void Init_RTC(void)
{
    CCR = 0x00;                   // Disable Clock Control Register (CCR) before configuring prescaler

    /* Prescaler values for 15 MHz peripheral clock */
    PREINT  = 457;                // Integer portion of RTC prescaler: int(15,000,000 / 32768) - 1 = 456/457
    PREFRAC = 8896;               // Fractional portion of RTC prescaler to generate accurate 32.768 kHz tick

    /* Enable RTC and select RTC clock source */
    CCR = 0x01 | (1 << 4);        // Bit 0 = 1 (Enable RTC counters), Bit 4 = 1 (Select internal PCLK prescaler)
}

/* Read time from RTC */
void RTC_GetTime()
{
    rhour = HOUR;                 // Read current hours (0–23) from RTC HOUR register
    rmin  = MIN;                  // Read current minutes (0–59) from RTC MIN register
    rsec  = SEC;                  // Read current seconds (0–59) from RTC SEC register
}

/* Read date from RTC */
void RTC_GetDate(void)
{
    rdate  = DOM;                 // Read current day of month (1–31) from RTC DOM register
    rmonth = MONTH;               // Read current month (1–12) from RTC MONTH register
    ryear  = YEAR;                // Read current year (0–4095) from RTC YEAR register
}

/* Write time to RTC */
void RTC_SetTime(void)
{
    CCR = 0x00;                   // Temporarily stop RTC clock while updating time registers

    HOUR = rhour;                 // Load new hour value into RTC HOUR register
    MIN  = rmin;                  // Load new minute value into RTC MIN register
    SEC  = rsec;                  // Load new second value into RTC SEC register

    CCR = 0x01 | (1 << 4);        // Re-enable RTC clock and prescaler source
}

/* Write date to RTC */
void RTC_Set_Date(void)
{
    CCR = 0x00;                   // Temporarily stop RTC clock while updating date registers

    DOM   = rdate;                // Load new day of month into RTC DOM register
    MONTH = rmonth;               // Load new month into RTC MONTH register
    YEAR  = ryear;                // Load new year into RTC YEAR register

    CCR = 0x01 | (1 << 4);        // Re-enable RTC clock and prescaler source
}

/* Get and validate date and time */
void Set_RTC(void)
{
//menu
u32 mkey,Tkey,Dkey;               // Variables to store user menu selections from keypad
u32 flag=0;                       // Tracks whether any time/date field was modified
while(1)
{
menu :CmdLCD(0x01);               // Main RTC configuration menu label; clear LCD
	strLCD("MENU");
	CmdLCD(0x01);
	strLCD("1:TIME 2:DATE");      // Display top-level menu options on Line 1
	CmdLCD(0xC0);
	strLCD("3:ALL 4:EXIT");       // Display remaining options on Line 2
	mkey=KeyScan();               // Wait for user's main menu choice
	delay_ms(500);
	if(mkey=='1'||mkey=='3')      // Option 1 (Time only) or Option 3 (Both Time & Date)
	{
		//flag=1;
	TIME_MENU:CmdLCD(0x01);       // Sub-menu for selecting individual time fields
		strLCD("1:HH 2:MIN 3:SEC");
		CmdLCD(0xC0);
	    strLCD("4:ALL 5:BACK 6:EX");
		Tkey=KeyScan();           // Read user's time sub-menu selection
		delay_ms(200);
		//if(mkey=='5')
		//	goto menu;
		if(Tkey=='1'||Tkey=='4')  // Set Hours (or All time fields)
		{
			flag=1;
			HOUR_INPUT:
    			CmdLCD(0x01);
    			strLCD("Enter Hour:");
    			rhour = Get_Number(2); // Read 2-digit hour from keypad
					if (rhour > 23)    // Validate 24-hour format (00–23)
    					{
        					CmdLCD(0x01);
        					strLCD("Invalid Hour");
							delay_ms(500);
        					goto HOUR_INPUT; // Re-prompt if invalid
    					}
		}
		if(Tkey=='2'||Tkey=='4')  // Set Minutes (or All time fields)
		{
				flag=1;
				MIN_INPUT:
			    	CmdLCD(0x01);
    				strLCD("Enter Minute:");
    				rmin = Get_Number(2); // Read 2-digit minute from keypad

    				if (rmin > 59)        // Validate minutes (00–59)
    				{
        				CmdLCD(0x01);
        				strLCD("Invalid Minute");
						delay_ms(500);
        				goto MIN_INPUT;   // Re-prompt if invalid
    				}
		}
		if(Tkey=='3'||Tkey=='4')  // Set Seconds (or All time fields)
		{		 
				flag=1;
				SEC_INPUT:
				    CmdLCD(0x01);
    				strLCD("Enter Second:");

    				rsec = Get_Number(2); // Read 2-digit second from keypad

    					if (rsec > 59)    // Validate seconds (00–59)
    					{
        					CmdLCD(0x01);
        					strLCD("Invalid Second");
							delay_ms(500);
        					goto SEC_INPUT; // Re-prompt if invalid
    					}
		}
		
		if(Tkey=='5')             // Option 5: Return to main menu
			goto menu;
		if(Tkey>'6')              // Handle invalid keypad input
		{
			CmdLCD(0x01);
			strLCD("INVALID KEY");
			delay_ms(500);
			goto TIME_MENU;
		}
		RTC_SetTime();            // Write validated time values into RTC hardware registers
		if(flag==1)
		{
			CmdLCD(0x01);
		 	strLCD("TIME UPDATED"); // Confirm time update on LCD
			delay_ms(500);
		}
		if(Tkey=='6')             // Option 6: Exit Set_RTC completely
			break;
		if(mkey!='3')             // Stay in TIME_MENU unless configuring ALL (mkey == '3')
			goto TIME_MENU;		
	 }
	 if(mkey=='2'||mkey=='3')     // Option 2 (Date only) or Option 3 (Both Time & Date)
	 {
	 	//flag=1;
	 	DATE_MENU:CmdLCD(0x01);   // Sub-menu for selecting individual date fields
			strLCD("1:DD 2:MM 3:YY");
			CmdLCD(0xC0);
	    	strLCD("4:ALL 5:BACK 6:EX");
			Dkey=KeyScan();       // Read user's date sub-menu selection
			delay_ms(200);
			if(Dkey=='1'||Dkey=='4') // Set Day of Month (or All date fields)
			{
				flag=1;
				// Enter date first 
				DATE_INPUT:
    				CmdLCD(0x01);
    				strLCD("Enter Date:");
    				rdate = Get_Number(2); // Read 2-digit day

    				if (rdate < 1 || rdate > 31) // Validate basic day range (1–31)
    				{
        				CmdLCD(0x01);
        				strLCD("Invalid Date");
						delay_ms(500);
        				goto DATE_INPUT;
    				}
			}

			if(Dkey=='2'||Dkey=='4') // Set Month (or All date fields)
			{
				flag=1;
				// Enter month next 
				MONTH_INPUT:
    				CmdLCD(0x01);
    				strLCD("Enter Month:");
    				rmonth = Get_Number(2); // Read 2-digit month

    					if (rmonth < 1 || rmonth > 12) // Validate month range (1–12)
    					{
        					CmdLCD(0x01);
        					strLCD("Invalid Month");
							delay_ms(500);
        					goto MONTH_INPUT;
    					}

    					// February: accept dates 1 to 28 
    					if (rmonth == 2) // Check February max day limit
						{
   					 		if (rdate > 28)
        					{
            					CmdLCD(0x01);
            					strLCD("Invalid Date");
								delay_ms(500);
            					goto DATE_INPUT;
        					}
    					}
    		      	// Months having 30 days 
    				if (rmonth == 4 || rmonth == 6 ||
        				rmonth == 9 || rmonth == 11) // Check 30-day months (Apr, Jun, Sep, Nov)
    				{
        				if (rdate > 30)
        				{
            				CmdLCD(0x01);
            				strLCD("Invalid Date");
							delay_ms(500);
            				goto DATE_INPUT;
        				}
    				}
			}
			if(Dkey=='3'||Dkey=='4') // Set Year (or All date fields)
			{
				flag=1;
				// Enter year 
				YEAR_INPUT:
    				CmdLCD(0x01);
    				strLCD("Enter Year:");
    				ryear = Get_Number(4); // Read 4-digit year

    				if (ryear < 2000 || ryear > 2099) // Validate year range (2000–2099)
    				{
        				CmdLCD(0x01);
        				strLCD("Invalid Year");
						delay_ms(500);
        				goto YEAR_INPUT;
    				}
			 }
		if(Dkey=='5')             // Option 5: Return to main menu
			goto menu;
		if(Dkey>'6')              // Handle invalid keypad input
		{
			CmdLCD(0x01);
			strLCD("INVALID KEY");
			delay_ms(500);
			goto DATE_MENU;
		}
		RTC_Set_Date();           // Write validated date values into RTC hardware registers
		if(flag==1)
		{
			CmdLCD(0x01);
			strLCD("DATE UPDATED"); // Confirm date update on LCD
			delay_ms(500);
		}
		if(Dkey=='6')             // Option 6: Exit Set_RTC completely
			break;
		if(mkey!='3')             // Stay in DATE_MENU unless configuring ALL
			goto DATE_MENU;
		else
			break;                // Exit after updating both Time and Date in 'ALL' mode
	}

if(mkey=='4')                     // Option 4 from main menu: Exit RTC settings
		break;
if(flag==1)
{
	CmdLCD(0X01);
	strLCD("RTC UPDATED");        // Final confirmation message on LCD
	delay_ms(500);
}
if(mkey>'4')                      // Handle invalid main menu selection
{
	CmdLCD(0x01);
	strLCD("INVALID KEY");
	delay_ms(500);
	goto menu;
}
}
}

/*
// Enter date first 
DATE_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Date:");
    rdate = Get_Number(2);

    if (rdate < 1 || rdate > 31)
    {
        CmdLCD(0x01);
        strLCD("Invalid Date");
        goto DATE_INPUT;
    }

// Enter month next 
MONTH_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Month:");
    rmonth = Get_Number(2);

    if (rmonth < 1 || rmonth > 12)
    {
        CmdLCD(0x01);
        strLCD("Invalid Month");
        goto MONTH_INPUT;
    }

    // February: accept dates 1 to 28 
    if (rmonth == 2)
    {
        if (rdate > 28)
        {
            CmdLCD(0x01);
            strLCD("Invalid Date");
            goto DATE_INPUT;
        }
    }

    // Months having 30 days 
    if (rmonth == 4 || rmonth == 6 ||
        rmonth == 9 || rmonth == 11)
    {
        if (rdate > 30)
        {
            CmdLCD(0x01);
            strLCD("Invalid Date");
            goto DATE_INPUT;
        }
    }

// Enter year 
YEAR_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Year:");
    ryear = Get_Number(4);

    if (ryear < 2000 || ryear > 2099)
    {
        CmdLCD(0x01);
        strLCD("Invalid Year");
        goto YEAR_INPUT;
    }

// Enter hour 
HOUR_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Hour:");
    rhour = Get_Number(2);

    if (rhour > 23)
    {
        CmdLCD(0x01);
        strLCD("Invalid Hour");
        goto HOUR_INPUT;
    }

// Enter minute 
MIN_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Minute:");
    rmin = Get_Number(2);

    if (rmin > 59)
    {
        CmdLCD(0x01);
        strLCD("Invalid Minute");
        goto MIN_INPUT;
    }

// Enter second 
SEC_INPUT:
    CmdLCD(0x01);
    strLCD("Enter Second:");

    rsec = Get_Number(2);

    if (rsec > 59)
    {
        CmdLCD(0x01);
        strLCD("Invalid Second");
        goto SEC_INPUT;
    }
	
// Update RTC registers 
    RTC_SetTime();
    RTC_Set_Date();

    CmdLCD(0x01);
    strLCD("RTC Updated");
}	*/
