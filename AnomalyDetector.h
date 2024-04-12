#ifndef ANOMALYDETECTOR_H
#define ANOMALYDETECTOR_H

#include "SensorData.h"
#include <string>

class AnomalyDetector {
public:
    bool isAnomalous(const SensorData& data);
    void analyzeData();
    bool isRateOfChangeAnomalous(const SensorData& currentData, const SensorData& previousData);
    std::string getEnvVar(const std::string &key, const std::string &defaultValue);
};

#endif // ANOMALYDETECTOR_H