#include "DatabaseConnector.h"
#include <iostream>
#include "AnomalyDetector.h"
#include "SensorData.h"
#include <iomanip>
#include <ctime>
#include <chrono>

DatabaseConnector::DatabaseConnector(const std::string& connectionString)
    : dbConnection(connectionString) {
    // Constructor implementation
}


void DatabaseConnector::testQuery() {
    try {
        pqxx::work txn(dbConnection);
        pqxx::result r = txn.exec("SELECT 'Hello, world!' AS greeting");

        for (auto row: r) {
            std::cout << row["greeting"].c_str() << std::endl;
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


void DatabaseConnector::fetchSensorData() {
    try {
        pqxx::work txn(dbConnection);
        pqxx::result r = txn.exec("SELECT ID, Sensor_ID, Sensor_Version, Plant_ID, Time, Temperature, Humidity FROM SensorsData");

        std::cout << "Fetched Sensor Data:" << std::endl;
        for (auto row : r) {
            std::cout << "ID: " << row["ID"].as<int>()
                << ", Sensor_ID: " << row["Sensor_ID"].as<int>()
                << ", Sensor_Version: " << row["Sensor_Version"].as<std::string>()
                << ", Plant_ID: " << row["Plant_ID"].as<int>()
                << ", Time: " << row["Time"].as<std::string>()
                << ", Temperature: " << row["Temperature"].as<float>()
                << "°C, Humidity: " << row["Humidity"].as<float>() << "%"
                << std::endl;
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Error fetching sensor data: " << e.what() << std::endl;
    }
}


std::chrono::system_clock::time_point convertTimestamp(const std::string& timestamp) {
    std::tm tm = {};
    std::stringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Adjust format if necessary
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}


void DatabaseConnector::fetchAndAnalyzeData() {
    try {
        pqxx::work txn(dbConnection);
        pqxx::result r = txn.exec("SELECT * FROM sensorsdata WHERE processed = false");

        AnomalyDetector anomalyDetector;

        for (auto row : r) {
            // Construct a SensorData object from the row
            SensorData currentData;
            currentData.sensorID = row["sensor_id"].as<int>();
            currentData.sensorVersion = row["sensor_version"].as<std::string>();
            currentData.plantID = row["plant_id"].as<int>();
            currentData.temperature = row["temperature"].as<float>();
            currentData.humidity = row["humidity"].as<float>();
            currentData.timeOfRecord = convertTimestamp(row["time"].as<std::string>());

            // Fetch the most recent previous record for the same sensor
            SensorData previousData = fetchMostRecentSensorData(currentData.sensorID, txn);

            // Use the AnomalyDetector to check if the rate of change is anomalous
            if (anomalyDetector.isRateOfChangeAnomalous(currentData, previousData)) {
                //std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID << std::endl;
                // Optionally, take further actions like logging the anomaly or alerting
            }
            else if (anomalyDetector.isAnomalous(currentData)) {
                //std::cout << "Anomaly detected for sensor ID: " << currentData.sensorID << std::endl;
                // Optionally, take further actions like logging the anomaly or alerting
            }
            else {
                std::cout << "Entry ID# " << row["id"] << " intact" << std::endl;
            }

            // Update the processed flag for the current row
            txn.exec0("UPDATE sensorsdata SET processed = true WHERE id = " + txn.quote(row["id"].as<int>()));
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Database query failed: " << e.what() << std::endl;
    }
}



SensorData DatabaseConnector::fetchMostRecentSensorData(int sensorID, pqxx::work& txn) {
    SensorData previousData;
    pqxx::result r = txn.exec("SELECT * FROM sensorsdata WHERE sensor_id = " + txn.quote(sensorID) + " AND processed = true ORDER BY time DESC LIMIT 1");

    if (!r.empty()) {
        auto row = r[0];
        previousData.sensorID = row["sensor_id"].as<int>();
        previousData.sensorVersion = row["sensor_version"].as<std::string>();
        previousData.plantID = row["plant_id"].as<int>();
        previousData.temperature = row["temperature"].as<float>();
        previousData.humidity = row["humidity"].as<float>();
        previousData.timeOfRecord = convertTimestamp(row["time"].as<std::string>());
        previousData.isValid = true; // Set to true because we have valid data
    }
    return previousData;
}
