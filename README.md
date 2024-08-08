# IoT-MotorControl-MobileApp

## Project Overview

This project demonstrates motor power and speed control using an ATmega32P microcontroller and a mobile app provided by Blynk. The system allows for the control of motor speed and direction via a smartphone, providing real-time feedback on the motor's status. The hardware setup includes an ATmega32P microcontroller, an ESP module for Wi-Fi communication, and a mobile app interface using Blynk.

## Features

- **Motor Speed Control:** Adjust the motor speed through the Blynk mobile app.
- **Motor Direction Control:** Switch the motor's direction between clockwise and counterclockwise.
- **LCD Display:** Real-time display of motor speed and direction on an LCD.
- **Start/Stop Functionality:** Control the motor's operation state through the mobile app.

## Hardware Components

- ATmega32P Microcontroller
- ESP8266 Wi-Fi Module
- LCD Display
- Motor Driver
- Power Supply
- Mobile Device with Blynk App

## Software Components

- Atmel Studio (for ATmega32P code)
- Arduino IDE (for ESP8266 code)
- Blynk Mobile App

## Code Overview

### ATmega32P Code

The main code for the microcontroller is written in C using Atmel Studio. The code handles motor speed and direction control, communication with the Blynk app via UART, and real-time data display on an LCD.

### ESP8266 Code

The ESP8266 code (to be added) will facilitate communication between the ATmega32P and the Blynk app over Wi-Fi.

**[Include ESP8266 code here once provided.]**

## Circuit Diagram

[Include or link to the circuit diagram here.]

## Project Video

Watch the project in action:  
[![Watch the video](video_thumbnail.png)](link_to_your_video)

## How to Use

1. Upload the provided code to the ATmega32P microcontroller.
2. Set up the ESP8266 module with the provided code.
3. Connect all hardware components as per the circuit diagram.
4. Open the Blynk app and connect to the ESP8266 module.
5. Use the app to control motor speed and direction.

---
