from flask import Flask, request, jsonify
from msgpack import unpackb
from base64 import b64decode
import os
import logging
from insertion_donnees import *

data_collection = Flask(__name__)
@data_collection.route(os.getenv("API_PATH"), methods=['POST'])
def receive_data():
    try:
        data = request.data

        # Clean up data if necessary
        if data[0] == 47:  # ord("/") = 47
            data = data[1:]
        if data[-1] == "=":
            data = data[:-1]

        # Decode and unpack data
        data = unpackb(b64decode(data))
        data = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
                for key, value in data.items()}
        # Rename keys if necessary
        keys = data.keys()
        if "sensor_id" in keys:
            data["Sensor_ID"] = data.pop("sensor_id")
        if "sensor_version" in keys:
            data["Sensor_Version"] = data.pop("sensor_version")
        if "plant_id" in keys:
            data["Plant_ID"] = data.pop("plant_id")
        if "time" in keys:
            data["Time"] = data.pop("time")

        # Process measures
        if "measures" in data:
            measures = {key.decode("utf-8") if isinstance(key, bytes) else key: value.decode("utf-8") if isinstance(value, bytes) else value
                        for key, value in data.pop("measures").items()}

            # Convert temperature units
            if "temperature" in measures.keys():
                temperature = measures["temperature"]
                unit = temperature[-1]
                value = float(temperature[:-2])

                if unit == "K":
                    data["Temperature"] = value - 273.15
                elif unit == "F":
                    data["Temperature"] = (value - 32) / 1.8
                elif unit == "C":
                    data["Temperature"] = value
                else:
                    logging.error(f"Invalid temperature value: {temperature}")
                    return jsonify(message="Invalid temperature value"), 400

            # Process humidity
            if "humidite" in measures.keys():
                data["Humidity"] = measures["humidite"][:-1]
            elif "humidity" in measures.keys():
                data["Humidity"] = measures["humidity"][:-1]
        inserer(data)
        return jsonify(message="Data received successfully"), 200
    except Exception as e:
        logging.error(f"Error processing data: {e}")
        return jsonify(message="An error occurred"), 500

if __name__ == "__main__":
    data_collection.run(host=os.getenv("API_HOST"), port=os.getenv("API_PORT"))
