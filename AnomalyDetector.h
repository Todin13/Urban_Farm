#ifndef ANOMALYDETECTOR_H
#define ANOMALYDETECTOR_H

#include "SensorData.h"

class AnomalyDetector {
public:
    bool isAnomalous(const SensorData& data);
    void analyzeData();
    bool isRateOfChangeAnomalous(const SensorData& currentData, const SensorData& previousData);

};

#endif // ANOMALYDETECTOR_H