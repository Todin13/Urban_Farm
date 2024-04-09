#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <pqxx/pqxx>
#include <string>

class DatabaseConnector {
public:
    explicit DatabaseConnector(const std::string& connectionString);
    // Methods to interact with the database

private:
    pqxx::connection dbConnection;
};

#endif // DATABASECONNECTOR_H