# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import requests
#from pymongo import MongoClient
ipadress = '192.168.11.218'
#cliente = MongoClient("mongodb+srv://skril:Pereieva1@cluster0-eufis.mongodb.net/test?retryWrites=true&w=majority")
#db = cliente.get_database('Student')
#Temperatura = db.Temperatures
#Lluminositat = db.Lluminositat
#Humitat = db.Humitat

#Lluminositat.insert_one({"Lluminositat":"3"})
#Humitat.insert_one({"Humitat":"5"})
    
def on_connect(client, userdata, flags, rc):  # The callback for when the client connects to the broker
    print("Connected with result code {0}".format(str(rc)))  # Print result of connection attempt
    client.subscribe([("Temperatura",0),("Humitat",0),("Pressure",0)])  # Subscribe to the topic “digitest/test1”, receive any messages published on it
    #client.subscribe("Humitat")
    #client.subscribe("Lluminositat") 


def on_message(client, userdata, msg):  # The callback for when a PUBLISH message is received from the server.
    #temp = {"Temperatura":"{0}".format(str(msg.payload))}
    #Temperatura.insert_one(temp)
    print("Dades rebudes-> " + msg.topic + " " + str(msg.payload))  # Print a received msg

    if(msg.topic == "Temperatura"):
        temp = {"Temperatura":"{0}".format(str(msg.payload))}
        cadena = 'http://iotlab.euss.es/cloud/guardar_dades_adaptat.php?id_sensor=81&valor={0}&temps='.format(str(msg.payload))
        print(cadena)
        r =requests.get(cadena)
        print(r)
        #Temperatura.insert_one(temp)
        
    if(msg.topic == "Humitat"):
        hum = {"Humitat":"{0}".format(str(msg.payload))}
        cadena1 = 'http://iotlab.euss.es/cloud/guardar_dades_adaptat.php?id_sensor=82&valor={0}&temps='.format(str(msg.payload))
        print(cadena1)
        r1 =requests.get(cadena1)
        print(r1)
       # Humitat.insert_one(hum)
        
    if(msg.topic == "Pressure"):
        llum = {"Pressure":"{0}".format(str(msg.payload))}
        cadena2 = 'http://iotlab.euss.es/cloud/guardar_dades_adaptat.php?id_sensor=83&valor={0}&temps='.format(str(msg.payload))
        print(cadena2)
        r2 =requests.get(cadena2)
        print(r2)
       # Lluminositat.insert_one(llum)    


client = mqtt.Client("digi_mqtt_test")  # Create instance of client with client ID “digi_mqtt_test”
client.on_connect = on_connect  # Define callback function for successful connection
client.on_message = on_message  # Define callback function for receipt of a message
# client.connect("m2m.eclipse.org", 1883, 60)  # Connect to (broker, port, keepalive-time)
client.connect("test.mosquitto.org", 1883) 
#client.loop_forever()  # Start networking daemon
client.loop_forever()

