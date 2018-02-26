/*
     ESP8266-01
 
  o     o   o     o
  GND             RX

  
  TX    3.3       3.3
  o     o   o     o
   
*/

#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"

#define WIFI_AP "YourWifiName"          // Replace with your WiFi name
#define WIFI_PASSWORD "YourWifiPassword"    // Replace with your WiFi password
#define server "IP_Address"         // Replace with your broker Ip Address

//ESP8266 TX is connected to Arduino Digital Pin 8
#define ESP8266_TX_PIN 8
 
//ESP8266 RX is connected to Arduino Digital Pin 7
#define ESP8266_RX_PIN 7


// Initialize the WiFi client object
WiFiEspClient espClient;
PubSubClient client(espClient);

                     // Rx        ,        Tx
SoftwareSerial soft(ESP8266_TX_PIN,ESP8266_RX_PIN);

int status = WL_IDLE_STATUS;

// select the input pin for the potentiometer 
int sensorAnalogPin = A0;     

// variable to store the value coming from the sensor
int moisture = 0;

void setup() 
{
    pinMode(sensorAnalogPin, INPUT);
    Serial.begin(9600);
    InitWiFi();
    client.setServer( server, 1883 );
}

void loop() 
{
  
  if(WiFi.status() != WL_CONNECTED){
   connectWiFi();
  }
  
  if ( !client.connected()){
    reconnect();
  } 
    
  // Call the function to publish to the Server
  publishToBroker();
  
  // Update and send only after 2 seconds
  delay(2000);
  
  client.loop();
}


void publishToBroker()
{
  // getting moisture value
  moisture = readSoilMoisture(sensorAnalogPin); 
  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"moisture\":"; 
  payload += moisture;
  payload += ","; 
  payload += "\"area\":"; 
  payload += "\"south\""; 
  payload += "}";

  
  char Data[200];
  payload.toCharArray( Data, 200 );
  
  // Send payload
  client.publish( "south/moisture", Data );
  Serial.println( Data );
}

void InitWiFi()
{
  // initialize serial for ESP module
  soft.begin(9600);
  // initialize ESP module
  WiFi.init(&soft);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  connectWiFi();
}

void connectWiFi(){
    
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.print(WIFI_AP);
    Serial.print(".");
    // Connect to WPA/WPA2 network
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    delay(500);
    }
  }

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Connecting to Broker...");
    
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Moisture South") ) 
    {
      Serial.println( "Connected" );
    } 
    else 
    {
      Serial.print( "Failed to connect with status = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

int readSoilMoisture(int sensorAnalogPin)
{

    
   // read the value from the sensor: 
  int sensorValue = analogRead(sensorAnalogPin);

  // coverting value in Scale of 0 to 100
  sensorValue = map(sensorValue,1023,0,0,100);

  Serial.print("Mositure : ");
  Serial.print(sensorValue);
  Serial.println("%");
  return (int)sensorValue;

}


