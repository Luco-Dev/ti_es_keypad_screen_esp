# ESP32 Keypad & Display Interface for ROS 2

## Introduction

This Arduino sketch is designed for the **DFRobot FireBeetle 2 ESP32-E**, acting as a bridge between a **keypad**, **OLED**, and **LCD**, while communicating with a **ROS 2 node** over UART. Display rendering is handled on the ESP, while logic and formatting are offloaded to ROS due to limited memory on the microcontroller.

Project repo:  
[https://github.com/Luco-Dev/ti_es_keypad_screen_esp](https://github.com/Luco-Dev/ti_es_keypad_screen_esp)

## Packages

No external packages are used beyond standard Arduino libraries available through the Arduino Library Manager.

Required libraries:
- Adafruit SSD1306  
- Adafruit GFX  
- Keypad  
- LiquidCrystal I2C

Include these in your sketch:
'''
#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
'''

## Hardware that interacts with it

- **Keypad**  
  - Row pins: D2, D4, D25, D26  
  - Column pins: D18, D13, D14, D15

- **OLED Display**  
  - I2C address: `0x3C`  
  - 128x64 resolution  

- **LCD Display**  
  - I2C address: `0x27`  
  - 16x2 characters  

- **ROS 2 Host (e.g. Raspberry Pi)**  
  - Communicates over UART2 (TX=1, RX=3)

## Installation Guide

1. Clone this repository or copy the files into a PlatformIO or Arduino IDE project:
   [ESP Sketch Folder](https://github.com/Luco-Dev/ti_es_keypad_screen_esp/tree/ea5f72bb9a9d6f243bc620badf1091c040fc443f)

2. Install the required libraries through the Arduino Library Manager.

3. Connect the hardware as described above.

4. Flash the sketch to the ESP32.

5. On the ROS host, run the companion ROS 2 node that communicates via UART and handles display logic:
   [ROS Node Repo](https://github.com/Luco-Dev/ti_es_keypad_screen_ros)

## Configuration Options

Messages sent to the ESP over UART must be formatted using a specific prefix to indicate the display target:

- `oled:` – Show only on OLED  
- `lcd:` – Show only on LCD  
- `both:` – Attempt to show on both  
- All messages must end with a pipe (`|`) character to indicate the end of transmission.

### Example messages from ROS:

'''
oled:Moonrise detected at 21:12|
lcd:Target: Mars|
both:Starting scan now...|
'''

Keypresses from the keypad are automatically transmitted to the ROS node as single characters.

## Author

Developed by **Luco Berkouwer**  
This project is part of a modular telescope control system using ROS 2 and embedded I2C/UART hardware.  

Feel free to fork, extend, or repurpose this code for your own microcontroller-based ROS projects!
