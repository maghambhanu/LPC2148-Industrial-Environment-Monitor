# 🏭 Industrial Environment Monitoring & Real-Time Data Logger (ARM7 LPC2148)

![MCU](https://img.shields.io/badge/MCU-NXP%20LPC2148%20(ARM7TDMI--S)-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-orange)
![Clock](https://img.shields.io/badge/CCLK-60%20MHz%20%7C%20PCLK%2015%20MHz-green)
![Protocol](https://img.shields.io/badge/Interfaces-ADC%20%7C%20UART0%20%7C%20RTC%20%7C%20EINT1-lightgrey)

A bare-metal embedded firmware system built on the **NXP LPC2148 (ARM7)** microcontroller for continuous industrial temperature monitoring, threshold-based audio alarming, real-time LCD telemetry, and automated serial data logging.

---

## 🎯 Project Objectives
- **Continuous Thermal Monitoring:** Acquire real-time ambient temperature readings using an **LM35** precision analog sensor interfaced to the LPC2148's on-chip 10-bit ADC.
- **User-Defined Threshold Protection:** Allow operators to configure custom temperature alarm limits at boot via a **4×4 Matrix Keypad** and **16×2 LCD** interface.
- **Immediate Audio & Visual Alerting:** Trigger an industrial buzzer (`P0.19`) and display high-visibility `BUZZER ALERT!!` warnings on the LCD alongside urgent serial alert banners whenever temperature exceeds the setpoint.
- **Interrupt-Driven Live Clock Calibration:** Provide non-blocking, hardware-interrupt-driven (`EINT1` on `P0.3`) reconfiguration of the internal **Real-Time Clock (RTC)** date and time without rebooting the system.
- **Automated Serial Data Logging:** Transmit structured, timestamped temperature records every minute over **UART0** (`9600 bps, 8N1`) to a host PC serial terminal for audit and monitoring.

---

## 🔌 System Block Diagram

![System Block Diagram](circuit.jpeg)

---

## 🔄 Firmware Execution Flowchart

![Firmware Flowchart](block_dagram.jpeg)

---

## 📸 Hardware Showcase

<p align="center">
  <img src="hardware_outputs.png" alt="LPC2148 Industrial Environment Monitor Operational States" width="100%">
</p>

- **Buzzer Alert:** Activates audio alarm and displays `BUZZER ALERT!!` on the 16×2 LCD when ambient temperature exceeds the configured setpoint.
- **Date Input:** Interactive keypad sub-menu (`1:DD 2:MM 3:YY 4:ALL 5:BACK 6:E`) for configuring individual or complete RTC date fields.
- **Real-Time Clock:** Displays live RTC date, timestamp, and LM35 temperature reading (`6/11/2026 03:28:50 TM:28°`) during active monitoring.
- **Time Input:** Interactive keypad sub-menu (`1:HH 2:MIN 3:SEC 4:ALL 5:BACK 6:E`) for setting hours, minutes, and seconds.
- **Mode Selection:** Main interrupt-triggered RTC configuration menu (`1:TIME 2:DATE 3:ALL 4:EXIT`).
- **Complete Hardware Setup:** Vector's ARM7 (LPC2148) development board wired to the 16×2 LCD, 4×4 matrix keypad, LM35 temperature sensor, buzzer, and DB9 UART0 serial interface.

---

## 📚 Peripheral Documentation (Beginner's Guide & Technical Details)

Microcontrollers communicate with the outside world through built-in hardware blocks called **peripherals**. Below is each peripheral documented in functional order—from sensor input to user interface, timing, interrupts, serial telemetry, and alarm output.

### 1. LM35 Temperature Sensor & 10-Bit ADC (`P0.29` / `AIN2`)
- **Beginner Concept:** A microcontroller only understands binary numbers (`0`s and `1`s), whereas heat is a continuous analog quantity. The **LM35** converts temperature into a linear voltage (`10 mV` per `1 °C`), and the LPC2148's built-in **ADC (Analog-to-Digital Converter)** translates that voltage into a 10-bit integer (`0` to `1023`).
- **Register & Signal Details:** `P0.29` is configured as `AIN2` via `PINSEL1`. The ADC clock runs at `3 MHz` (`PCLK = 15 MHz`, `CLKDIV = 4`). Reading `ADDR2` (`(ADDR2 >> 6) & 0x3FF`) yields the digital count converted via `Temp (°C) = (ADC_Value × 3.3 / 1023.0) × 100.0`.

![LM35 and ADC Block Diagram](adc_lm35.png)

### 2. 16×2 Alphanumeric LCD (`P0.8–P0.15`, `P0.16–P0.18`)
- **Beginner Concept:** A two-line screen (16 characters per row) that acts as the local dashboard for prompts, menus, live readings, and alarm messages.
- **Register & Signal Details:** Operates in **8-bit parallel mode** using `P0.8–P0.15` (`D0–D7`) for ASCII/command bytes and `P0.16` (`RS`), `P0.18` (`RW`), and `P0.17` (`EN`) for control signaling (`RS = 0` for commands like `0x38`/`0x01`, `RS = 1` for character data).

![16x2 LCD Block Diagram](lcd_16x2.png)

### 3. 4×4 Matrix Keypad (`P1.16–P1.23`)
- **Beginner Concept:** A 16-key grid arranged into **4 rows** and **4 columns** so the microcontroller can read 16 buttons using only 8 GPIO pins instead of 16.
- **Register & Signal Details:** Port 1 pins `P1.16–P1.19` act as row scan outputs and `P1.20–P1.23` act as column sense inputs with software debouncing. Supports numeric entry (`0–9`), `*` for backspace/clear, and `#` for confirmation.

![4x4 Keypad Block Diagram](keypad_4x4.png)

### 4. On-Chip Real-Time Clock (`RTC`)
- **Beginner Concept:** A built-in hardware digital clock inside the LPC2148 chip that tracks seconds, minutes, hours, day, month, and year continuously in the background.
- **Register & Signal Details:** Driven by a `1 Hz` timebase generated from `PCLK = 15 MHz` using prescaler registers `PREINT` and `PREFRAC`, and enabled via `CCR`. Live values are read directly from `SEC`, `MIN`, `HOUR`, `DOM`, `MONTH`, and `YEAR` registers.

![On-Chip RTC Block Diagram](rtc_block.png)

### 5. External Interrupt 1 (`EINT1` on `P0.3`) & VIC
- **Beginner Concept:** An "emergency doorbell" for the processor. Instead of repeatedly checking a button in a loop, the CPU runs normally until the switch is pressed.
- **Register & Signal Details:** `P0.3` is assigned to `EINT1` via `PINSEL0`, configured as falling-edge triggered (`EXTMODE`, `EXTPOLAR`), and registered in the **Vectored Interrupt Controller (`VIC`)**. Triggering `EINT1_ISR` pauses monitoring and launches the `1:TIME 2:DATE 3:ALL 4:EXIT` menu.

![External Interrupt EINT1 Block Diagram](eint1_vic.png)

### 6. UART0 Serial Interface (`TXD0` / `P0.0`, `RXD0` / `P0.1`)
- **Beginner Concept:** A two-wire serial communication channel that streams formatted text reports from the board to a PC screen.
- **Register & Signal Details:** Configured for **9600 bps, 8N1** (`U0LCR = 0x83`, `U0DLL = 97`, `U0DLM = 0` at `15 MHz PCLK`, then `U0LCR = 0x03`). Logic levels are shifted via an onboard **MAX232** transceiver over a DB9 serial cable.

![UART0 Serial Interface Block Diagram](uart0_serial.png)

### 7. Industrial Audio Buzzer (`GPIO` on `P0.19`)
- **Beginner Concept:** A digital alarm output controlled like an electronic ON/OFF switch by a single GPIO pin.
- **Register & Signal Details:** Configured as an output (`IODIR0 |= (1 << 19)`). Driven `HIGH` (`IOSET0 = (1 << 19)`) when `Temperature > Setpoint` and cleared `LOW` (`IOCLR0 = (1 << 19)`) once temperature drops back within safe limits.

![Industrial Audio Buzzer Block Diagram](buzzer_gpio.png)

---

## ⚙️ Overall Working Process

1. **Startup & Peripheral Initialization:** Configures PLL (`CCLK = 60 MHz`, `PCLK = 15 MHz`) and initializes `ADC0`, `16×2 LCD`, `4×4 Keypad`, `RTC`, `EINT1` (`VIC`), `UART0`, and buzzer GPIO (`P0.19`).
2. **Threshold Configuration:** Prompts the user on the LCD to enter the temperature alert setpoint via the keypad (`*` clears digits, `#` confirms) and transmits the initial `SYSTEM CONFIGURATION` header over `UART0`.
3. **Continuous Real-Time Monitoring:** Samples the LM35 voltage via `ADC0`, converts it to Celsius, reads live RTC date/time, and refreshes the 16×2 LCD every second (`DD/MM/YYYY HH:MM:SS TM:XX°`).
4. **Automated 1-Minute Serial Logging:** Whenever `SEC == 00`, transmits a formatted timestamped temperature row over `UART0` to the PC serial terminal.
5. **Threshold Breach & Audio Alarm:** If `Temperature > Setpoint`, asserts `P0.19` `HIGH` to sound the buzzer, displays `BUZZER ALERT!!` on the LCD, and sends an immediate `[!!] TEMPERATURE ALERT` banner over `UART0`.
6. **Interrupt-Driven Live RTC Setup:** Pressing the `EINT1` switch (`P0.3`) pauses monitoring, logs `INTERUPPT RAISED....` on `UART0`, and opens the keypad menu to update `TIME`, `DATE`, or `ALL` fields before resuming.

---

## 📌 Hardware Pinout & Peripheral Mapping

| Peripheral | LPC2148 Pin(s) | Function / Description |
| :--- | :--- | :--- |
| **LM35 Temp Sensor** | `P0.29` (`AIN2`) | Analog voltage input (`10 mV/°C`) |
| **16×2 LCD Data Bus** | `P0.8 – P0.15` | 8-bit parallel data interface (`D0–D7`) |
| **16×2 LCD Control** | `P0.16`, `P0.18`, `P0.17` | `RS`, `RW`, and `EN` control lines |
| **4×4 Keypad Rows** | `P1.16 – P1.19` | Row scan outputs (`ROW0–ROW3`) |
| **4×4 Keypad Columns** | `P1.20 – P1.23` | Column sense inputs (`COL0–COL3`) |
| **UART0 Serial** | `P0.0` (`TXD0`), `P0.1` (`RXD0`) | Serial data logger (`9600 baud`) |
| **External Interrupt** | `P0.3` (`EINT1`) | Edge-triggered interrupt for RTC setup |
| **Alert Buzzer** | `P0.19` | Digital GPIO output (`HIGH` = Alert) |

---

## 🧪 Verification & Test Cases

| Test ID | Test Scenario | Input / Action | Expected System Behavior | Status |
| :--- | :--- | :--- | :--- | :--- |
| **TC-01** | **Boot & Setpoint Configuration** | Power ON board; enter `35` on keypad and press `#` (test `*` backspace) | LCD echoes digits, `*` deletes last digit, `#` saves `35.0 °C` setpoint and prints config banner on `UART0` | ✅ Pass |
| **TC-02** | **Normal Temperature Monitoring** | Ambient temperature `28.0 °C` (`< 35.0 °C` setpoint) | LCD shows live RTC date/time and `TM:28°`; Buzzer remains OFF (`P0.19 = LOW`) | ✅ Pass |
| **TC-03** | **Threshold Breach & Alarm Trigger** | Apply heat to LM35 until reading exceeds `35.0 °C` (e.g., `52.5 °C`) | Buzzer turns ON (`P0.19 = HIGH`), LCD displays `BUZZER ALERT!!`, and `UART0` logs `[!!] TEMPERATURE ALERT` | ✅ Pass |
| **TC-04** | **Alarm Auto-Recovery** | Allow LM35 to cool below `35.0 °C` (e.g., `30.9 °C`) | Buzzer turns OFF (`P0.19 = LOW`) and LCD automatically returns to normal date/time/temperature view | ✅ Pass |
| **TC-05** | **Periodic 1-Minute UART Logging** | Observe `UART0` terminal across minute rollovers (`SEC == 00`) | Formatted `DATE \| TIME \| TEMPERATURE` entry is transmitted once every 60 seconds | ✅ Pass |
| **TC-06** | **Interrupt-Driven RTC Update (`EINT1`)** | Press `EINT1` switch (`P0.3`), select `1:TIME` or `2:DATE`, enter new values | Monitoring pauses, `UART0` logs `INTERUPPT RAISED....`, RTC updates with new values, and monitoring resumes | ✅ Pass |

---

## 🖥️ Sample UART Serial Output
📄 Full captured log available in [`uart_log.txt`](uart_log.txt)

```text
============================================================
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
```

---

## ⚠️ System Limitations
- **Volatile Configuration & Logs:** Temperature setpoint and logged records reside only in SRAM and UART stream output; power loss resets the setpoint and requires an active PC terminal to capture logs.
- **No RTC Battery Backup (`VBAT`):** Without an external `3V` coin cell connected to `VBAT`, the RTC resets upon complete board power disconnection.
- **Single-Zone Sensing:** Monitors only a single analog temperature channel (`AIN2`) without multi-zone or humidity sensing.
- **Wired Telemetry Only:** Relies on a physical DB9 RS-232 serial connection (`UART0`), limiting remote monitoring range.

---

## 🚀 Future Enhancements
- **Non-Volatile Storage (`EEPROM` / `SD Card`):** Interface an I2C EEPROM (`AT24C256`) or SPI microSD module to retain temperature setpoints across reboots and store offline CSV logs.
- **Wireless IoT Telemetry (`ESP8266` / `ESP32` / `GSM`):** Bridge `UART0` to a Wi-Fi or GSM module to push live telemetry to an MQTT/cloud dashboard and send SMS/email alerts during threshold breaches.
- **Multi-Sensor Industrial Expansion:** Utilize additional `ADC0` channels (`AIN0`, `AIN1`, `AIN3`) to monitor humidity (`DHT22`/`HIH-4000`), gas/smoke (`MQ-2`), and multi-zone temperatures simultaneously.
- **Automated Relay Actuation:** Add an opto-isolated relay driver to automatically turn on industrial exhaust fans or cooling units alongside the audio buzzer.
- 
