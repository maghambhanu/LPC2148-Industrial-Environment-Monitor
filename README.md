# 🏭 Industrial Environment Monitoring & Real-Time Data Logger (ARM7 LPC2148)

![MCU](https://img.shields.io/badge/MCU-NXP%20LPC2148%20(ARM7TDMI--S)-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-orange)
![Clock](https://img.shields.io/badge/CCLK-60%20MHz%20%7C%20PCLK%2015%20MHz-green)
![Protocol](https://img.shields.io/badge/Interfaces-ADC%20%7C%20UART0%20%7C%20RTC%20%7C%20EINT1-lightgrey)

A bare-metal embedded firmware system built on the **NXP LPC2148 (ARM7)** microcontroller for continuous industrial temperature monitoring, threshold-based audio alarming, real-time LCD telemetry, and automated serial data logging[cite: 10, 51].

---

## 🔌 System Block Diagram

![System Block Diagram](docs/circuit.jpeg)

---

## 🔄 Firmware Execution Flowchart

![Firmware Flowchart](docs/block_dagram.jpeg)

---

## ✨ Key Features
* **Precision Analog Sensing:** Interfaces with an **LM35** temperature sensor via the on-chip 10-bit ADC (`3 MHz` ADC clock) with real-time Celsius conversion (`10 mV/°C`)[cite: 2, 32].
* **Interactive Threshold Configuration:** Uses a **4×4 Matrix Keypad** and **16×2 Alphanumeric LCD** with backspace (`*`) and confirmation (`#`) handling to set custom temperature limits at boot[cite: 10, 16].
* **Interrupt-Driven RTC Updates:** Uses **External Interrupt 1 (EINT1)** on `P0.3` to pause normal monitoring and allow live reconfiguration of Date and Time (`HH:MM:SS` & `DD/MM/YYYY`) via an interactive LCD menu[cite: 10, 14, 34].
* **Automated UART0 Telemetry:** Logs formatted timestamped temperature reports to a PC serial terminal (`9600 bps, 8N1`) once every minute and triggers instant `[!!] TEMPERATURE ALERT` banners during threshold breaches[cite: 10, 44].
* **Audio Alarm System:** Activates an industrial buzzer on `P0.19` whenever ambient temperature exceeds the configured setpoint[cite: 10].

---

## 📌 Hardware Pinout & Peripheral Mapping

| Peripheral | LPC2148 Pin(s) | Function / Description |
| :--- | :--- | :--- |
| **LM35 Temp Sensor** | `P0.29` (`AIN2`) | Analog voltage input (`10 mV/°C`)[cite: 2, 32] |
| **16×2 LCD Data Bus** | `P0.8 – P0.15` | 8-bit parallel data interface (`D0–D7`)[cite: 26] |
| **16×2 LCD Control** | `P0.16`, `P0.18`, `P0.17` | `RS`, `RW`, and `EN` control lines[cite: 26] |
| **4×4 Keypad Rows** | `P1.16 – P1.19` | Row scan outputs (`ROW0–ROW3`)[cite: 18] |
| **4×4 Keypad Columns** | `P1.20 – P1.23` | Column sense inputs (`COL0–COL3`)[cite: 18] |
| **UART0 Serial** | `P0.0` (`TXD0`), `P0.1` (`RXD0`) | Serial data logger (`9600 baud`)[cite: 44] |
| **External Interrupt** | `P0.3` (`EINT1`) | Edge-triggered interrupt for RTC setup[cite: 14] |
| **Alert Buzzer** | `P0.19` | Digital GPIO output (`HIGH` = Alert)[cite: 10] |

---

## 🖥️ Sample UART Serial Output
📄 Full captured log available in [`uart_log.txt`](uart_log.txt)[cite: 49]

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
