# This Script will only run once to send latitude and longitude to ThingsBoard
# to show the sensor location on map.

import paho.mqtt.client as mqtt
import json

THINGSBOARD_HOST = 'demo.thingsboard.io'

############### Sending North location ###########################################################

# north device access token
ACCESS_TOKEN = 'NorthDeviceAccessToken'

def on_connect(client, userdata, flags, rc):

    if rc == 0:

        print("Connected to broker")

    else:
        print("Connection failed")

# Replace with your sensor location
north = {'latitude': 26.789160, 'longitude': 75.826810}  

client = mqtt.Client("Python")
client.on_connect= on_connect

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port
client.connect(THINGSBOARD_HOST, 1883)

print("North: ", north)

# Sending north location to ThingsBoard
client.publish('v1/devices/me/attributes', json.dumps(north))

client.disconnect()

############################## Sending South location ##################################################

# Access token for south
ACCESS_TOKEN = 'SouthDeviceAccessToken'

# Replace with your sensor location
south = {'latitude': 26.788666, 'longitude': 75.827114}
client = mqtt.Client("Python")
client.on_connect= on_connect
client.username_pw_set(ACCESS_TOKEN)

client.connect(THINGSBOARD_HOST, 1883)

print("sensor_data: ", south)

# Sending south location to ThingsBoard
client.publish('v1/devices/me/attributes', json.dumps(south))

client.disconnect()
