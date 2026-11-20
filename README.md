# 🏭 Industrial Environment Monitoring & Real-Time Data Logger (ARM7 LPC2148)

![Microcontroller](https://img.shields.io/badge/MCU-NXP%20LPC2148%20(ARM7TDMI--S)-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-orange)
![Clock](https://img.shields.io/badge/CCLK-60%20MHz%20%7C%20PCLK%2015%20MHz-green)
![Protocol](https://img.shields.io/badge/Interfaces-ADC%20%7C%20UART0%20%7C%20RTC%20%7C%20EINT1-lightgrey)

A bare-metal embedded firmware system built on the **NXP LPC2148 (ARM7)** microcontroller for continuous industrial temperature monitoring, threshold-based audio alarming, real-time LCD telemetry, and automated serial data logging.

---

## ✨ Key Features
* **Precision Analog Sensing:** Interfaces with an **LM35** temperature sensor via the on-chip 10-bit ADC (`3 MHz` ADC clock) with real-time Celsius conversion (`10 mV/°C`).
* **Interactive Threshold Configuration:** Uses a **4×4 Matrix Keypad** and **16×2 Alphanumeric LCD** with backspace (`*`) and confirmation (`#`) handling to set custom temperature limits at boot.
* **Interrupt-Driven RTC Updates:** Uses **External Interrupt 1 (EINT1)** on `P0.3` to pause normal monitoring and allow live reconfiguration of Date and Time (`HH:MM:SS` & `DD/MM/YYYY`) via an interactive LCD menu.
* **Automated UART0 Telemetry:** Logs formatted timestamped temperature reports to a PC serial terminal (`9600 bps, 8N1`) once every minute and triggers instant `[!!] TEMPERATURE ALERT` banners during threshold breaches.
* **Audio Alarm System:** Activates an industrial buzzer on `P0.19` whenever ambient temperature exceeds the configured setpoint.

---

## 🔌 Hardware Pinout & Peripheral Mapping

| Peripheral | LPC2148 Pin(s) | Function / Description |
| :--- | :--- | :--- |
| **LM35 Temp Sensor** | `P0.29` (`AIN2`) | Analog voltage input (`10 mV/°C`) |
| **16×2 LCD Data Bus** | `P0.8 – P0.15` | 8-bit parallel data interface (`D0–D7`) |
| **16×2 LCD Control** | `P0.16`, `P0.18`, `P0.17` | `RS`, `RW`, and `EN` control lines |
| **4×4 Keypad Rows** | `P1.16 – P1.19` | Row scan outputs (`ROW0–ROW3`) |
| **4×4 Keypad Columns**| `P1.20 – P1.23` | Column sense inputs (`COL0–COL3`) |
| **UART0 Serial** | `P0.0` (`TXD0`), `P0.1` (`RXD0`) | Serial data logger (`9600 baud`) |
| **External Interrupt**| `P0.3` (`EINT1`) | Edge-triggered interrupt for RTC setup |
| **Alert Buzzer** | `P0.19` | Digital GPIO output (`HIGH` = Alert) |

---

## 🖥️ Sample UART Serial Output
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

 REAL-TIME SENSOR DATA
 ------------------------------------------------------------
 DATE         TIME          TEMPERATURE
 ------------------------------------------------------------
 20/11/2026    16:30:00       29.4 ° C
