## Overview

This ROS node handles communication between a ROS-based system and an Arduino connected via I2C. It processes input commands and relays information to the Arduino for display on connected devices.

## Setup

### Dependencies

- **ROS 2**: Ensure ROS 2 is installed and properly configured.
- **Python Libraries**: `rclpy`, `smbus2` (On Ubuntu, you can install `smbus2` with `apt install python3-smbus2`).

### Installation

1. Clone the repository into your ROS workspace:
   ```
   cd ~/your_ros_workspace/src
   git clone <repository-url>
   ```
2. Install required Python packages (if not on Ubuntu):
   ```
   pip install smbus2
   ```
3. Build the package:
   ```
   cd ~/your_ros_workspace
   colcon build
   ```

## Node Details

### Initialization

- **Node Name**: `i2c_publisher`
- **Publisher**: `/keypad_input` (std_msgs/String) - Publishes keys pressed on the keypad.
- **Subscription**: `/send_to_arduino` (std_msgs/String) - Subscribes to messages to send to Arduino.

### Communication

- **I2C Bus**: Communicates with Arduino using I2C. Default address is `0x08`.
- **Sending**: Messages are sent to the Arduino, with logic to split messages into chunks if they exceed 32 bytes.
- **Receiving**: Reads single key inputs from the Arduino and publishes them on the ROS topic.

## Functions

- `send_to_arduino(msg)`: Processes and sends text commands to the Arduino with appropriate prefixes.
- `send_in_batches(i2c_addr, register, message)`: Sends messages to Arduino in 32-byte chunks for reliability.
- `read_keypad()`: Reads and publishes a single key press from the Arduino.

## Logic Handling

- Manages prefix logic such as `"oled:"`, `"lcd:"`, and `"both:"` on the ROS side to save Arduino RAM.
- Truncates text for the LCD display to prevent overflow.

## Usage

1. Run the node using:
   ```
   ros2 run <package_name> i2cpublisher
   ```
2. Ensure the Arduino is connected and functioning as intended.
3. Interact with the node via the ROS topics for sending and receiving data.