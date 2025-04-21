## Overview
This Arduino sketch interfaces with a keypad, an OLED display, and an LCD using I2C communication. It's designed to relay keypad inputs to a connected system and display messages from an external source. Due to RAM limitations on the Arduino, some logic processing is handled by a ROS (Robot Operating System) node.

Target board: **DFRobot FireBeetle 2 ESP32-E**

## Setup

### Hardware

- **Keypad**: Connected to digital pins 2, 4, 25, 26 (rows) and 18, 13, 14, 15 (columns).
- **OLED Display**: I2C at address `0x3C`.
- **LCD Display**: I2C at address `0x27`.
- **UART Communication**: Uses UART2 (TX=1, RX=3) for serial communication with external systems.

### Libraries

- `Wire.h`: For I2C communication.
- `Keypad.h`: For keypad interfacing.
- `Adafruit_SSD1306` and `Adafruit_GFX`: For OLED display control.
- `LiquidCrystal_I2C`: For LCD display control.

Install libraries through the Arduino Library Manager:
```
Adafruit SSD1306
Adafruit GFX
Keypad
LiquidCrystal I2C
```

## Logic Handling via ROS

Due to memory constraints on the ESP32, message parsing and routing logic are offloaded to a ROS 2 node. The ROS node processes incoming data and tags it with the appropriate display prefix (`oled:`, `lcd:`, `both:`), which the Arduino then parses and displays.

## Key Functions

- `setup()`: Initializes UART, displays, and keypad.
- `loop()`: Reads keypad input and incoming UART messages, dispatches to display.
- `displayText(const char* text, int textSize)`: Displays text on OLED, LCD, or both depending on the prefix (`oled:`, `lcd:`, `both:`).

## Usage

- **Keypad Input**: Pressed keys are sent via UART to an external system (e.g., ROS).
- **External Messages**: The Arduino receives messages over UART ending with `|` and displays them based on the prefix.

### Example UART Messages

```
oled:Hello from ROS|
lcd:Line1 Line2 on LCD|
both:Universal Msg|
```

Each message must end with `|` to be recognized as complete.

## Notes

- OLED uses 128x64 resolution and can display larger fonts.
- LCD is limited to 16x2 characters; text longer than this is truncated.
- Communication is one-way UART for keypresses and UART read for display instructions.