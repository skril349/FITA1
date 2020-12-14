#https://zellwk.com/blog/crud-express-mongodb/
#https://pubsubclient.knolleary.net/api#callback
#https://reprap.org/forum/read.php?424,856363,856402#msg-856402
#!/usr/bin/python
# -*- coding: utf-8 -*-
import paho.mqtt.publish as publish
from sense_hat import SenseHat
import time
import subprocess

sense = SenseHat()
value = 0


while True:
    value = value+1
    temp = (sense.get_temperature_from_pressure())
    pressure = sense.get_pressure()/1000
    humidity = sense.get_humidity()
    
    #cpu_temp = subprocess.check_output("vcgencmd measure_temp", shell=True)
    #print(cpu_temp)
    
    #temp_calibrated_c = temp - ((cpu_temp - temp)/5.466)

    print("missatge {0}= Temperatura Enviada".format(value))
    msgs = [{'topic': "Temperatura", 'payload': "{0:.4f}".format(temp)},
            {'topic': "Humitat", 'payload': "{0:.4f}".format(humidity)},
            {'topic': "Pressure", 'payload': "{0:.4f}".format(pressure)}
            ]
    publish.multiple(msgs, hostname="test.mosquitto.org", port=1883)
    time.sleep(2)
