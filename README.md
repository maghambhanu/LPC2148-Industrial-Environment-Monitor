# 🏭 Industrial Environment Monitoring & Real-Time Data Logger (ARM7 LPC2148)

![MCU](https://img.shields.io/badge/MCU-NXP%20LPC2148%20(ARM7TDMI--S)-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-orange)
![Clock](https://img.shields.io/badge/CCLK-60%20MHz%20%7C%20PCLK%2015%20MHz-green)
![Protocol](https://img.shields.io/badge/Interfaces-ADC%20%7C%20UART0%20%7C%20RTC%20%7C%20EINT1-lightgrey)

A bare-metal embedded firmware system built on the **NXP LPC2148 (ARM7)** microcontroller for continuous industrial temperature monitoring, threshold-based audio alarming, real-time LCD telemetry, and automated serial data logging.

---

## 🔌 System Block Diagram

![System Block Diagram](docs/circuit.jpeg)

---

## 🔄 Firmware Execution Flowchart

![Firmware Flowchart](docs/block_dagram.jpeg)

---

## 📸 Hardware Showcase

<p align="center">
  <img src="docs/hardware_outputs.png" alt="LPC2148 Industrial Environment Monitor Operational States" width="100%">
</p>

- **Buzzer Alert:** Activates audio alarm and displays `BUZZER ALERT!!` on the 16×2 LCD when ambient temperature exceeds the configured setpoint[cite: 1, 7].
- **Date Input:** Interactive keypad sub-menu (`1:DD 2:MM 3:YY 4:ALL 5:BACK 6:E`) for configuring individual or complete RTC date fields[cite: 2, 7].
- **Real-Time Clock:** Displays live RTC date, timestamp, and LM35 temperature reading (`6/11/2026 03:28:50 TM:28°`) during active monitoring[cite: 3, 7].
- **Time Input:** Interactive keypad sub-menu (`1:HH 2:MIN 3:SEC 4:ALL 5:BACK 6:E`) for setting hours, minutes, and seconds[cite: 4, 7].
- **Mode Selection:** Main interrupt-triggered RTC configuration menu (`1:TIME 2:DATE 3:ALL 4:EXIT`)[cite: 5, 7].
- **Complete Hardware Setup:** Vector's ARM7 (LPC2148) development board wired to the 16×2 LCD, 4×4 matrix keypad, LM35 temperature sensor, buzzer, and DB9 UART0 serial interface[cite: 6, 7].

---

## 🧩 Peripheral Interfacing Diagram

```mermaid
flowchart LR
    subgraph INPUTS ["Input & Trigger Peripherals"]
        LM35["🌡️ LM35 Temp Sensor<br/>(10 mV/°C Analog)"]
        KEYPAD["⌨️ 4×4 Matrix Keypad<br/>(Rows: P1.16–P1.19 | Cols: P1.20–P1.23)"]
        INT_BTN["🔘 RTC Setup Switch<br/>(Falling-Edge Trigger)"]
    end

    subgraph MCU ["NXP LPC2148 (ARM7TDMI-S @ 60 MHz)"]
        ADC["10-Bit ADC0<br/>(AIN2 / P0.29 | 3 MHz)"]
        GPIO_KP["GPIO Port 1<br/>(Row Scan & Col Read)"]
        VIC["VIC + EINT1<br/>(P0.3 Vectored IRQ)"]
        RTC["On-Chip RTC<br/>(1 Hz Prescaled Clock)"]
        CORE["ARM7 Core<br/>(Threshold & Control Logic)"]
        GPIO_OUT["GPIO Port 0<br/>(LCD & Buzzer Control)"]
        UART["UART0 Controller<br/>(9600 bps, 8N1)"]
    end

    subgraph OUTPUTS ["Output & Telemetry Peripherals"]
        LCD["📟 16×2 Alphanumeric LCD<br/>(D0–D7: P0.8–P0.15 | RS/EN/RW: P0.16–P0.18)"]
        BUZZER["🔔 Industrial Buzzer<br/>(P0.19 Digital Out)"]
        PC["🖥️ PC Serial Terminal<br/>(TXD0: P0.0, RXD0: P0.1 via MAX232)"]
    end

    LM35 -->|"Analog Voltage"| ADC
    KEYPAD <-->|"Matrix Scan"| GPIO_KP
    INT_BTN -->|"Hardware IRQ"| VIC

    ADC --> CORE
    GPIO_KP --> CORE
    VIC -->|"ISR Flag"| CORE
    RTC <-->|"Date & Time"| CORE

    CORE --> GPIO_OUT
    CORE --> UART

    GPIO_OUT -->|"8-Bit Parallel"| LCD
    GPIO_OUT -->|"HIGH / LOW"| BUZZER
    UART -->|"DB9 Serial"| PC
