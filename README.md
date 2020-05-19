# servicealerter
An application which gets the ADXL345 roll, pitch,time and tempreature values, and sends it to a MQTT server which is preset with a specific username and password. The message published is then subscribed by clients, and based on base limit rules set in the subscriber the user is alerted. The servicealertvisualiser repo uses these message data to visualise it a QT application.

## Introduction
MQTT is a communication protocol which is used to carry less data to low powered machines. ADXL345 is a low priced and small accelerometer which can quantise the angular position w.r.t. Earth’s gravitational force. We are going to use this sensor to get the Pitch and Roll value and publish the data to Mosquitto broker using the Paho MQTT C library, and perform multiple tasks by connecting it to a Raspberry Pi 4. 

## Connection 
The connection between the ADXL345 sensor and Raspberry Pi has been established with a help of a mini-bread board as per the below Table :

| ADXL345 PINS | Raspberry Pi 4 pins|
| --------|---------|
| GND| GND – PIN 9|
| VCC and CS | 3V – PIN 1|
| SDA |SDA – GPIO 2 – PIN 3|
| SCL |SCL – GPIO 3 – PIN 5|

## Environment Setup-Mosquitto service
Installs mosquito broker in the environment
```
sudo apt-get update
sudo apt-get install mosquitto
```
Installs mosquitto clients to publish and subcribe to topic
```
sudo apt-get install mosquitto-clients
```
To establish a connection securely using a username and password, we would be utilising the utility built within mosquitto, named mosquitto_passwd, to generate the password file required for doing so.
```
sudo mosquitto_passwd -c /etc/mosquitto/passwd rageobi
Password: llll
```
It is also essential, to let the Mosquitto service know the password file created by us above. This could be done by creating a config file with below commands.
```
sudo nano /etc/mosquitto/conf.d/default.conf
```
And on the opened default.conf file, we can paste the content below
```
allow_anonymous false
password_file /etc/mosquitto/passwd
```
Once, this has been done the service can now be utilised only if there is a valid username and password associated with the publish or subscribe client calls. 

## Code
- ADXL345.cpp and ADXL345.h – The sensor macros
- I2CDevice.cpp and I2CDevice.h – The sensor macros inherit some basic I2C functionalities from these files
- MQTT.cpp and MQTT.h – Programmed an approach that is more Object oriented in establishing connection for a client(Publisher/Subscriber) and sends/receives message. 
  - MQTT(const char*…) – Constructor to initialise all the required parameters
  - void EstablishConnection(MQTT::Type type) – Establishes connection based on the MQTT enum TYPE(Publisher/Subscriber)
  - void messageROS(const char * jsonPayload="") – Sends jsonPayload parameter if MQTT enum type is Publisher or Receives messages if it’s a subscriber
  - virtual ~MQTT() – Disconnects/Destroys the client created
- AppFunctions.cpp and AppFunctions.h – Non-class function repository for the application
  - float getCPUTemperature() – Reads the Raspberry Pi temperature value stored in the "/sys/class/thermal/thermal_zone0/temp" location
  - char* getDateTime() – Uses time library to fetch the local time of Raspberry Pi.
  - const char * jsonCharacteriser(float pitchVal, float rollVal) – Function to convert Pi’s Tempreature, Local time, ADXL345’s pitch and roll values into a tree structured json key-value pairs using the json-c library
  - Sample json – {“d”:{“key1”:”value1”,”key2”:”value2”……..}}
- application.cpp – The main source file which connects all the others mentioned above
  - Creates an object of class MQTT, and establishes connection as Publisher/Subscriber.
  - Creates an object of class ADXL345, and sets resolution,range, pitch and roll through the sensor state function.
  - Gets the json character array through jsonCharacteriser function, and set that as the jsonPayload for sending a message if the MQTT class object is for Publisher Type.
- subscriber1.cpp – Subscriber client which parses the json data sent by the Publisher through the json-c library functions lights LED connected to Raspberry Pi GPIO pin if the temperature of Raspberry is greater than 40 for 3 seconds.
- subscriber2.cpp – Subscriber client which parses the json data sent by the Publisher through the json-c library functions lights LED connected to Raspberry Pi GPIO pin if the Pitch value of the ADXL345 sensor is greater than -10 for a second.

## Execution
- application.cpp
  - To build: g++ -Wall -o "application" "application.cpp" AppFunctions.cpp MQTT.cpp ADXL345.cpp I2CDevice.cpp -lpaho-mqtt3c -ljson-c
  - To execute: ./application
- subscriber1.cpp
  - To build: g++ -Wall -o "subscriber1" " subscriber1.cpp" -lpaho-mqtt3c -ljson-c -lwiringPi
  - To execute: ./subscriber1
- subscriber2.cpp
  - To build:g++ -Wall -o "subscriber2" " subscriber2.cpp" -lpaho-mqtt3c -ljson-c -lwiringPi
  - To execute: ./ subscriber2
