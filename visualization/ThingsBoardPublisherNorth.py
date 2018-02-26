# link to Dashboard: https://demo.thingsboard.io/dashboards/d7455310-065d-11e8-83e6-1d8d2edf4f93?publicId=2ade3530-f6a2-11e7-abe9-1d8d2edf4f93

import time
import paho.mqtt.client as mqtt
import json

THINGSBOARD_HOST = 'demo.thingsboard.io'     # ThingsBoard Host Address
ACCESS_TOKEN = 'NorthDeviceAccessToken'	     # Access Token for north device


def on_connect(client, userdata, flags, rc):

    if rc == 0:

        print("Connected to broker")

        global Connected                #Use global variable
        Connected = True                #Signal connection

    else:

        print("Connection failed")

sensor_data = {'temperature': 0, 'humidity': 0, 'moisture': 0}

client = mqtt.Client("Python")
client.on_connect= on_connect

# Set access token
client.username_pw_set(ACCESS_TOKEN)
# Connect to ThingsBoard using default MQTT port
client.connect(THINGSBOARD_HOST, 1883)

client.loop_start()

try:
    while True:
	# opening north Moisture file
        moistureFile = open('northMoisture.txt', 'r')
	# opening north temperature and humidity file
        temperatureAndHumidityFile = open('northTemperatureAndHumidity.txt', 'r')
	
	# Reading Data
        moistureData = moistureFile.read().strip()
        temperatureAndHumidityData = temperatureAndHumidityFile.read().strip()

	# loading data into Json format
        moistureData = json.loads(moistureData)
        temperatureAndHumidityData = json.loads(temperatureAndHumidityData)
		
	# Closing all files
        moistureFile.close()
        temperatureAndHumidityFile.close()

        humidity = temperatureAndHumidityData['humidity']
        temperature = temperatureAndHumidityData['temperature']
        moisture = moistureData['moisture']
        sensor_data['temperature'] = int(temperature)
        sensor_data['humidity'] = int(humidity)
        sensor_data['moisture'] = int(moisture)
        print("sensor_data: ", sensor_data)

        # Sending data to ThingsBoard
        client.publish('v1/devices/me/telemetry', json.dumps(sensor_data))
        time.sleep(7)       # delay for 7 seconds

except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
