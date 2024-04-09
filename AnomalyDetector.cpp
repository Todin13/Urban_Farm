#include "AnomalyDetector.h"

bool AnomalyDetector::isAnomalous(const SensorData& data) {
    // Implement the logic to detect anomalies based on temperature and humidity
    const float temperatureThresholdLow = 10.0f;
    const float temperatureThresholdHigh = 40.0f;

    if (data.temperature < temperatureThresholdLow || data.temperature > temperatureThresholdHigh) {
        return true;
    }

    // Implement additional checks for humidity and other conditions if necessary

    return false;
}