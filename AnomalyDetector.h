#ifndef ANOMALYDETECTOR_H
#define ANOMALYDETECTOR_H

#include "SensorData.h"

class AnomalyDetector {
public:
    bool isAnomalous(const SensorData& data);
};

#endif // ANOMALYDETECTOR_H