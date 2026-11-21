# 🏭 Industrial Environment Monitoring System Using LPC2148

![Embedded Systems](https://img.shields.io/badge/Domain-Embedded%20Systems-blue)
![Microcontroller](https://img.shields.io/badge/Controller-NXP%20LPC2148%20(ARM7TDMI--S)-orange)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green)
![Clock](https://img.shields.io/badge/Clock-CCLK%2060%20MHz%20%7C%20PCLK%2015%20MHz-red)
![Protocols](https://img.shields.io/badge/Interfaces-ADC%20%7C%20UART0%20%7C%20RTC%20%7C%20EINT1-lightgrey)

---

## 1. Project Overview

The **Industrial Environment Monitoring System** is a bare-metal embedded systems project developed using the **NXP LPC2148 ARM7** microcontroller.

The primary goal of this project is to continuously monitor ambient industrial temperature, display real-time temperature alongside live calendar date and time on a **16×2 LCD**, trigger an audible **buzzer alert** upon threshold violations, and log structured telemetry reports to a host PC via **UART0**.

An **LM35** precision analog sensor measures surrounding temperature (`10 mV/°C`), and the LPC2148's inbuilt **10-bit ADC** converts the analog voltage into a digital value. At startup, the operator sets a custom temperature threshold using a **4×4 matrix keypad** (with `*` backspace and `#` confirmation). During normal operation, pressing an **External Interrupt (`EINT1`)** switch pauses monitoring and opens an interactive keypad menu to modify the internal **Real-Time Clock (RTC)** date and time (`HH:MM:SS` and `DD/MM/YYYY`).

When measured temperature exceeds the configured setpoint (e.g., `35.0 °C`), the firmware activates an industrial buzzer on `P0.19`, displays `BUZZER ALERT!!` on the LCD, and transmits an immediate `[!!] TEMPERATURE ALERT` banner over `UART0`.

---

## 2. Project Objectives

- Monitor industrial environmental temperature continuously in real time.
- Measure ambient temperature using an **LM35** analog sensor (`10 mV/°C`).
- Convert analog sensor output into a 10-bit digital value using the LPC2148 inbuilt **ADC0**.
- Allow custom temperature threshold configuration at startup using a **4×4 Matrix Keypad**.
- Display live temperature, date, time, and alert messages on a **16×2 Alphanumeric LCD**.
- Maintain accurate wall-clock time and calendar date using the on-chip **RTC**.
- Use **External Interrupt 1 (`EINT1`)** on `P0.3` to enter the interactive RTC configuration menu without polling overhead.
- Activate an industrial **Buzzer** (`P0.19`) whenever measured temperature exceeds the configured setpoint.
- Transmit 1-minute periodic telemetry tables and instant alert reports via **UART0** (`9600 bps, 8N1`).

---

## 3. Hardware Components

| Component | Purpose |
| :--- | :--- |
| **NXP LPC2148 (ARM7TDMI-S)** | Main 32-bit microcontroller (`60 MHz` CCLK, `15 MHz` PCLK) |
| **LM35 Sensor** | Precision analog temperature measurement (`10 mV/°C`) |
| **Inbuilt 10-Bit ADC (`ADC0`)** | Analog-to-digital voltage conversion on `P0.29` (`AIN2`) |
| **16×2 Alphanumeric LCD** | Live local display for telemetry, setup menus, and alerts |
| **On-Chip RTC** | Hardware real-time clock and calendar (`1 Hz` prescaled timebase) |
| **4×4 Matrix Keypad** | User input for temperature setpoint and RTC date/time editing |
| **Push Button (`EINT1`)** | Hardware external interrupt trigger on `P0.3` for RTC setup |
| **Industrial Buzzer** | Audible alarm driven via `P0.19` during threshold breaches |
| **UART0 & MAX232** | RS-232 level-shifted serial telemetry to PC (`9600 baud`) |

---

## 4. Software Requirements

- **IDE & Compiler:** Keil µVision (ARMCC)
- **Programming Language:** Bare-Metal Embedded C
- **Target Hardware:** Vector's Advanced Development Board for ARM7 (`LPC2148`)
- **Serial Monitor:** PC Serial Terminal (`9600 bps, 8N1`, no flow control)
- **Flash Utility:** Flash Magic (ISP programming via UART0)

---

## 5. System Architecture

### 5.1 System Block Diagram

![System Block Diagram](docs/circuit.jpeg)

### 5.2 Interactive Architecture Flowchart

```mermaid
flowchart TD
    A[LM35 Temperature Sensor] -->|10 mV/°C Analog| B[10-Bit ADC0 - P0.29]
    B --> C[LPC2148 ARM7 Microcontroller]
    D[On-Chip RTC] <-->|Date & Time| C
    E[4x4 Matrix Keypad] -->|Threshold & RTC Input| C
    F[External Interrupt EINT1 - P0.3] -->|VIC IRQ| C
    C -->|8-Bit Parallel| G[16x2 Alphanumeric LCD]
    C -->|9600 bps TXD0| H[UART0 + MAX232]
    H -->|DB9 Cable| I[PC Serial Terminal]
    C -->|GPIO P0.19| J[Industrial Buzzer]
```

---

## 6. Hardware Showcase

![Hardware Showcase](docs/hardware_outputs.png)

- **Buzzer Alert:** Displays `BUZZER ALERT!!` on the 16×2 LCD and activates the buzzer when temperature exceeds the setpoint.
- **Date Input:** Keypad sub-menu (`1:DD 2:MM 3:YY 4:ALL 5:BACK 6:E`) for updating individual or full RTC date fields.
- **Real-Time Clock:** Normal monitoring screen displaying live date, time, and temperature (`6/11/2026 03:28:50 TM:28°`).
- **Time Input:** Keypad sub-menu (`1:HH 2:MIN 3:SEC 4:ALL 5:BACK 6:E`) for updating hours, minutes, and seconds.
- **Mode Selection:** Interrupt-triggered RTC setup menu (`1:TIME 2:DATE 3:ALL 4:EXIT`).
- **Complete Hardware Setup:** Vector's ARM7 (LPC2148) development board wired to the 16×2 LCD, 4×4 matrix keypad, LM35 sensor, buzzer, and DB9 UART0 cable.

---

## 7. Peripheral Interfacing and Working

### 7.1 LPC2148 ARM7 Microcontroller

The **NXP LPC2148** is a 32-bit ARM7TDMI-S microcontroller acting as the central controller of the system. Configured via the Phase-Locked Loop (PLL), it runs at a `60 MHz` CPU clock (`CCLK`) and `15 MHz` peripheral clock (`PCLK`).

**Main responsibilities:**
- Sample analog sensor voltage through `ADC0` and compute temperature in °C.
- Read and update calendar date and time registers in the internal `RTC`.
- Drive the `16×2 LCD` in 8-bit parallel mode.
- Scan the `4×4 Matrix Keypad` for threshold entry and clock settings.
- Service `EINT1` hardware interrupts through the Vectored Interrupt Controller (`VIC`).
- Transmit periodic telemetry and alert banners over `UART0`.
- Control the industrial buzzer on `P0.19`.

---

### 7.2 LM35 Temperature Sensor

The **LM35** is a precision integrated-circuit temperature sensor whose output voltage is linearly proportional to the Celsius temperature (`10 mV/°C`).

**Important characteristics:**
- Linear analog output (`10 mV/°C` scale factor).
- Does not require external calibration.
- Directly compatible with the LPC2148's `0–3.3 V` analog input range.

**Example output values:**

| Ambient Temperature | LM35 Output Voltage |
| :---: | :---: |
| `25 °C` | `250 mV` |
| `28 °C` | `280 mV` |
| `35 °C` | `350 mV` |
| `52.5 °C` | `525 mV` |

**LM35 interfacing block diagram:**

```mermaid
flowchart LR
    A[Ambient Heat] --> B[LM35 Sensor - 10 mV/°C]
    B -->|Analog Voltage| C[LPC2148 ADC - P0.29]
    C --> D[10-Bit Digital Conversion]
    D --> E[Celsius Calculation]
    E --> F[16x2 LCD & UART0]
```

---

### 7.3 ADC Interfacing (`ADC0` – `AIN2` / `P0.29`)

**ADC** stands for **Analog-to-Digital Converter**. Because the microcontroller processes binary numbers while the LM35 outputs an analog voltage, the LPC2148's inbuilt **10-bit ADC** translates the sensor voltage into a digital value between `0` and `1023`.

- **ADC Clock:** `3 MHz` (derived from `PCLK = 15 MHz` with `CLKDIV = 4`).
- **Conversion Formula:** `Temperature (°C) = (ADC_Result × 3.3 / 1023.0) × 100.0`.

**ADC working block diagram:**

```mermaid
flowchart TD
    A[LM35 Analog Output] --> B[P0.29 / AD0.2 Input]
    B --> C[Start 10-Bit ADC Conversion]
    C --> D[Read ADDR2 Result Register]
    D --> E[Convert Digital Count to °C]
    E --> F[Compare Setpoint & Display]
```

**ADC pin connection:**

| Signal | LPC2148 Pin |
| :--- | :--- |
| LM35 Analog Output | `P0.29` (`AD0.2` / `AIN2`) |

---

### 7.4 16×2 LCD Interfacing

A **16×2 alphanumeric LCD** (2 rows × 16 columns) provides the local operator display for boot prompts, setpoint entry feedback, interactive RTC menus, live telemetry (`DD/MM/YYYY HH:MM:SS TM:XX°`), and `BUZZER ALERT!!` warnings.

**Important LCD pins:**

| LCD Pin | Function |
| :--- | :--- |
| `RS` | Register Select (`0` = Command, `1` = Data) |
| `RW` | Read/Write (`0` = Write to LCD) |
| `EN` | Enable strobe pulse |
| `D0–D7` | 8-bit parallel data bus |

**LCD interfacing block diagram:**

```mermaid
flowchart LR
    A[LPC2148 MCU] -->|8-Bit Data Bus: P0.8–P0.15| B[16x2 Alphanumeric LCD]
    A -->|Control Lines: RS, EN, RW| B
    B --> C[Displays Date, Time, Temp & Alerts]
```

**LCD pin connections:**

| LCD Signal | LPC2148 Pin |
| :--- | :--- |
| `D0–D7` | `P0.8 – P0.15` |
| `RS` | `P0.16` |
| `EN` | `P0.17` |
| `RW` | `P0.18` |

---

### 7.5 4×4 Matrix Keypad Interfacing

A **4×4 matrix keypad** arranges 16 switches in 4 rows and 4 columns, allowing the microcontroller to detect any key press using 8 GPIO pins.

- **Startup Role:** Enter custom temperature threshold (`*` clears/backspaces digits, `#` confirms).
- **Menu Role:** Navigate and edit RTC fields (`1:TIME`, `2:DATE`, `3:ALL`, `4:EXIT`).

**Keypad working block diagram:**

```mermaid
flowchart LR
    A[LPC2148 Port 1] -->|Row Scan: P1.16–P1.19| B[4x4 Matrix Keypad]
    B -->|Column Read: P1.20–P1.23| A
    A --> C[Identify Key: 0–9, *, #]
    C --> D[Update Setpoint or RTC]
```

**Keypad pin connections:**

| Keypad Signal | LPC2148 Pin |
| :--- | :--- |
| Row 1 (`ROW0`) | `P1.16` |
| Row 2 (`ROW1`) | `P1.17` |
| Row 3 (`ROW2`) | `P1.18` |
| Row 4 (`ROW3`) | `P1.19` |
| Column 1 (`COL0`) | `P1.20` |
| Column 2 (`COL1`) | `P1.21` |
| Column 3 (`COL2`) | `P1.22` |
| Column 4 (`COL3`) | `P1.23` |

---

### 7.6 RTC Interfacing

**RTC** stands for **Real-Time Clock**. The LPC2148 features a dedicated on-chip RTC block driven by a `1 Hz` prescaled clock (`PREINT` and `PREFRAC` derived from `15 MHz PCLK`) that continuously tracks seconds, minutes, hours, day of month, month, and year independently of the CPU loop.

**RTC working block diagram:**

```mermaid
flowchart TD
    A[1 Hz Prescaled Clock] --> B[LPC2148 Internal RTC Registers]
    B --> C[Read SEC, MIN, HOUR, DOM, MONTH, YEAR]
    C --> D[Display on 16x2 LCD & Log via UART0]
    E[Keypad Input via EINT1] --> F[Write New Date/Time]
    F --> B
```

**Working sequence:**
1. The RTC increments time and calendar registers every second (`1 Hz`).
2. The firmware reads live date and time registers on each monitoring cycle.
3. Values are formatted onto the 16×2 LCD and included in 1-minute `UART0` reports.
4. When `EINT1` is triggered, the user enters updated fields via the keypad, which are written back into the RTC registers.

---

### 7.7 External Interrupt (`EINT1` on `P0.3`)

An **external interrupt** allows the processor to respond immediately to a hardware button press without wasting CPU cycles polling in the main loop.

In this project, **External Interrupt 1 (`EINT1`)** on `P0.3` is configured as falling-edge triggered and registered in the **Vectored Interrupt Controller (`VIC`)** to launch the interactive RTC configuration menu.

**External interrupt block diagram:**

```mermaid
flowchart LR
    A[Switch Press on P0.3] -->|Falling Edge| B[EINT1 & VIC Controller]
    B --> C[Execute EINT1_ISR]
    C --> D[Log Interrupt on UART0]
    D --> E[Open LCD RTC Menu]
    E --> F[Update RTC via Keypad]
```

**Pin connection:**

| Signal | LPC2148 Pin |
| :--- | :--- |
| External Interrupt (`EINT1`) | `P0.3` |

---

### 7.8 UART Communication (`UART0`)

**UART** stands for **Universal Asynchronous Receiver Transmitter**. `UART0` streams formatted system headers, 1-minute tabular sensor logs, interrupt notifications, and threshold breach alerts to a host PC serial terminal at **9600 bps, 8N1** through a **MAX232** RS-232 level shifter and DB9 serial cable.

**UART working block diagram:**

```mermaid
flowchart LR
    A[LPC2148 UART0 - TXD0 P0.0] -->|9600 bps, 8N1| B[MAX232 Level Shifter]
    B -->|DB9 Serial Cable| C[PC Serial Terminal Log]
```

**UART0 pin connections:**

| UART Signal | LPC2148 Pin |
| :--- | :--- |
| `TXD0` (Transmit) | `P0.0` |
| `RXD0` (Receive) | `P0.1` |

---

### 7.9 Buzzer Interfacing

An industrial **buzzer** connected to `P0.19` provides immediate audible notification whenever ambient temperature rises above the user-configured threshold.

**Working:**
- Measured temperature is compared against the configured setpoint on every cycle.
- If `Temperature > Setpoint`, `P0.19` is driven `HIGH` (`IOSET0`), sounding the buzzer and displaying `BUZZER ALERT!!` on the LCD.
- Once temperature drops below the setpoint, `P0.19` is cleared `LOW` (`IOCLR0`), silencing the alarm and restoring normal display.

**Buzzer working block diagram:**

```mermaid
flowchart TD
    A[Measured Temperature] --> B{Temperature > Setpoint?}
    B -->|Yes| C[Set P0.19 HIGH - Buzzer ON & LCD Alert]
    B -->|No| D[Clear P0.19 LOW - Buzzer OFF]
```

**Buzzer pin connection:**

| Signal | LPC2148 Pin |
| :--- | :--- |
| Buzzer Control | `P0.19` (Digital GPIO Output) |

---

## 8. Pin Configuration Summary

| Peripheral | Signal | LPC2148 Pin(s) |
| :--- | :--- | :--- |
| **LM35 Sensor** | Analog Output (`AIN2`) | `P0.29` (`AD0.2`) |
| **16×2 LCD** | Data Bus (`D0–D7`) | `P0.8 – P0.15` |
| **16×2 LCD** | `RS` (Register Select) | `P0.16` |
| **16×2 LCD** | `EN` (Enable) | `P0.17` |
| **16×2 LCD** | `RW` (Read/Write) | `P0.18` |
| **4×4 Keypad** | Rows (`ROW0–ROW3`) | `P1.16 – P1.19` |
| **4×4 Keypad** | Columns (`COL0–COL3`) | `P1.20 – P1.23` |
| **UART0** | `TXD0` / `RXD0` | `P0.0` / `P0.1` |
| **External Interrupt** | `EINT1` | `P0.3` |
| **Buzzer** | Digital Output | `P0.19` |
| **RTC** | Internal Clock & Calendar | On-chip RTC registers |

---

## 9. Overall Project Working

### 9.1 Step-by-Step Execution Sequence

1. **System Initialization:** Configure PLL (`60 MHz` CCLK, `15 MHz` PCLK) and initialize `LCD`, `ADC0`, `RTC`, `UART0`, `4×4 Keypad`, `EINT1` (`VIC`), and buzzer GPIO (`P0.19`).
2. **Startup Setpoint Entry:** Prompt the operator on the LCD to enter a temperature threshold via the keypad (`*` backspace, `#` confirm) and print the initial configuration banner over `UART0`.
3. **Sensor Acquisition:** Read the analog voltage from the LM35 sensor on `P0.29` (`AD0.2`) and convert the 10-bit ADC result into degrees Celsius.
4. **Live LCD Telemetry:** Read current date and time from the RTC and refresh the 16×2 LCD every second (`DD/MM/YYYY HH:MM:SS TM:XX°`).
5. **Periodic UART Logging:** At every 1-minute boundary (`SEC == 00`), transmit a formatted tabular row (`DATE | TIME | TEMPERATURE`) to the PC serial terminal.
6. **Threshold Comparison & Alerting:** Compare live temperature against the setpoint; if exceeded, drive `P0.19` `HIGH` to activate the buzzer, display `BUZZER ALERT!!` on the LCD, and send `[!!] TEMPERATURE ALERT` over `UART0`.
7. **Live RTC Interrupt Handling:** If `EINT1` (`P0.3`) is pressed at any time, pause monitoring, log `INTERUPPT RAISED....` on `UART0`, update date/time via the keypad menu, and resume normal monitoring.

### 9.2 Firmware Execution Flowchart

```mermaid
flowchart TD
    A([Start]) --> B[Initialize PLL, LCD, ADC0, RTC, UART0, Keypad & EINT1]
    B --> C[Enter Temperature Setpoint via 4x4 Keypad]
    C --> D[Transmit Configuration Header via UART0]
    D --> E{EINT1 Interrupt Flag?}
    E -->|Yes| F[Open LCD RTC Menu & Update Date/Time]
    F --> E
    E -->|No| G[Read LM35 via ADC0 & Compute Temp °C]
    G --> H[Read RTC Date & Time]
    H --> I[Update 16x2 LCD Display]
    I --> J{SEC == 00?}
    J -->|Yes| K[Log Timestamped Data via UART0]
    J -->|No| L{Temperature > Setpoint?}
    K --> L
    L -->|Yes| M[Activate Buzzer P0.19, LCD Alert & UART Alert]
    L -->|No| N[Deactivate Buzzer P0.19]
    M --> E
    N --> E
```

---

## 10. Captured Hardware & Serial Output

📄 Full captured serial terminal log is available in [`uart_log.txt`](uart_log.txt).

### 10.1 16×2 LCD Display States

**Normal Monitoring Mode:**
```text
6/11/2026
03:28:50  TM:28°
```

**Threshold Breach Alert Mode:**
```text
BUZZER ALERT!!
```

### 10.2 Captured UART0 Serial Terminal Output

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

## 11. Software Modules

The firmware is written in modular **Embedded C**, separating hardware driver logic for each peripheral:

| Module | Description |
| :--- | :--- |
| `main.c` | System initialization, threshold entry, main monitoring loop, and alarm control |
| `adc.c` / `adc.h` | `ADC0` initialization and 10-bit conversion routines for LM35 (`AIN2`) |
| `lcd.c` / `lcd.h` | 8-bit parallel `16×2 LCD` command, string, integer, and float display drivers |
| `kpm.c` / `kpm.h` | `4×4 Matrix Keypad` row/column scanning and debounced key decoding |
| `rtc.c` / `rtc.h` | On-chip `RTC` prescaler setup, register read/write, and menu configuration |
| `uart.c` / `uart.h` | `UART0` (`9600 bps, 8N1`) initialization and formatted string/number transmission |
| `ext_int.c` / `ext_int.h` | `VIC` and `EINT1` (`P0.3`) interrupt configuration and ISR handler |

---

## 12. Testing & Verification

| Test ID | Test Case | Action / Input | Expected Result | Status |
| :--- | :--- | :--- | :--- | :--- |
| **TC-01** | **Power ON & Boot** | Apply power to ARM7 board | Peripherals initialize and LCD prompts for temperature setpoint | ✅ Pass |
| **TC-02** | **Threshold Entry** | Type `35` on keypad, test `*` backspace, press `#` | LCD echoes digits, `*` deletes digit, `#` confirms `35.0 °C` setpoint | ✅ Pass |
| **TC-03** | **Temperature & ADC** | Keep LM35 at ambient `28.0 °C` (`< 35.0 °C`) | ADC converts analog voltage accurately and LCD shows `TM:28°` | ✅ Pass |
| **TC-04** | **RTC Live Display** | Observe LCD during normal monitoring | Live date (`DD/MM/YYYY`) and time (`HH:MM:SS`) increment every second | ✅ Pass |
| **TC-05** | **Threshold Alert** | Heat LM35 above `35.0 °C` (e.g., `52.5 °C`) | Buzzer turns ON (`P0.19 = HIGH`), LCD shows `BUZZER ALERT!!`, UART prints alert | ✅ Pass |
| **TC-06** | **Alarm Recovery** | Cool LM35 back below `35.0 °C` (`30.9 °C`) | Buzzer turns OFF (`P0.19 = LOW`) and LCD resumes normal telemetry view | ✅ Pass |
| **TC-07** | **1-Minute UART Log** | Monitor serial terminal across `SEC == 00` | Formatted `DATE \| TIME \| TEMPERATURE` row logs every 60 seconds | ✅ Pass |
| **TC-08** | **EINT1 RTC Update** | Press `EINT1` button (`P0.3`), select `1:TIME` / `2:DATE` | Monitoring pauses, `UART0` logs interrupt, RTC updates and resumes | ✅ Pass |

---

## 13. Applications

- Industrial shop-floor and machinery thermal supervision.
- Server room and electrical panel temperature monitoring.
- Pharmaceutical and laboratory environment compliance logging.
- Cold-storage and warehouse temperature threshold alerting.
- Embedded systems and ARM7 peripheral interfacing training.

---

## 14. Limitations & Future Enhancements

### Current System Limitations
- **Volatile Memory Only:** Setpoint configuration is stored in SRAM and resets when board power is removed; serial logs require an active PC terminal connection.
- **No RTC Coin-Cell Backup (`VBAT`):** Disconnecting main power resets the RTC unless an external `3V` battery is attached to `VBAT`.
- **Single-Channel Sensing:** Monitors a single analog temperature zone (`AIN2`) over a wired RS-232 serial connection.

### Future Enhancements
- **Non-Volatile Logging (`I2C EEPROM` / `SD Card`):** Store user setpoints in EEPROM (`AT24C256`) and save offline CSV logs onto a microSD card.
- **Wireless IoT & Cloud Telemetry:** Interface an `ESP8266`/`ESP32` Wi-Fi or `GSM` module via UART to stream data to cloud dashboards and send SMS/email alerts.
- **Multi-Sensor Industrial Expansion:** Add humidity (`DHT22`/`HIH-4000`), gas/smoke (`MQ-2`), and multi-zone temperature sensors on unused `ADC0` channels.
- **Automated Cooling Actuation:** Integrate an opto-isolated relay driver to automatically switch on industrial exhaust fans or cooling units during high-temperature events.

---

## 15. Conclusion

The **Industrial Environment Monitoring System** demonstrates complete bare-metal peripheral interfacing on the **NXP LPC2148 (ARM7)** microcontroller. By integrating analog sensor acquisition (`LM35` + `ADC0`), real-time timekeeping (`RTC`), local operator interaction (`16×2 LCD` + `4×4 Keypad`), asynchronous hardware interrupts (`EINT1` + `VIC`), serial telemetry (`UART0`), and threshold-based audio alarming (`Buzzer`), the system delivers a reliable and responsive embedded monitoring solution.

---

## 17. Author

- **Author:** Magham Bhanu Prakash
- **Project:** Industrial Environment Monitoring System Using LPC2148
- **Microcontroller:** NXP LPC2148 (ARM7TDMI-S)
- **Language & IDE:** Embedded C | Keil µVision
