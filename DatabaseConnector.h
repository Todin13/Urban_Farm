#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <pqxx/pqxx>
#include <string>

class DatabaseConnector {
public:
    explicit DatabaseConnector(const std::string& connectionString);
    void testQuery(); // Declare a method for testing a simple query

private:
    pqxx::connection dbConnection;
};

#endif // DATABASECONNECTOR_H
