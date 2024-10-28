# Smart Sorting Machine

This project is a microcontroller-based Smart Sorting Machine, which assists users in sorting plastic and non-plastic items, saving relevant data, and providing a user-friendly interface with an LCD, keypad, and sensors. It uses libraries for keypad input, LCD display, servos for mechanical sorting, and HX711 for weight measurement.

## Features

- **User Input & Identification:** Allows users to enter a unique identification number.
- **EEPROM Storage:** Stores user data in EEPROM for persistence across restarts.
- **Plastic Detection & Sorting:** Classifies objects as plastic or non-plastic, with mechanical sorting.
- **Weight Measurement:** Uses a load cell to measure the weight of plastic items.
- **LCD Display:** Provides feedback to users with a 16x2 I2C LCD display.
- **Ultrasonic Sensor for Capacity Check:** Detects if the machine is full, preventing further input until it is emptied.

## Components and Libraries

- **Components:**
  - 4x4 Keypad
  - HX711 Load Cell Amplifier
  - 2 Servos
  - I2C LCD Display
  - Ultrasonic Sensor (HC-SR04)

- **Libraries:**
  - `Keypad.h` - For 4x4 keypad input.
  - `Wire.h` - For I2C communication.
  - `LiquidCrystal_I2C.h` - For LCD control over I2C.
  - `Servo.h` - For controlling servos.
  - `HX711.h` - For interfacing with the load cell.
  - `EEPROM.h` - For storing user data.

## Setup

1. **Wiring the Components**:
   - Connect the load cell to HX711 (DOUT and CLK to pins 13 and 12).
   - Connect the servos to digital pins 8 and 9.
   - Wire the ultrasonic sensor to digital pins 10 (Trig) and 11 (Echo).
   - Connect the LCD to the I2C pins.
   - Keypad connections: rowPins (0, 14, 2, 3) and colPins (4, 5, 6, 7).

2. **Libraries**: Ensure all required libraries are installed in your Arduino IDE.

3. **Upload the Code**: Compile and upload the code to your microcontroller.

## Functional Overview

- **User Data Management**:
  - Enter user information via keypad and save it in EEPROM.
  - Data is read back from EEPROM at startup to retain user history.

- **Plastic Detection and Sorting**:
  - After user input, the machine checks if the item is plastic.
  - Plastic items are accepted, weighed, and stored; non-plastic items are returned.

- **Ultrasonic Sensor**:
  - Continuously monitors machine capacity. If full, displays a warning on the LCD.

- **LCD Interface**:
  - Provides feedback and instructions to the user during operation, such as user verification, weight display, and sorting results.

## Code Structure

- **Key Functions**:
  - `saveDataToEEPROM()`: Saves user-entered data to EEPROM.
  - `readDataFromEEPROM()`: Retrieves stored user data at startup.
  - `calcdist()`: Calculates distance to determine if the machine is full.
  - `tiltright()`, `tiltleft()`, `tilt180()`: Moves servos to direct items to the appropriate compartment.
  - `measure()`: Measures the weight of plastic items.
  - `displaydat()`: Displays current user and weight on LCD and Serial Monitor.

- **Control Flow**:
  - `setup()`: Initializes the machine and components, and retrieves stored data.
  - `loop()`: Manages the machine's main functions, such as user entry, item sorting, and capacity checking.

## Usage

1. **Start**:
   - Press `A` to begin the sorting process.
   - Enter user number via the keypad, then press `D` to confirm or `*` to retry.

2. **Sort Object**:
   - Select `B` for plastic or `C` for non-plastic.
   - The machine sorts the object based on selection and displays relevant data.

3. **Idle Mode**:
   - When idle, the machine checks if it is full, showing "Machine is full" on the LCD when necessary.

## Troubleshooting

- **LCD Display Issues**: Check I2C address and wiring.
- **Servo Movement Issues**: Verify power supply and pin connections.
- **Data Persistence Issues**: Ensure EEPROM storage addresses are correctly defined.

## License

This project is open-source and available for further development and customization.