# SIMDPS: Smart Industry Monitoring And Disaster Prediction System

An Arduino-based smart industry monitoring system with various gas, ambient and disaster detection sensors, GPS (Global Positioning System) and Wi-Fi modules, and an alerting mechanism.

## Technologies

The project is developed and deployed on an Arduino UNO with the following sensors and devices

- Arduino UNO
- Ultrasonic Distance Sensor
- Breadboard and Jumper Wires
- MQ2 (Gas & Smoke Sensor)
- MQ3 (Alcohol Sensor)
- MQ5 (Methane, LPG & Propane Sensor)
- MQ7 (Carbon Monoxide Sensor)
- BMP-180 (Pressure Sensor)
- LDR, Buzzer & LED
- ESP-8266 WiFi Card
- Neo-6M (GPS)
- DHT-11 (Temperature & Humidity Sensor)
- SW-420 (Vibration Sensor)
- SEN-16 (Flame Sensor)
- PIR (Motion Detection Sensor)

## Deployment

- Flash the code to the Arduino UNO via an appropriate COM port and WiFi credentials.

> Note: Due to memory constraints, the code was split between two Arduino UNOs. An Arduino MEGA, having higher memory capacity, might be a more suitable option.

## Installation

The Arduino IDE needs the following packages installed either manually or through the GUI package manager

- [BMP-180](https://github.com/sparkfun/BMP180_Breakout_Arduino_Library)
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)
- [DHT11](https://playground.arduino.cc/Main/DHTLib/)

## Circuit Diagram
<br/>

![Circuit Diagram](https://user-images.githubusercontent.com/67030839/216771136-889ddccf-1ac3-41a0-ba27-aa5f97f2baeb.png)

<br/>

## Authors

<table>
  <tr>
    <td align="center"><a href="https://github.com/AJ1479"><img src="https://avatars2.githubusercontent.com/u/67030839?v=4" width="100px;" alt=""/><br /><sub><b>AJ1479</b></sub></a><br /><a href="https://github.com/csivitu/CodeGolf-Backend/commits?author=AJ1479" title="Code">💻</a> <a href="https://github.com/csivitu/CodeGolf-Backend/commits?author=AJ1479" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/theProgrammerDavid"><img src="https://avatars.githubusercontent.com/u/35698009?v=4" width="100px;" alt=""/><br /><sub><b>David</b></sub></a><br /><a href="https://github.com/csivitu/CodeGolf-Backend/commits?author=ashikka" title="Code">💻</a> <a href="https://github.com/csivitu/CodeGolf-Backend/commits?author=ashikka" title="Documentation">📖</a></td>
  </tr>
</table>
