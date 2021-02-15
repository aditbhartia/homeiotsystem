"""EE 250L Lab 11 Final Project

sensor_processing.py: Sensor data processing.

TODO: List team members here.
Adit Bhartia
Mia Ahmady
TODO: Insert Github repository link here.
https://github.com/usc-ee250-fall2019/finalproj-lab11-aditmia/tree/finalproj
"""

import paho.mqtt.client as mqtt
import time
import datetime
import logging
import requests
import json

topic = "abhartia/light"
lightStatus = 0
threshold = 500

def on_connect(client, userdata, flags, rc):
    print("Connected to server Code: " + str(rc))
    client.subscribe(topic)
    client.message_callback_add(topic, lightCallback)

#default callback
def on_message(client, userdata, msg):
    print("Default callback: " + msg.topic + " " + str(msg.payload, "utf-8"))

#callback on specific topic
def lightCallback(client, userdata, msg):
    
    logging.debug("Light Callback: " + str(msg.payload, "utf-8"))
    global lightStatus
    
    #if light is off and we recieve a message that indicates light is now on
    if int(msg.payload) >= threshold and lightStatus == 0:
        
        #format parameters
        params = {
            "time": str(datetime.datetime.now()),
            "event": "LIGHTS_ON",
            "room": "vhe_205"
        }

        logging.debug(json.dumps(params, sort_keys=True, indent=4))

        #send to web server
        response = requests.post('http://127.0.0.1:5000/event', json=params)

        #if server successfully recieved message, update lightStatus to 1 (on)
        if response.status_code == 200:
            data = response.json()
            logging.debug(json.dumps(data, sort_keys=True, indent=4))
            lightStatus = 1

        else:
            print('error: got response code %d' % response.status_code)
            print(response.text)

    #if light is on, and recieve message that light is off
    elif int(msg.payload) < threshold and lightStatus == 1:

        params = {
            "time": str(datetime.datetime.now()),
            "event": "LIGHTS_OFF",
            "room": "vhe_205"
        }

        response = requests.post('http://127.0.0.1:5000/event', json=params)

        if response.status_code == 200:
            data = response.json()
            logging.debug(json.dumps(data, sort_keys=True, indent=4))
            lightStatus = 0

        else:
            print('error: got response code %d' % response.status_code)
            print(response.text)


if __name__ == '__main__':

    #used logging for easy debugging
    logging.basicConfig(level = logging.DEBUG)

    client = mqtt.Client()
    client.on_message = on_message
    client.on_connect = on_connect
    #connect to mqtt broker
    client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
    client.loop_start()

    while True:
        time.sleep(1)



        
