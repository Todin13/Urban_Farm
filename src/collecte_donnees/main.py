from flask import Flask, request
from msgpack import unpackb
from base64 import b64decode
from insertion_donnees import *

data_collection = Flask(__name__)

@data_collection.route('/receive', methods=['POST'])
def receive_data():
    print(1000*".")
    try:
        data = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
        	for key, value in unpackb(b64decode(request.data)).items()}
        if "measures" in data:
            measures = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
            		for key, value in data.pop("measures").items()}
            if "temperature" in measures.keys():
            	data["temperature"] = measures["temperature"]
            if "humidite" in measures.keys():
            	data["humidite"] = measures["humidite"]
            elif "humidity" in measures.keys():
            	data["humidite"] = measures["humidity"]
        print("Données reçues : ", data)
        
        return data
    except:
        print(data)
        print("Les données reçues ne sont pas utilisables.")
        return {}

data_collection.run(host='0.0.0.0', port=8080)

#valeurs = ("ID", "Sensor_ID", "Sensor_Version", "Plant_ID", "Time", "Temperature", "Humidity")
