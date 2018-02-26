import paho.mqtt.client as mqttClient
import time
import json

def dumpData(payload):

""" this fuction will grab data and write it to their respective txt file
    in key value format """

    # loading json data
    data = json.loads(payload)
    print data
    if data["area"] == 'north':
        if 'temperature' in data:
            temperature = data['temperature']
            humidity = data['humidity']
            fileStr = "{" + '"temperature":' + str(temperature) + ',' + '"humidity":' + str(humidity) + '}'
            fileObj = open('northTemperatureAndHumidity.txt', 'w')
            fileObj.write(fileStr)
            fileObj.close()
            return "Temperature and Humidity updated in North"
        elif 'moisture' in data:
            moisture = data['moisture']
            fileStr = "{" + '"moisture":' + str(moisture) + '}'
            fileObj = open('northMoisture.txt', 'w')
            fileObj.write(fileStr)
            fileObj.close()
            return "Moisture updated in North"
    elif data['area'] == 'south':
        if 'temperature' in data:
            temperature = data['temperature']
            humidity = data['humidity']
            fileStr = "{" + '"temperature":' + str(temperature) + ',' + '"humidity":' + str(humidity) + '}'
            fileObj = open('southTemperatureAndHumidity.txt', 'w')
            fileObj.write(fileStr)
            fileObj.close()
            return "Temperature and Humidity updated in South"
        elif 'moisture' in data:
            moisture = data['moisture']
            fileStr = "{" + '"moisture":' + str(moisture) + '}'
            fileObj = open('southMoisture.txt', 'w')
            fileObj.write(fileStr)
            fileObj.close()
            return "Moisture updated in South"

def on_connect(client, userdata, flags, rc):

    if rc == 0:

        print("Connected to broker")

        global Connected                #Use global variable
        Connected = True                #Signal connection

    else:

        print("Connection failed")

def on_message(client, userdata, message):
    print "Message received: "  + message.payload
    dumpMessage = dumpData(message.payload)
    print dumpMessage

Connected = False   #global variable for the state of the connection


broker_address=	"IP_Address"  #Broker IP address
port = 1883                         #Broker port

client = mqttClient.Client("Ankit")               #create new instance

client.on_connect= on_connect                      #attach function to callback
client.on_message= on_message                      #attach function to callback

client.connect(broker_address, port=port)          #connect to broker

client.loop_start()        #start the loop


while Connected != True:    #Wait for connection
    time.sleep(0.1)

client.subscribe("north/+")  # Subscribe all north topics
client.subscribe("south/+")  # Subscribe all south topics

try:
    while True:
        time.sleep(1)

except KeyboardInterrupt:
    print "exiting"
    client.disconnect()
    client.loop_stop()
