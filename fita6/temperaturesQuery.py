#!/usr/bin/python
import sqlite3
import json
con = sqlite3.connect('temperatures.db')
Temperatures = []
data = []
with con:
    cur = con.cursor()
    cur.execute("SELECT * FROM fita6mqtt WHERE Id == 82")
    while True:
        row = cur.fetchone()
        if row == None:
            break
        #print(row[2])
        Temperatures.append(row[2])
    print(Temperatures)
    with open("lectures.json") as json_file:
        data = json.load(json_file)
        data["temperatura"].append(Temperatures[-1])
        print(data)
    with open("lectures.json", "w") as jsonFile:
        json.dump(data, jsonFile, indent=4)
      
    del Temperatures[:]
    
