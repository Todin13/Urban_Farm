#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <string>
#include <chrono>

class SensorData {
public:
    int sensorID;
    std::string sensorVersion;
    float temperature;
    float humidity;
    int plantID;
    std::chrono::system_clock::time_point timeOfRecord;
};

#endif // SENSORDATA_H