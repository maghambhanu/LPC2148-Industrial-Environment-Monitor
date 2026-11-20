#include "types.h"                // Custom data type definitions (u8, s8, u32, s32, f32)

/* UART0 initialization */
void UART0_Init(void);            // Configures UART0 pins, frame format (8N1), and baud rate

/* Transmit and receive single character */
void UART0_Tx(u8 sDat);           // Transmits a single byte via UART0
u8 UART0_Rx(void);                // Waits for and receives a single byte via UART0

/* Transmit string */
void U0_TxStr(s8 *s);             // Transmits a null-terminated string via UART0

/* Transmit unsigned and signed integers */
void U0_TxU32(u32 n);             // Transmits a 32-bit unsigned integer as ASCII text
void U0_TxS32(s32 n);             // Transmits a 32-bit signed integer as ASCII text

/* Transmit floating-point value */
void U0_TxF32(f32 fn, u8 nDP);    // Transmits a float formatted to 'nDP' decimal places

/* Receive string */
s8 *U0_RxStr(void);               // Receives an Enter-terminated string with terminal echo
