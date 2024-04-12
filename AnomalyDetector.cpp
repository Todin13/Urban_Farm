#include "DatabaseConnector.h"  // Ensure this comes first if it defines a type used in AnomalyDetector.h
#include "AnomalyDetector.h"
#include <iostream>
#include <cstdlib>  // For getenv()
#include <string>


bool AnomalyDetector::isAnomalous(const SensorData& data, pqxx::work& txn) {
    const float temperatureThresholdLow = 10.0f;
    const float temperatureThresholdHigh = 40.0f;
    const float humidityThresholdLow = 10.0f;
    const float humidityThresholdHigh = 80.0f;

    bool isAnomalyDetected = false;
    std::string reason;

    if (data.temperature < temperatureThresholdLow || data.temperature > temperatureThresholdHigh) {
        reason = "Temperature out of range (" + std::to_string(data.temperature) + "°C).";
        std::cout << "Anomaly detected for sensor ID: " << data.sensorID << " for plant ID: " << data.plantID
                  << ". Reason: " << reason << std::endl;
        dbConnector.insertWarning(txn, data.sensorID, reason);
        isAnomalyDetected = true;
    }

    if (data.humidity < humidityThresholdLow || data.humidity > humidityThresholdHigh) {
        reason = "Humidity out of range (" + std::to_string(data.humidity) + "%).";
        std::cout << "Anomaly detected for sensor ID: " << data.sensorID << " for plant ID: " << data.plantID
                  << ". Reason: " << reason << std::endl;
        dbConnector.insertWarning(txn, data.sensorID, reason);
        isAnomalyDetected = true;
    }

    return isAnomalyDetected;
}



std::string AnomalyDetector::getEnvVar(const std::string &key, const std::string &defaultValue) {
    const char* val = getenv(key.c_str());
    if (val == nullptr) {  // Variable not found
        return defaultValue;
    }
    return std::string(val);
}

void AnomalyDetector::analyzeData(pqxx::work& txn) {
    std::cout << "Analyzing data..." << std::endl;
    // Pass the transaction object to fetchAndAnalyzeData
    dbConnector.fetchAndAnalyzeData(txn);
}

bool AnomalyDetector::isRateOfChangeAnomalous(const SensorData& currentData, const SensorData& previousData, pqxx::work& txn) {
    if (!previousData.isValid) {
        // No previous data to compare with
        return false;
    }

    double temperatureChange = std::abs(currentData.temperature - previousData.temperature);
    double humidityChange = std::abs(currentData.humidity - previousData.humidity);

    const double temperatureChangeThreshold = 5.0;
    const double humidityChangeThreshold = 10.0;

    bool anomalyDetected = false;
    std::string reason;

    if (temperatureChange > temperatureChangeThreshold) {
        reason = "Temperature change too rapid (" + std::to_string(temperatureChange) + "°C change).";
        std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID 
                  << " for plant ID: " << currentData.plantID << ". Reason: " << reason << std::endl;
        dbConnector.insertWarning(txn, currentData.sensorID, reason); // Pass transaction object to insertWarning
        anomalyDetected = true;
    }

    if (humidityChange > humidityChangeThreshold) {
        reason = "Humidity change too rapid (" + std::to_string(humidityChange) + "% change).";
        std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID 
                  << " for plant ID: " << currentData.plantID << ". Reason: " << reason << std::endl;
        dbConnector.insertWarning(txn, currentData.sensorID, reason); // Pass transaction object to insertWarning
        anomalyDetected = true;
    }

    return anomalyDetected;
}
