# IoT Based Smart Agriculture System

The project aims making use of evolving technology i.e. IoT and smart agriculture using automation. Monitoring environmental factors is the major factor to improve the yield of the efficient crops. The feature of this project includes monitoring temperature, humidity and moisture in agricultural field through sensors DHT11, YL69. It will turn ON/OFF motor on the basis of soil moisture.

## Modules:

[Flow Diagram](/modules/Diagram12.jpg)

### Module-1:

In the first module we are using ESP8266 Wi-Fi module that will act as a microcontroller and it will attached to a DHT11 to sense the current temperature and humidity. The module will publish the temperature and humidity data to MQTT broker i.e Raspberry Pi.

#### Component Used:

__•    ESP8266 ESP-01:__ It is low power consuming Wi-Fi module with integrated TCP/IP protocol stack that can give any microcontroller access to WiFi network. It is self-contained SOC (System On a Chip) that doesn’t necessarily need a microcontroller to manipulate inputs and outputs.

__•    DHT11:__ It is a temperature and humidity sensor that generates calibrated digital output for temperature and humidity. It uses a humidity sensor and thermistor to measure surrounding environment. It has fast response and excellent quality.


### Module-2:

In the second module we are using Arduino as a microcontroller that will get the soil moisture through a moisture sensor and publish it to MQTT broker by using ESP8266 Wi-Fi module.

#### Component Used:

__•    Arduino Uno:__ It is a microcontroller board based on the ATmega328. It has 14 digital I/O pins and 6 analog pins. It just need to connect with computer with a USB cable to upload the code and powered through a AC to DC adapter or a battery.

__•    YL-69:__ It is soil moisture sensor used to measure water content of soil. It can detect whether the soil is too dry or wet. This sensor uses the two probes to pass current through the soil, and then it reads that resistance to get the moisture level.

__•    ESP8266 ESP-01__

### Module-3:

In this module ESP8266 WiFi Module works as a subscriber who subscribes the moisture data from the MQTT Broker(Rasberry Pi). ESP8266 connected to relay i.e connected to motor and it will turn motor ON/OFF on the basis of soil moisture.


#### Component Used:

__•    Relay:__ A relay is is an electrically operated switch. It means that it can be turned on or off, letting the current going through or not. when a relay contact is normally open (NO), there is an open contact when the relay is not energized. When a relay contact is Normally Closed (NC), there is a closed contact when the relay is energized.

__•    ESP8266 ESP-01__


### Raspberry Pi as Broker:

MQTT (Message Queue Telemetry Transport) is lightweight messaging protocol for small devices and sensors. It is a publisher-Subscriber based model. Publisher can publish data to broker and subscriber can subscribe topic from broker. So, broker plays a very important role as a middle man.

In this project we are creating raspberry pi as a broker. To make it broker, we are using a Mosquitto MQTT i.e open source implementation of message broker. Raspberry Pi as a broker receives Temperature and Humidity data with a specific topic from arduino microcontroller as described in module1 and also receives soil moisture data with topic information as mentioned in module 2.

Now subscriber as module 3 here can subscribe data from broker by specifying topic name.

## Code Details:
This project is based on two area __North__ and __South__ both are using their own module 1 & 2 to publish temperature, humidity and moisture.

__Subscriber (Module-3)__ : it is using two channel Realy that will turn on/off motors on the basis of soil moisture of a particular area.

### Visulization:

I am using [ThingsBoard](https://thingsboard.io/) to visualize data i.e great open-source IoT platform for data visulalize real time data.
__dataSubscriber.py__: This Script is used to subscribe data of all areas from the broker and write it into different text files.

__thingsboardNorth.py and thingsboardSouth.py__: These Scripts will scrap data from text files and upload it to ThingsBoard Server.
[ThingsBoard Dashboard](/visualization/screenshot4.jpg)

[Link To Dashboard](https://demo.thingsboard.io/dashboards/d7455310-065d-11e8-83e6-1d8d2edf4f93?publicId=2ade3530-f6a2-11e7-abe9-1d8d2edf4f93)

## Wiring:

### Module 1

__ESP8266 ESP-01__  

Vcc     --------------------------------------     3.3V  
Gnd    --------------------------------------     Gnd  
Pin 2   --------------------------------------     DHT Data pin  

__DHT11__  
Vcc      --------------------------------------  5V  
Gnd     --------------------------------------   Gnd  
Data    --------------------------------------  ESP Pin 2  

### Module 2

__YL-69 Soil Sensor__                 __Arduino UNO__  
Vcc    --------------------------------------   5V  
Gnd   --------------------------------------   Gnd  
A0     ---------------------------------------  A0  


__ESP8266 ESP-01__                    __Arduino UNO__  
Vcc     ---------------------------------------    3.3V  
Gnd     --------------------------------------     Gnd  
Rx      ---------------------------------------    Pin 7  
Tx      ---------------------------------------    Pin 8  

### Module 3

__Relay(2 Channel)__  
IN1        ------------------------    ESP pin 0  
IN1        ------------------------    ESP pin 2  
Vcc       ------------------------    5V  
Gnd      ------------------------    Gnd  
