/*

  o     o   o     o
  GND             RX


  TX    3.3       3.3
  o     o   o     o


*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define ON   0
#define OFF  1

// replace with your Wi-Fi ssid and Password
const char* WIFI_SSID= "YourWifiName";
const char* WIFI_PASSWORD= "YourWifiPassword";

// change it to your raspberry ip adddress
const char* mqttServer="IP_address";
const int mqttPort= 1883; // also change port if it is different

// the relays connect to
int northRealyPin = 0;
int southRealyPin =2;

// you can adjust the soil moisture threshold value
int thresholdValue = 50;

WiFiClient espClient;
PubSubClient client(espClient);

// Allocating buffer memeory for JSON
StaticJsonBuffer<100> JSONBuffer;

void setup()
{
  Serial.begin(115200);
  
  pinMode(northRealyPin, OUTPUT);
  pinMode(southRealyPin, OUTPUT);
  
  setRelayStatus(northRealyPin, OFF);           //turn off north the relay as default
  setRelayStatus(southRealyPin, OFF);           //turn off south the relay as default

  // connecting to Wi-Fi
  InitWiFi();

  // Setting Server, port and callback function
  client.setServer( mqttServer, mqttPort );
  client.setCallback(callback);
  
  // connecting to broker
  if ( !client.connected() )
  {
    reconnect();
  }

  // subscribing required topics
  client.subscribe("north/moisture");
  client.subscribe("south/moisture");   
}

void loop()
{
  if ( !client.connected() ){
    reconnect();
  }

  if( WiFi.status() != WL_CONNECTED){
    InitWiFi();
  }
  
  client.loop(); 
}

// callback fuction is called whenever data comes in subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));

  // converting the payload into a string
  String response="";
  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(response);

  // parsing json data
  JsonObject& parsed = JSONBuffer.parseObject(response);

  // checking if any error occurs
  if (!parsed.success()) {   
    Serial.println("Parsing failed");
    delay(5000);
    return;
    }

  int moisture = parsed["moisture"];
  String area = parsed["area"];

  Serial.print("Moisture in ");
  Serial.print(area);
  Serial.print(": ");
  Serial.println(moisture);
  // passing values to control relay 
  // on the basis of moisture value
  controlRelay(area, moisture);

  // free buffer memory
  JSONBuffer.clear();
}


void controlRelay(String area, int moisture)
{
  if(area == "north"){
    if(moisture < thresholdValue){
      setRelayStatus(northRealyPin, ON);    // turning on north motor if moisture is lower than threshold value
      }else{
        setRelayStatus(northRealyPin, OFF);    // otherwise off
        }
    
  }else if(area == "south"){
    if(moisture < thresholdValue){
      setRelayStatus(southRealyPin, ON);       // turning on south motor if moisture is lower than threshold value
      }else{
        setRelayStatus(southRealyPin, OFF);   // otherwise off
      }
  }
    
}

void InitWiFi()
{
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(WIFI_SSID);
  // attempt to connect to WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while ( WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WIFI Network");
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Connecting to Raspberry Pi MQTT Server ...");
    
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Relay Subscriber"))
    {
      Serial.println( "Connected to MQTT Server." );
    }
    else
    {
      Serial.print( "Failed with status" );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

void setRelayStatus(int relayPin, unsigned char relayStatus)
{
  digitalWrite(relayPin, relayStatus);  
}
