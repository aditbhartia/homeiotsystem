## Light Sensor IoT System - Adit + Mia

This is the final project for my IoT class that I worked on with a classmate. 

These files encompass an implementation of a basic IoT system. It is meant to simulate a light sensor being attached to a centralized home automation system that can process and log events. The overall system has an analog Grove Light Sensor connected to an ADC on an OpenMote. This OpenMote then sends and receives UDP packets with the light sample and a specified MQTT topic to/from a central OpenMote. This central OpenMote will use the information provided in the packets to publish to an MQTT broker. 

A seperate machine that subscribes to the specified topic will process these values and send POST requests over HTTP to a server for logging.

The polling of the ADC and UDP communications that run on the OpenMote are built ontop of Riot OS. For the purposes of this repo, I have just focused on the files we created. You can run the .c files ontop of Riot OS. 

- `main.c, udp_rx.c, udp_rx.h` are meant to be run on the OpenMote. One thread polls the ADC for sensor data and sends hte UDP packet to the relayer node. The other packet waits for sleep command UDP packets and puts the mote to sleep for a specified period. 

- `sensor_processing.py` has the logic for subscribing to the specific MQTT topic and processes the data stream to check when the light is turned off/on. For each event, a HTTP post request is sent to the web server. 

- `web_server.py` logs all the on/off events and displays the history at `localhost:5000/history`. 

The preprocessor is used to compile for either a `native` or `openmote-b` target. 

For `native`, create a tap interface first: 

    sudo ip tuntap add tap0 mode tap user ${USER}
    sudo ip link set tap0 up


