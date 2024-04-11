#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <pqxx/pqxx>
#include <string>
#include "SensorData.h"

class DatabaseConnector {
public:
    explicit DatabaseConnector(const std::string& connectionString);
    void testQuery(); // Declare a method for testing a simple query
    void fetchSensorData();
    void fetchAndAnalyzeData();
    SensorData fetchMostRecentSensorData(int sensorID, pqxx::work& txn);

private:
    pqxx::connection dbConnection;
};

#endif // DATABASECONNECTOR_H
