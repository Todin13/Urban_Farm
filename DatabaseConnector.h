#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <pqxx/pqxx>
#include <string>
#include "SensorData.h"

class DatabaseConnector {
public:
    explicit DatabaseConnector(const std::string& connectionString);
    void fetchAndAnalyzeData(pqxx::work& txn); // Make sure this accepts a transaction
    pqxx::connection& getConnection(); // Add this line for declaration
    static void insertWarning(pqxx::work& txn, int sensorId, const std::string& type); // Made static, as it doesn't depend on instance state
    void testQuery(); // Declare a method for testing a simple query
    void fetchSensorData();
    SensorData fetchMostRecentSensorData(int sensorID, pqxx::work& txn);
    
    

private:
    pqxx::connection dbConnection;
};

#endif // DATABASECONNECTOR_H
