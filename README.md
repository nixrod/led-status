# led-status
A showcase for an interaction between an esp8266, and a backend which runs a webapp

## Hardware setup

* Wire up the hardware as seen in the picture above
* The brown cable goes to d0
* The white cable goes to d1
* The resistor has a value of 1kΩ

## Flash the NodeMcu
* Setup your Arduino IDE as described in this [instructable](http://www.instructables.com/id/Programming-ESP8266-ESP-12E-NodeMCU-Using-Arduino-/)
* Install the [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library through the Arduino IDE Library Manager
* Update the wifi ssid and password
* Update the ip of your machine which will run the server
* Flash the sketch from the arduino folder to the nodemcu

## Launch the server
* [Download and install](https://nodejs.org/en/) node.js
* Open the command prompt
* Navigate to the server folder
* run `npm install` to install all dependencies
* run `node server.js` to run the backend
* Open your browser and go to [http://localhost:3000]

## Todo
* Secure the API endpoints with basic auth
