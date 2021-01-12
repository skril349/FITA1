#https://zellwk.com/blog/crud-express-mongodb/
#https://pubsubclient.knolleary.net/api#callback
#https://reprap.org/forum/read.php?424,856363,856402#msg-856402
#!/usr/bin/python
# -*- coding: utf-8 -*-
import paho.mqtt.publish as publish
from sense_hat import SenseHat
import time
import subprocess
import os

def get_cpu_temp():
  res = os.popen("vcgencmd measure_temp").readline()
  t = float(res.replace("temp=","").replace("'C\n",""))
  return(t)


sense = SenseHat()
value = 0


while True:
    value = value+1
    #temp = (sense.get_temperature_from_pressure())
    pressure = sense.get_pressure()/1000
    humidity = sense.get_humidity()
    t = sense.get_temperature_from_humidity()
    t_cpu = get_cpu_temp()
    temp = t - ((t_cpu-t)/0.9)
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
