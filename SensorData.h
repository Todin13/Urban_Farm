#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <string>
#include <chrono>

// A simple struct to represent sensor data
struct SensorData {
    int sensorID;                // Corresponds to the sensor_id in the database
    std::string sensorVersion;   // Corresponds to the sensor_version in the database
    int plantID;                 // Corresponds to the plant_id in the database
    float temperature;           // Corresponds to the temperature in the database
    float humidity;              // Corresponds to the humidity in the database
    // Add a std::chrono::time_point or similar field for the time
    std::chrono::system_clock::time_point timeOfRecord;
    // Add a boolean field for the processed status
    bool processed;
    
    bool isValid; // New field to indicate if the data is valid

    // Constructor
    SensorData() : sensorID(0), plantID(0), temperature(0.0f), humidity(0.0f), isValid(false) {}

    // Add more fields as necessary
};

#endif // SENSORDATA_H
