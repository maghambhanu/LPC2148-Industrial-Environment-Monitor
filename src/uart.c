//uart.c
#include <LPC21xx.h>              // LPC21xx microcontroller register definitions
#include "types.h"                // Custom data type definitions (u8, s8, u32, s32, f32)
#include "uart_defines.h"         // UART0 configuration macros (baud rate, pin functions, bit positions)

// Initializes UART0 peripheral with 8-bit data, no parity, 1 stop bit (8N1) at the configured baud rate
void UART0_Init(void) 
{
	 //cfg p0.0,p0.1 pins as
	 //TXD0 & RXD0 pins respectively
	 PINSEL0&=~((3<<2)|(3<<0));   // Clear function select bits for P0.0 (bits 1:0) and P0.1 (bits 3:2)
	 PINSEL0|=TXD0_PIN_FUNC|RXD0_PIN_FUNC; // Configure P0.0 as TXD0 and P0.1 as RXD0
	 //cfg UxLCR for 8N1,&DLAB sets
	 U0LCR=((1<<DLAB_BIT)|        // Set DLAB = 1 to unlock divisor latch registers (U0DLL/U0DLM)
	        (_8BIT<<WORD_LEN_SEL_BITS)); // Set 8-bit character length, 1 stop bit, no parity
	 //cfg baudrate
   U0DLL=DIVISOR;                 // Load lower 8 bits of the baud rate divisor
   U0DLM=DIVISOR>>8;              // Load upper 8 bits of the baud rate divisor
   //reset/clear DLAB bit
   U0LCR&=~(1<<DLAB_BIT); 	      // Clear DLAB = 0 to lock divisor latches and enable normal THR/RBR access
}

// Transmits a single byte over UART0 and blocks until transmission completes
void UART0_Tx(u8 sDat) 
{ 
   //write to UxTHR,serialization begins
   U0THR=sDat;                    // Load byte into Transmit Holding Register (starts serial shifting)
   //wait until serialization out completes
   while(((U0LSR>>TEMT_BIT)&1)==0); // Poll Transmitter Empty (TEMT) flag in Line Status Register until 1
} 

// Waits for and reads a single received byte from UART0
u8 UART0_Rx(void) 
{ 
    //wait until reception status observed
	  while(((U0LSR>>DR_BIT)&1)==0); // Poll Data Ready (DR) flag in Line Status Register until a byte arrives
	  //read & return recvd byte
	  return U0RBR;               // Read and return the received byte from Receiver Buffer Register
} 

// Transmits a null-terminated ASCII string over UART0
void U0_TxStr(s8 *s)
{
	while(*s)                     // Loop until null terminator ('\0') is reached
		UART0_Tx(*s++);           // Send current character and advance string pointer
}

// Converts and transmits a 32-bit unsigned integer as ASCII digits over UART0
void U0_TxU32(u32 n)
{
	s32 i=0;	u8 a[10];         // Buffer to store up to 10 extracted decimal digits
	if(n==0)
	{
		UART0_Tx('0');            // Handle zero explicitly
	}
  else
	{
		while(n>0)                // Extract digits from least significant to most significant
		{
			a[i++]=(n%10)+48;     // Convert digit (0-9) to ASCII ('0'-'9')
			n/=10;
		}
		for(--i;i>=0;i--)         // Transmit digits in reverse (most significant digit first)
		{
			UART0_Tx(a[i]);
		}
	}		
}

// Converts and transmits a 32-bit signed integer over UART0
void U0_TxS32(s32 n)
{
	if(n<0)                       // If negative, transmit minus sign and negate value
	{
		UART0_Tx('-');
		n=-n;
	}
	U0_TxU32(n);                  // Transmit magnitude using unsigned integer function
}

// Transmits a floating-point number ('fn') with a specified number of decimal places ('nDP')
void U0_TxF32(f32 fn,u8 nDP)
{
   u32 n,i;
	 if(fn<0.0)                   // Handle negative sign if value < 0
	 {	 
		 UART0_Tx('-');
		 fn=-fn;
	 }	 
   n=fn;                          // Extract integer part
   U0_TxU32(n);                   // Transmit integer part
   UART0_Tx('.');                 // Transmit decimal point
	 for(i=0;i<nDP;i++)           // Loop 'nDP' times to extract and send fractional digits
	 {
		fn=(fn-n)*10;             // Shift next fractional digit into integer position
		n=fn;                     // Isolate single digit
    UART0_Tx(n+48);		          // Convert to ASCII and transmit
	 }		 
}

// Receives a newline/carriage-return terminated string from UART0 with local echo
s8 * U0_RxStr(void)
{
	static s8 rBuff[100];         // Static receive buffer (persists after function returns)
  s8 t,i=0;
  while(1)
	{
		t=UART0_Rx();             // Wait for and read incoming character
		rBuff[i]=t;               // Store character in buffer
		UART0_Tx(t);              // Echo received character back to sender terminal
		if((t=='\n')||(t=='\r')) // Check for Enter key (Line Feed or Carriage Return)
		{
			rBuff[i]='\0';        // Replace newline/CR with null terminator
			break;
		}
		i++;
  }
	U0_TxStr("\n\r");             // Send newline + carriage return to terminal
  return rBuff;	                  // Return pointer to received string buffer
}
