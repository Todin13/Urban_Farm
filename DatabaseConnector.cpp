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

pqxx::connection& DatabaseConnector::getConnection() {
    return dbConnection; // return the reference to the database connection
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
                << "�C, Humidity: " << row["Humidity"].as<float>() << "%"
                << std::endl;
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Error fetching sensor data: " << e.what() << std::endl;
    }
}

void DatabaseConnector::insertWarning(pqxx::work& txn, int sensorId, const std::string& type) {
    try {
        std::string query = "INSERT INTO Warnings (sensor_id, type) VALUES (" +
                            txn.quote(sensorId) + ", " + txn.quote(type) + ");";
        txn.exec0(query);
    } catch (const std::exception& e) {
        std::cerr << "Failed to insert warning: " << e.what() << std::endl;
        throw; // Optionally rethrow to handle further up if necessary
    }
}



std::chrono::system_clock::time_point convertTimestamp(const std::string& timestamp) {
    std::tm tm = {};
    std::stringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Adjust format if necessary
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}


void DatabaseConnector::fetchAndAnalyzeData(pqxx::work& txn) {
    try {
        pqxx::result r = txn.exec("SELECT * FROM sensorsdata WHERE processed = false");

        // Pass this DatabaseConnector instance to the AnomalyDetector
        AnomalyDetector anomalyDetector(*this);  // Pass current instance to the constructor

        for (auto row : r) {
            int sensorID = row["sensor_id"].as<int>();

            // Check for null values in temperature or humidity before proceeding
            if (row["temperature"].is_null() || row["humidity"].is_null()) {
                std::string nullFields;
                if (row["temperature"].is_null()) {
                    nullFields += "temperature, ";
                }
                if (row["humidity"].is_null()) {
                    nullFields += "humidity, ";
                }
                nullFields = nullFields.substr(0, nullFields.size() - 2);  // Remove the last comma and space

                std::string warningMessage = "Null values detected for " + nullFields + " in sensor ID " + std::to_string(sensorID);
                std::cout << warningMessage << std::endl;
                insertWarning(txn, sensorID, warningMessage);
                continue;  // Skip further processing for this sensor data
            }

            // Construct a SensorData object from the row
            SensorData currentData;
            currentData.sensorID = sensorID;
            currentData.sensorVersion = row["sensor_version"].as<std::string>();
            currentData.plantID = row["plant_id"].as<int>();
            currentData.temperature = row["temperature"].as<float>();
            currentData.humidity = row["humidity"].as<float>();
            currentData.timeOfRecord = convertTimestamp(row["time"].as<std::string>());

            // Fetch the most recent previous record for the same sensor
            SensorData previousData = fetchMostRecentSensorData(currentData.sensorID, txn);

            // Use the AnomalyDetector to check if the rate of change is anomalous
            if (anomalyDetector.isRateOfChangeAnomalous(currentData, previousData, txn)) {
                std::cout << "Anomalous rate of change detected for sensor ID: " << currentData.sensorID << std::endl;
                // Optionally, take further actions like logging the anomaly or alerting
            }
            else if (anomalyDetector.isAnomalous(currentData, txn)) {
                std::cout << "Anomaly detected for sensor ID: " << currentData.sensorID << std::endl;
                // Optionally, take further actions like logging the anomaly or alerting
            }
            else {
                std::cout << "Entry ID# " << row["id"] << " intact" << std::endl;
            }

            // Update the processed flag for the current row
            txn.exec0("UPDATE sensorsdata SET processed = true WHERE id = " + txn.quote(row["id"].as<int>()));
        }

        txn.commit(); // Only commit if everything in the loop has executed without throwing an exception
    } catch (const std::exception& e) {
        std::cerr << "Error during fetch and analyze: " << e.what() << std::endl;
        txn.abort(); // Rollback the transaction in case of an error
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
