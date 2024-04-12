#ifndef ANOMALYDETECTOR_H
#define ANOMALYDETECTOR_H

#include "SensorData.h"
#include <string>
#include <pqxx/pqxx> // Make sure to include the pqxx header for the work class.

class AnomalyDetector {
public:
    // Constructor does not need to store dbConnector anymore
    AnomalyDetector() = default;

    // Functions that will perform actions on the database receive a transaction reference
    explicit AnomalyDetector(DatabaseConnector& connector) : dbConnector(connector) {}
    void analyzeData(pqxx::work& txn);
    bool isAnomalous(const SensorData& data, pqxx::work& txn);
    bool isRateOfChangeAnomalous(const SensorData& currentData, const SensorData& previousData, pqxx::work& txn); 
    static std::string getEnvVar(const std::string &key, const std::string &defaultValue);

    // No need to store dbConnector here, functions will receive a transaction reference

private:
    DatabaseConnector& dbConnector;
};

#endif // ANOMALYDETECTOR_H