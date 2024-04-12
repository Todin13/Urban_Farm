#ifndef WARNINGREPORTER_H
#define WARNINGREPORTER_H

#include "SensorData.h"

class WarningReporter {
public:
    static void report(const SensorData& data);
};

#endif // WARNINGREPORTER_H