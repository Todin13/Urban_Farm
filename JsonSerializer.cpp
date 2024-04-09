#include "JsonSerializer.h"

SensorData JsonSerializer::deserialize(const web::json::value& jsonValue) {
    SensorData data;
    data.sensorID = jsonValue.at(U("sensor_ID")).as_integer();
    data.sensorVersion = jsonValue.at(U("sensor_Version")).as_string();
    data.temperature = jsonValue.at(U("Temperature")).as_double();
    data.humidity = jsonValue.at(U("Humidity")).as_double();
    data.plantID = jsonValue.at(U("Plant_ID")).as_integer();
    // Deserialize timeOfRecord from the JSON value
    return data;
}

web::json::value JsonSerializer::serialize(const SensorData& data) {
    web::json::value jsonValue;
    jsonValue[U("sensor_ID")] = web::json::value::number(data.sensorID);
    jsonValue[U("sensor_Version")] = web::json::value::string(data.sensorVersion);
    jsonValue[U("Temperature")] = web::json::value::number(data.temperature);
    jsonValue[U("Humidity")] = web::json::value::number(data.humidity);
    jsonValue[U("Plant_ID")] = web::json::value::number(data.plantID);
    // Serialize timeOfRecord to the JSON value
    return jsonValue;
}