# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import requests
import sqlite3
import json
from sqlite3 import Error
from datetime import date
from datetime import datetime

TemperaturesJson = []
HumitatJson = []
PressureJson = []



def cadenaTime():
    today = date.today()
    now = datetime.now()
    d1 = today.strftime("%d/%m/%Y")
    current_time = now.strftime("%H:%M:%S")
    valueTemps=str(d1+' '+current_time)
    #print (valueTemps)
    return(valueTemps)


#from pymongo import MongoClient
ipadress = '192.168.11.218'
    
def sql_insert(con, entities):

    cursorObj = con.cursor()
    
    cursorObj.execute('INSERT INTO fita6mqtt(Id,Sensor,Valor,Temps) VALUES(?, ?, ?, ?)', entities)
    
    con.commit()
    
def sql_connection():
    try:
        con = sqlite3.connect('temperatures.db')
        return con
    except Error:
        print(Error)
        
def sql_table(con):
    cursorObj = con.cursor()
    cursorObj.execute("CREATE TABLE fita6mqtt(Id integer,Sensor text, Valor text, Temps text)")
    con.commit()
#per crear la taula executem les següents dues linies de comandes: 
con = sql_connection()
#sql_table(con)   
    
    
    
    
    
    
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
        #print(cadena)
        r =requests.get(cadena)
        str(msg.payload)
        #print(type(msg.payload))
        temps = str(cadenaTime())
        entities = (81, 'Temperatura', str(msg.payload),temps)
        sql_insert(con, entities)
        with con:
            cur = con.cursor()
            cur.execute("SELECT * FROM fita6mqtt WHERE Id == 81")
            while True:
                row = cur.fetchone()
                if row == None:
                    break
                #print(row[2])
                TemperaturesJson.append(row[2])
            #print(Temperatures)
            with open("lectures.json") as json_file:
                data = json.load(json_file)
                data["temperatura"].append(TemperaturesJson[-1])
                #print(data)
            with open("lectures.json", "w") as jsonFile:
                json.dump(data, jsonFile, indent=4)
              
            del TemperaturesJson[:]
    
        #print(r)
        #Temperatura.insert_one(temp)
        
    if(msg.topic == "Humitat"):
        hum = {"Humitat":"{0}".format(str(msg.payload))}
        cadena1 = 'http://iotlab.euss.es/cloud/guardar_dades_adaptat.php?id_sensor=82&valor={0}&temps='.format(str(msg.payload))
        #print(cadena1)
        r1 =requests.get(cadena1)
        #print(r1)
        temps = str(cadenaTime())
        entities = (82, 'Humitat', str(msg.payload),temps)
        sql_insert(con, entities)
        with con:
            cur = con.cursor()
            cur.execute("SELECT * FROM fita6mqtt WHERE Id == 82")
            while True:
                row = cur.fetchone()
                if row == None:
                    break
                #print(row[2])
                HumitatJson.append(row[2])
            #print(Temperatures)
            with open("lectures.json") as json_file:
                data = json.load(json_file)
                data["humitat"].append(HumitatJson[-1])
                #print(data)
            with open("lectures.json", "w") as jsonFile:
                json.dump(data, jsonFile, indent=4)
              
            del HumitatJson[:]
       # Humitat.insert_one(hum)
        
    if(msg.topic == "Pressure"):
        llum = {"Pressure":"{0}".format(str(msg.payload))}
        cadena2 = 'http://iotlab.euss.es/cloud/guardar_dades_adaptat.php?id_sensor=83&valor={0}&temps='.format(str(msg.payload))
        #print(cadena2)
        r2 =requests.get(cadena2)
        #print(r2)
        temps = str(cadenaTime())
        entities = (83, 'Pressure', str(msg.payload),temps)
        sql_insert(con, entities)
        with con:
            cur = con.cursor()
            cur.execute("SELECT * FROM fita6mqtt WHERE Id == 83")
            while True:
                row = cur.fetchone()
                if row == None:
                    break
                #print(row[2])
                PressureJson.append(row[2])
            #print(Temperatures)
            with open("lectures.json") as json_file:
                data = json.load(json_file)
                data["pressure"].append(PressureJson[-1])
                #print(data)
            with open("lectures.json", "w") as jsonFile:
                json.dump(data, jsonFile, indent=4)
              
            del PressureJson[:]
       # Lluminositat.insert_one(llum)    


client = mqtt.Client("digi_mqtt_test")  # Create instance of client with client ID “digi_mqtt_test”
client.on_connect = on_connect  # Define callback function for successful connection
client.on_message = on_message  # Define callback function for receipt of a message
# client.connect("m2m.eclipse.org", 1883, 60)  # Connect to (broker, port, keepalive-time)
client.connect("test.mosquitto.org", 1883) 
#client.loop_forever()  # Start networking daemon
client.loop_forever()

