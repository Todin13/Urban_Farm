from flask import Flask, request
from msgpack import unpackb
from base64 import b64decode
from insertion_donnees import *


data_collection = Flask(__name__)

@data_collection.route('/receive', methods=['POST'])
def receive_data():
    try:
        data = request.data
        if data[0]==47: # ord("/") = 47
            data = data[1:]
        if data[-1]=="=":
            data = data[:-1]
        data = unpackb(b64decode(data))
        data = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
        	for key, value in data.items()}
        keys = data.keys()
        if "sensor_id" in keys:
            data["Sensor_ID"] = data.pop("sensor_id")
        if "sensor_version" in keys:
            data["Sensor_Version"] = data.pop("sensor_version")
        if "plant_id" in keys:
            data["Plant_ID"] = data.pop("plant_id")
        if "time" in keys:
            data["Time"] = data.pop("time")
        if "measures" in data:
            measures = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
            		for key, value in data.pop("measures").items()}
            if "temperature" in measures.keys():
            	if measures["temperature"][-1]=="K":
            	    data["Temperature"] = float(measures["temperature"][:-2])-273.15
            	elif measures["temperature"][-1]=="F":
            	    data["Temperature"] = (float(measures["temperature"][:-2])-32)/1.8
            	elif measures["temperature"][-1]=="C":
            	    data["Temperature"] = measures["temperature"][:-2]
            	else:
            	    print(f"Données non exploitables : {request.data}\nLa valeur de la température (\"{measures['temperature']}\") est invalide.")
            	    return {}
            if "humidite" in measures.keys():
            	data["Humidity"] = measures["humidite"][:-1]
            elif "humidity" in measures.keys():
            	data["Humidity"] = measures["humidity"][:-1]
        inserer(data)
        return data
    except:
        print(f"Données non exploitables : {request.data}")
        return {}

data_collection.run(host='0.0.0.0', port=8080)
