#include <lpc21xx.h>             // Header file for NXP LPC21xx microcontroller register definitions
#include "types.h"               // Custom data type aliases (u8, u16, u32, f32, etc.)
#include "miniadc.h"             // ADC driver functions
#include "lm35.h"                // LM35 temperature sensor driver functions
#include "rtc.h"                 // Real-Time Clock (RTC) driver functions and global time/date variables
#include "minilcd.h"             // Character LCD display driver functions
#include "minikpm.h"             // Keypad Matrix (KPM) driver functions
#include "uart.h"                // UART serial communication driver functions
#include "interrupt.h"           // External interrupt setup and ISR definitions
#include"delay.h"                // Software delay functions
#include"uart_rtc_transmit.h"    // Helper routines for transmitting RTC data over UART

#define BUZZER_PIN (1 << 19)     // Defines GPIO pin P0.19 as the buzzer control pin
//#define SET_TEMP   35
unsigned int prev_sec = 60;      // Tracks previous second value (initialized to 60 so first check triggers)
volatile unsigned char set_time_flag = 0; // Interrupt flag set by ISR when user requests RTC time configuration
float tdegc, tdegf;              // Global variables for temperature in Celsius and Fahrenheit
unsigned int temp;               // Global integer temperature variable

int main(void)
{
  unsigned int temp,set_temp;    // Local variables for integer temperature and user-defined alert threshold
   float tdegc, tdegf;           // Local floating-point temperature readings (Celsius & Fahrenheit)

	unsigned int prev_min = 0;   // Tracks the last minute when data was logged over UART

    // --- Peripheral Initializations ---
    Init_ADC();                  // Initialize Analog-to-Digital Converter for LM35 sensor
    Init_RTC();                  // Initialize Real-Time Clock module
    InitLCD();                   // Initialize 16x2 alphanumeric LCD
    InitKPM();                   // Initialize Keypad Matrix for user input
    UART0_Init();                // Initialize UART0 for serial terminal logging
    init_eint();                 // Initialize External Interrupt (used to trigger RTC time setting)
	UART0_Init();                // Re-initialize UART0
	
	// Print startup banner to UART serial terminal
	U0_TxStr("\r\n");
	U0_TxStr("============================================================\r\n");
	U0_TxStr("       INDUSTRIAL ENVIRONMENT MONITORING SYSTEM\r\n");
	U0_TxStr("          ARM7 LPC2148 | REAL-TIME DATA LOGGER\r\n");
	U0_TxStr("============================================================\r\n");
	U0_TxStr("\r\n");
	
	// Prompt user via LCD and keypad to enter the temperature alert threshold
	CmdLCD(0x01);                // Clear LCD screen
	strLCD("T:set point");       // Display prompt on line 1
	CmdLCD(0xC0);                // Move LCD cursor to line 2
	set_temp = Get_Temp(5);      // Read temperature setpoint from keypad
	CmdLCD(0x01);                // Clear LCD screen

	// Log the configured temperature setpoint and active status over UART
	U0_TxStr(" SYSTEM CONFIGURATION\r\n");
	U0_TxStr(" ------------------------------------------------------------\r\n");
	U0_TxStr(" Temperature Setpoint : ");
	U0_TxF32(set_temp,1);        // Transmit setpoint temperature with 1 decimal place
	U0_TxStr(" ");
	UART0_Tx(0xB0);              // Transmit ASCII/extended degree symbol
    U0_TxStr(" C\r\n");
	U0_TxStr(" Monitoring Status    : ACTIVE\r\n"); 
	U0_TxStr(" ------------------------------------------------------------\r\n");
	delay_ms(1000);              // Wait 1 second before starting main loop
	
    IODIR0 |= BUZZER_PIN;        // Configure P0.19 as an output pin for the buzzer
    IOCLR0 = BUZZER_PIN;         // Ensure buzzer is initially OFF (logic LOW)

    // --- Main Infinite Monitoring Loop ---
    while (1)
    {
        read_lm35(&tdegc, &tdegf); // Read current temperature from LM35 sensor in °C and °F

        RTC_GetTime();           // Fetch current hours, minutes, and seconds from RTC
        RTC_GetDate();           // Fetch current day, month, and year from RTC


        // Check if external interrupt occurred to update RTC time/date
        if (set_time_flag == 1)
        {	
			U0_TxStr("============================================================\r\n");
            U0_TxStr("\r\n");
            U0_TxStr("Time: ");

            // Print current timestamp (HH:MM:SS) with leading zeros before updating
            if (rhour < 10)
                U0_TxStr("0");
            U0_TxU32(rhour);
            U0_TxStr(":");

            if (rmin < 10)
                U0_TxStr("0");
            U0_TxU32(rmin);
            U0_TxStr(":");

            if (rsec < 10)
                U0_TxStr("0");
            U0_TxU32(rsec);

			U0_TxStr(" | ");
			U0_TxStr("INTERUPPT RAISED....");
			delay_ms(500);
            set_time_flag = 0;   // Clear the interrupt flag
            Set_RTC();           // Enter interactive routine to set new RTC time/date via keypad

            RTC_GetTime();       // Refresh time variables after update
            RTC_GetDate();       // Refresh date variables after update
			CmdLCD(0x01);        // Clear LCD display
            prev_sec = 60;       // Reset previous second tracker
            U0_TxStr("\r\n");
			U0_TxStr("Time : ");

            // Print updated timestamp (HH:MM:SS) to UART terminal
            if (rhour < 10)
                U0_TxStr("0");
            U0_TxU32(rhour);
            U0_TxStr(":");

            if (rmin < 10)
                U0_TxStr("0");
            U0_TxU32(rmin);
            U0_TxStr(":");

            if (rsec < 10)
                U0_TxStr("0");
            U0_TxU32(rsec);

			U0_TxStr(" | ");
			U0_TxStr("RTC UPDATED");
			U0_TxStr("\r\n");
			U0_TxStr("============================================================\r\n");
			U0_TxStr("SYSTEM STATUS : MONITORING ACTIVE ");
			U0_TxStr("\r\n");
			U0_TxStr("============================================================\r\n");
        }

        temp = (unsigned int)tdegc; // Convert float Celsius reading to integer for compact LCD display

        // --- LCD Date, Time & Temperature Display ---
		//CmdLCD(0X01);
		CmdLCD(0x80);            // Move cursor to Line 1, Position 0
		//strLCD("D:");
        U32LCD(rdate);           // Display day
        strLCD("/");
        U32LCD(rmonth);          // Display month
        strLCD("/");
        U32LCD(ryear);           // Display year

        //CmdLCD(0x80);
		CmdLCD(0xC0);            // Move cursor to Line 2, Position 0
		//strLCD("T:");
        CharLCD((rhour / 10) + '0'); // Display tens digit of hours
        CharLCD((rhour % 10) + '0'); // Display units digit of hours
        CharLCD(':');

        CharLCD((rmin / 10) + '0');  // Display tens digit of minutes
        CharLCD((rmin % 10) + '0');  // Display units digit of minutes
        CharLCD(':');

        CharLCD((rsec / 10) + '0');  // Display tens digit of seconds
        CharLCD((rsec % 10) + '0');  // Display units digit of seconds

		strLCD("  ");
        strLCD("TM:");
        U32LCD(temp);            // Display current integer temperature
		CharLCD(223);            // Display LCD degree symbol (°)
        strLCD("C");

        
		 //U0_TxStr("RTC LOOP\r\n");

        // --- Buzzer & Over-Temperature Alert Logic ---
		//CUSTOM TEMO FOR BUZZER 
        if (tdegc > set_temp)    // Check if measured temperature exceeds user setpoint
		{
			CmdLCD (0x01);       // Clear LCD screen
			strLCD("BUZZER ALERT!!"); // Show alert warning on LCD
			//CmdLCD (0x01);
			delay_ms(1000);
			CmdLCD(0X01);
			U0_TxStr("\r\n");
			U0_TxStr(" [!!] TEMPERATURE ALERT\r\n");
			U0_TxStr(" ------------------------------------------------------------\r\n");
			U0_TxStr(" Alert Temperature : ");
            U0_TxF32(tdegc, 1);  // Transmit exact alert temperature over UART
			U0_TxStr(" ");
			UART0_Tx(0xB0);
            U0_TxStr(" C\r\n");	
			U0_TxStr(" BUZZER STATUS : ACTIVATED\r\n");
			U0_TxStr(" ------------------------------------------------------------\r\n");		
            IOSET0 = BUZZER_PIN; // Turn ON buzzer (set P0.19 HIGH)
		}
        else
            IOCLR0 = BUZZER_PIN; // Turn OFF buzzer (set P0.19 LOW) when temperature is normal

        // --- Periodic UART Data Logging (Once per minute at 00 seconds) ---
         //if(rsec != prev_sec )
		 /*||
            rdate != prev_date ||
            rmonth != prev_month ||
            ryear != prev_year)	*/
			if(rsec==0&&(rmin!=prev_min)) // Trigger log once every minute when seconds hit 00
			{
			U0_TxStr("\r\n");
			U0_TxStr(" REAL-TIME SENSOR DATA\r\n");
			U0_TxStr(" ------------------------------------------------------------\r\n");
			U0_TxStr(" DATE         TIME          TEMPERATURE\r\n");
			U0_TxStr(" ------------------------------------------------------------\r\n");
            U0_TxU32(rdate);     // Transmit date
            U0_TxStr("/");
            U0_TxU32(rmonth);    // Transmit month
            U0_TxStr("/");
            U0_TxU32(ryear);     // Transmit year

			U0_TxStr("    ");
            if (rhour < 10)
                U0_TxStr("0");
            U0_TxU32(rhour);     // Transmit hours
            U0_TxStr(":");

            if (rmin < 10)
                U0_TxStr("0");
            U0_TxU32(rmin);      // Transmit minutes
            U0_TxStr(":");

            if (rsec < 10)
                U0_TxStr("0");
            U0_TxU32(rsec);      // Transmit seconds
				
 			U0_TxStr("       ");
            U0_TxF32(tdegc, 1);  // Transmit temperature reading with 1 decimal place
			U0_TxStr(" ");
			UART0_Tx(0xB0);
            U0_TxStr(" C\r\n");//*/
			//uart_rtc_transmit();

            prev_sec = rsec;     // Update last logged second
			prev_min=rmin;       // Update last logged minute so it only logs once this minute
           // prev_date = rdate;
            //prev_month = rmonth;
          //  prev_year = ryear;
        }
    }	
}
