#include "AnomalyDetector.h"
#include "DatabaseConnector.h"
#include <iostream>
#include <cstdlib>  // For getenv()
#include <string>

bool AnomalyDetector::isAnomalous(const SensorData& data) {
    const float temperatureThresholdLow = 10.0f;
    const float temperatureThresholdHigh = 40.0f;
    const float humidityThresholdLow = 10.0f;
    const float humidityThresholdHigh = 80.0f;

    if (data.temperature < temperatureThresholdLow || data.temperature > temperatureThresholdHigh) {
        std::cout << "Anomaly detected for sensor ID: " << data.sensorID << " for plant ID: " << data.plantID
            << ". Reason: Temperature out of range (" << data.temperature << "�C)." << std::endl;
        return true;
    }

    if (data.humidity < humidityThresholdLow || data.humidity > humidityThresholdHigh) {
        std::cout << "Anomaly detected for sensor ID: " << data.sensorID << " for plant ID: " << data.plantID
            << ". Reason: Humidity out of range (" << data.humidity << "%)." << std::endl;
        return true;
    }

    // Add other conditions

    return false;
}


std::string AnomalyDetector::getEnvVar(const std::string &key, const std::string &defaultValue) {
    const char* val = getenv(key.c_str());
    if (val == nullptr) {  // Variable not found
        return defaultValue;
    }
    return std::string(val);
}

void AnomalyDetector::analyzeData()
{
    //std::cout << "Test 0" << std::endl;
    // Initialize the database connector
    std::string host = getEnvVar("DB_HOST", "localhost");
    std::string port = getEnvVar("DB_PORT", "5433");
    std::string dbname = getEnvVar("DB_NAME", "urbanfarm");
    std::string user = getEnvVar("DB_USER", "admin");
    std::string password = getEnvVar("DB_PASSWORD", "urbanfarm123");

    std::string connectionString = "host=" + host +
                                   " port=" + port +
                                   " dbname=" + dbname +
                                   " user=" + user +
                                   " password=" + password;

    std::cout << "Connection String: " << connectionString << std::endl;
    DatabaseConnector dbConnector(connectionString);
    dbConnector.fetchAndAnalyzeData(); // Implement this method to fetch new data and analyze it
}

bool AnomalyDetector::isRateOfChangeAnomalous(const SensorData& currentData, const SensorData& previousData) {
    if (!previousData.isValid) {
        // No previous data to compare with
        return false;
    }

    double temperatureChange = std::abs(currentData.temperature - previousData.temperature);
    double humidityChange = std::abs(currentData.humidity - previousData.humidity);

    const double temperatureChangeThreshold = 5.0;
    const double humidityChangeThreshold = 10.0;

    if (temperatureChange > temperatureChangeThreshold) {
        std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID << " for plant ID: " << currentData.plantID
            << ". Reason: Temperature change too rapid (" << temperatureChange << "�C change)." << std::endl;
        return true;
    }

    if (humidityChange > humidityChangeThreshold) {
        std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID << " for plant ID: " << currentData.plantID
            << ". Reason: Humidity change too rapid (" << humidityChange << "% change)." << std::endl;
        return true;
    }

    // Add other rate of change checks

    return false;
}

