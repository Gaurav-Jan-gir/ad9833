# AD9833 Function Generator

This project implements a compact, microcontroller-based function generator using the **AD9833** programmable waveform generator. It can output sine, square, and triangle waveforms over a configurable frequency and phase range, making it ideal for signal testing, circuit development, and educational applications.

## 📦 Features

- Sine, square, and triangle waveform output  
- Frequency and phase control via SPI interface  
- Compatible with microcontrollers (e.g., Arduino, STM32)  
- Optional user interface: buttons + display or serial commands  
- Low-power and portable design  

## 🧠 About the AD9833

The AD9833 is a low-power, highly integrated waveform generator capable of producing precise frequencies using Direct Digital Synthesis (DDS). It communicates via SPI and supports up to 12.5 MHz output (depending on your MCLK and configuration).

## 🛠️ Hardware Requirements

- AD9833 module  
- Microcontroller (e.g., Arduino Nano, Uno, STM32, ESP32)  
- Optional: OLED/LCD display, push buttons, rotary encoder  
- Power source (USB or battery)  
- Breadboard or custom PCB  

## 📁 Repository Contents

- `src/` – Source code for microcontroller (Arduino-compatible)  
- `lib/` – Optional libraries (if not using PlatformIO)  
- `docs/` – Circuit diagram, datasheets, and project notes  
- `README.md` – Project overview  
- `LICENSE` – Project license  

## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/ad9833-function-generator.git
