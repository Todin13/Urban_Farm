#include "DatabaseConnector.h"
#include <iostream>

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
