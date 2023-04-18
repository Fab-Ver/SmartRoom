# SmartRoom
Third assignment of Embedded Systems and IoT course 2022/2023
## Assignment Description
We want to realise an IoT system implementing a simplified version of a smart room, as a smart system monitoring and controlling the state of a room. 

The system is composed of 5 subsystems: 
- <b>Room Sensor-Board (esp):</b> embedded system to monitor the state of the room by using a set of sensors. It interacts with the Room Service (via MQTT).
- <b>Room Service (backend - pc):</b> service functioning as the main unit governing the management of the room, it interacts through the serial line with the Controller (arduino), it interacts via MQTT with the Room SensorBoard (esp), it interacts via HTTP with the Dashboard (frontend/PC).
- <b>Room Controller (Arduino):</b> embedded system controlling lighting and roller blinds; it interacts via serial line with the Room Service and via BT with the Room App.
- <b>Room App (Android - smartphone):</b> mobile app that makes it possible to manually control lights and roller blinds, it interacts with the Room Controller via Bluetooth.
- <b>Room Dashboard (Frontend)</b>: front-end to visualise and track the state of the room, it interacts with the Room Service.

### Hardware components 
- <b>Room Sensor-board:</b>
  - SoC ESP32 board (or ESP8266)
  - 1 green led 
  - 1 PIR
  - 1 photoresistor analog sensor
- <b>Room Controller:</b>
  - Microcontroller Arduino UNO board including:
  - 1 green led simulating a light subsystem
  - 1 servo motor simulating the roller blind subsystem
  - 1 Bluetooth module HC-06 o HC-05

### General Behaviour of the system

The Smart Room system is meant to control the lighting system and roller blinds according to the following policy:
- If no one is in the room, the light (of the lighting subsystem) should be off.
- If someone enters in the the room and the room is dark, then the light should be turned on (if it was off).
- The roller blinds are fully rolled up automatically the first time someone enters in the room, from 8:00 (if someone enters).
- The roller blinds are fully unrolled at 19:00 (if they are up and no one is in the room), or as soon as someone who is still in the room at 19:00 leaves the room.
- Through the mobile app, a user can: turn on/off the light roll-up/unroll the roller blind – also partially (from 0 to 100%).
- Through the dashboard a room manager can: track the state of the room, in particular in which hours and how long the lights where on fully control the light and roller blinds.
- It can be assumed that the room is accessed from 8:00 to 19:00. 

### Further details:

- About the Room Sensor-board: the led should be on when someone is in the room and off when no one is the room
- About the Room Controller: the servo motor controls/simulates the roller blinds (0° means completely rolled-up
180° means completely unrolled), the green light simulates the lighting system: on/off.
- No specific constraints/requirements are given for the Room Mobile App and the Room Dashboard
