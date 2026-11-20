#define LCD_DATA 8   // Starting GPIO Port 0 pin for the 8-bit LCD data bus (P0.8 to P0.15 -> D0 to D7)
#define LCD_RS 16    // GPIO pin P0.16 connected to LCD Register Select (RS: 0 = Command, 1 = Data)
#define LCD_RW 18    // GPIO pin P0.18 connected to LCD Read/Write (RW: 0 = Write, 1 = Read)
#define LCD_EN 17    // GPIO pin P0.17 connected to LCD Enable (EN: latches data on falling edge)
