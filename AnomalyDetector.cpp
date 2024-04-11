#include "AnomalyDetector.h"
#include "DatabaseConnector.h"
#include <iostream>

bool AnomalyDetector::isAnomalous(const SensorData& data) {
    const float temperatureThresholdLow = 10.0f;
    const float temperatureThresholdHigh = 40.0f;
    const float humidityThresholdLow = 10.0f;
    const float humidityThresholdHigh = 80.0f;

    if (data.temperature < temperatureThresholdLow || data.temperature > temperatureThresholdHigh) {
        std::cout << "Anomaly detected for sensor ID: " << data.sensorID << " for plant ID: " << data.plantID
            << ". Reason: Temperature out of range (" << data.temperature << "°C)." << std::endl;
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



void AnomalyDetector::analyzeData()
{
    //std::cout << "Test 0" << std::endl;
    const std::string connectionString = "host=localhost port=5433 dbname=urbanfarm user=admin password=urbanfarm123";
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
            << ". Reason: Temperature change too rapid (" << temperatureChange << "°C change)." << std::endl;
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

