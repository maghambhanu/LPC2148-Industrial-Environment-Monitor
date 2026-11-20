#define TXD0_PIN_FUNC 0x00000001  // PINSEL0 bitmask to configure P0.0 as TXD0 (UART0 Transmit)
#define RXD0_PIN_FUNC 0x00000005  // PINSEL0 bitmask combining P0.0 as TXD0 and P0.1 as RXD0 (UART0 Receive)
#define BAUD 9600                 // Target serial communication baud rate: 9600 bps
#define FOSC 12000000             // External crystal oscillator frequency: 12 MHz
#define CCLK (FOSC*5)             // CPU Clock frequency (CCLK): 60 MHz (12 MHz * 5 PLL multiplier)
#define PCLK (CCLK/4)             // Peripheral Clock frequency (PCLK): 15 MHz (CCLK / 4 default VPB divider)
#define DIVISOR (PCLK/(16 * BAUD)) // Baud rate divisor formula: 15,000,000 / (16 * 9600) = 97

//defines for UxLCR sfr
#define _8BIT             3       // Value (0b11) for 8-bit character word length in Line Control Register
#define WORD_LEN_SEL_BITS 0       // Starting bit position (bits 1:0) for word length select in U0LCR
#define DLAB_BIT          7       // Divisor Latch Access Bit (bit 7 in U0LCR; 1 = access U0DLL/U0DLM)

//defines for UxLSR sfr
#define TEMT_BIT          6       // Transmitter Empty status bit position (bit 6 in U0LSR; 1 = shift reg empty)
#define DR_BIT            0       // Data Ready status bit position (bit 0 in U0LSR; 1 = unread byte in U0RBR)
