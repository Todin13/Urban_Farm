#include "RestServer.h"
#include "DatabaseConnector.h"
#include "MetricsCollector.h"
#include "AnomalyDetector.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  // For getenv()

std::string getEnvVar(const std::string& key, const std::string& defaultValue) {
    const char* val = getenv(key.c_str());
    return (val == nullptr) ? defaultValue : std::string(val);
}

int main() {
    std::cout.setf(std::ios::unitbuf); // turn off buffering for cout
    std::cout << "This is a test message.\n";

    // Initialize the database connector with values from environment or default ones
    std::string host = getEnvVar("DB_HOST", "localhost");
    std::string port = getEnvVar("DB_PORT", "5433");
    std::string dbname = getEnvVar("DB_NAME", "urbanfarm");
    std::string user = getEnvVar("DB_USER", "admin");
    std::string password = getEnvVar("DB_PASS", "urbanfarm123");

    std::string connectionString = "host=" + host +
                                   " port=" + port +
                                   " dbname=" + dbname +
                                   " user=" + user +
                                   " password=" + password;

    std::cout << "Connection String: " << connectionString << std::endl;
    DatabaseConnector dbConnector(connectionString);
    
    // Initialize the metrics collector
    MetricsCollector metricsCollector;

    // Initialize and start the REST server
    RestServer server(8080, dbConnector, metricsCollector);
    server.start();

    // Create an instance of AnomalyDetector
    AnomalyDetector anomalyDetector(dbConnector);


    // Set the polling frequency in seconds
    const int pollingFrequency = 5; // Example: Poll every 5 seconds

    std::cout << "Starting data analysis with polling frequency of " << pollingFrequency << " seconds." << std::endl;
    int i = 0;
    while (true) {
        std::cout << "POLL # " << i++ << std::endl;

        try {
            pqxx::work txn(dbConnector.getConnection()); // Start a new transaction
            anomalyDetector.analyzeData(txn); // Pass the transaction to analyzeData
            txn.commit(); // Commit the transaction
        } catch (const std::exception& e) {
            std::cerr << "Anomaly analysis failed: " << e.what() << std::endl;
            // Depending on the failure, you may choose to retry, back off, or abort the program
        }

        // Wait for the specified polling frequency before the next iteration
        std::this_thread::sleep_for(std::chrono::seconds(pollingFrequency));
    }

    // Since the while loop is infinite, you'd likely have signal handling elsewhere to terminate gracefully
    return 0;
}
