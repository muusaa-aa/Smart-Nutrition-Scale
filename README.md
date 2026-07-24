# Smart-Nutrition-Scale
## Overview
The Smart Nutrition Scale is an embedded systems project designed to weigh food portions and instantly calculate their estimated calorie and protein content. Built using C, it utilizes a hardware interrupt-driven time-base for precise execution and features direct register-level GPIO configuration. 

## Features
* **Real-Time Weighing:** Accurately measures food weight using a load cell and HX711 24-bit ADC.
* **Nutritional Tracking:** Calculates and logs cumulative daily calories and protein across multiple meals.
* **Scrollable Menu:** Interactive button-based navigation to select between 8 pre-programmed food items (Chicken, Rice, Eggs, Beef, Broccoli, Oats, Whey Protein, Salmon).
* **Bare-Metal GPIO & Interrupts:** Utilizes Timer 1 in CTC mode for a 1 ms interrupt time-base, enabling non-blocking delays without relying on standard high-level library functions.
* **Hardware Interfacing:** Displays real-time data on a 16x2 I2C LCD and provides LED status feedback.

## Hardware & Circuit
*Note: The firmware for this project was developed in C for an AVR microcontroller using the Arduino framework, while the circuit schematic and simulation were mapped using an STM32F401VD in Proteus.*

**Components Used:**
* Microcontroller (AVR / STM32F)
* HX711 Load Cell Amplifier
* Full-bridge strain-gauge load cell
* 16x2 Character LCD with PCF8574 I2C Expander
* 4x Tactile Push Buttons (Active-Low)
* 1x LED Indicator

*(Optional: Add a screenshot of your Proteus circuit diagram or physical prototype here by dragging and dropping the image into GitHub!)*

## Software Architecture
* **Language:** Embedded C
* **Core Concepts Demonstrated:**
  * Direct AVR Register Manipulation (`DDRD`, `PORTD`, `TCCR1A`, etc.)
  * Hardware Timer Interrupts (`TIMER1_COMPA_vect`)
  * Data Structs for memory-efficient food databases
  * I2C Communication

## Future Improvements
* Implementation of persistent storage (EEPROM) to save daily nutritional totals across power cycles.
* Expansion of the food database using external memory.
* UART logging for exporting meal history to a PC.

## Author
* **Musab Jamaleldien Mohamed Yousif**
