#ifndef METRICSCOLLECTOR_H
#define METRICSCOLLECTOR_H

#include <iostream>
#include <atomic>

class MetricsCollector {
private:
    std::atomic<int> totalRecordsProcessed;
    std::atomic<int> anomaliesDetected;
    std::atomic<int> recordsSkipped;

public:
    MetricsCollector() : totalRecordsProcessed(0), anomaliesDetected(0), recordsSkipped(0) {}

    void incrementProcessed() {
        totalRecordsProcessed++;
    }

    void incrementAnomalies() {
        anomaliesDetected++;
    }

    void incrementSkipped() {
        recordsSkipped++;
    }

    void logMetrics() const {
        std::cout << "Metrics - Total Processed: " << totalRecordsProcessed
                  << ", Anomalies Detected: " << anomaliesDetected
                  << ", Records Skipped: " << recordsSkipped << std::endl;
    }
};

#endif // METRICSCOLLECTOR_H
