🏭 Industrial Environment Monitoring & Real-Time Data Logger (ARM7 LPC2148)A bare-metal embedded firmware system built on the NXP LPC2148 (ARM7) microcontroller for continuous industrial temperature monitoring, threshold-based audio alarming, live LCD telemetry, and automated serial data logging.🔌 System Block Diagram🔄 Firmware Execution Flowchart✨ Key FeaturesPrecision Analog Temperature Sensing: Interfaces with an LM35 temperature sensor (10 mV/°C) through the 10-bit on-chip ADC (AIN2 / P0.29 at 3 MHz ADC clock).Interactive Setpoint Configuration: Uses a 4×4 Matrix Keypad and 16×2 Alphanumeric LCD (supporting * for backspace and # for confirmation) to configure the high-temperature threshold at boot.Interrupt-Driven RTC Reconfiguration: Pressing the external interrupt push button (EINT1 on P0.3) pauses normal monitoring and opens an interactive LCD/keypad menu to update Time (HH:MM:SS) and Date (DD/MM/YYYY) live.Threshold-Based Audio Alarming: Activates an industrial buzzer on P0.19, flashes BUZZER ALERT!! on the LCD, and transmits an instant [!!] TEMPERATURE ALERT warning over UART whenever temperature exceeds the setpoint.Automated UART0 Data Logger: Transmits formatted timestamped temperature logs to a PC serial terminal (9600 bps, 8N1) once every minute (rsec == 0).📌 Hardware Pinout & Peripheral MappingPeripheral / ModuleLPC2148 Pin(s)Function / DescriptionLM35 Temp SensorP0.29 (AIN2)Analog temperature voltage input (10 mV/°C)16×2 LCD Data BusP0.8 – P0.158-bit parallel data interface (D0–D7)16×2 LCD ControlP0.16, P0.18, P0.17RS (Register Select), RW (Read/Write), EN (Enable)4×4 Keypad RowsP1.16 – P1.19Row scan outputs (ROW0–ROW3)4×4 Keypad ColumnsP1.20 – P1.23Column sense inputs (COL0–COL3)UART0 SerialP0.0 (TXD0), P0.1 (RXD0)Serial terminal data logger (9600 baud, 8N1)Push ButtonP0.3 (EINT1)Edge-triggered external interrupt for RTC setupAlert BuzzerP0.19Digital GPIO output (HIGH = Alert Active)🖥️ Sample UART Serial Terminal Output📄 Full captured terminal log: uart_log.txt============================================================
       INDUSTRIAL ENVIRONMENT MONITORING SYSTEM
          ARM7 LPC2148 | REAL-TIME DATA LOGGER
============================================================

 SYSTEM CONFIGURATION
 ------------------------------------------------------------
 Temperature Setpoint : 35.0 ° C
 Monitoring Status    : ACTIVE
 ------------------------------------------------------------

 [!!] TEMPERATURE ALERT
 ------------------------------------------------------------
 Alert Temperature : 52.5 ° C
 BUZZER STATUS : ACTIVATED
 ------------------------------------------------------------

 REAL-TIME SENSOR DATA
 ------------------------------------------------------------
 DATE         TIME          TEMPERATURE
 ------------------------------------------------------------
 10/11/2026    03:55:00       30.9 ° C
============================================================

Time: 03:55:08 | INTERUPPT RAISED....
Time : 03:55:20 | RTC UPDATED
============================================================
SYSTEM STATUS : MONITORING ACTIVE 
============================================================

 REAL-TIME SENSOR DATA
 ------------------------------------------------------------
 DATE         TIME          TEMPERATURE
 ------------------------------------------------------------
 10/11/2026    03:56:00       27.7 ° C
🛠️ Build & Simulation InstructionsOpen Keil µVision and create a new project targeting NXP LPC2148.Add all .c source files to your Source Group and add the header directory to Options for Target → C/C++ → Include Paths.Enable Create HEX File under Options for Target → Output and build the project (F7).Load the generated .hex file into Proteus ISIS (with a 12 MHz crystal and Virtual Terminal set to 9600 baud) or flash it onto an LPC2148 development board using Flash Magic.
