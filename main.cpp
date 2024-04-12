#include "RestServer.h"
#include "DatabaseConnector.h"
#include "MetricsCollector.h"
#include "AnomalyDetector.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  // For getenv()


int main() {
    std::cout.setf(std::ios::unitbuf); // turn off buffering for cout
    std::cout << "This is a test message.";
    // Create an instance of AnomalyDetector
    AnomalyDetector anomalyDetector;

    // Initialize the database connector
    std::string host = anomalyDetector.getEnvVar("DB_HOST", "localhost");
    std::string port = anomalyDetector.getEnvVar("DB_PORT", "5433");
    std::string dbname = anomalyDetector.getEnvVar("DB_NAME", "urbanfarm");
    std::string user = anomalyDetector.getEnvVar("DB_USER", "admin");
    std::string password = anomalyDetector.getEnvVar("DB_PASSWORD", "urbanfarm123");

    std::string connectionString = "host=" + host +
                                   " port=" + port +
                                   " dbname=" + dbname +
                                   " user=" + user +
                                   " password=" + password;

    std::cout << "Connection String: " << connectionString << std::endl;
    DatabaseConnector dbConnector(connectionString);
    dbConnector.testQuery();
    dbConnector.fetchSensorData();
    
    // Initialize the metrics collector
    MetricsCollector metricsCollector;

    // Initialize and start the REST server
    RestServer server(8080, dbConnector, metricsCollector);
    server.start();

    // Set the polling frequency in seconds
    const int pollingFrequency = 5; // Example: Poll every 5 seconds


    std::cout << "Starting data analysis with polling frequency of " << pollingFrequency << " seconds." << std::endl;
    int i = 0;
    while (true) {
        std::cout << "POLL # " << i++ << std::endl;

        // Call the analyzeData method on the instance of AnomalyDetector
        anomalyDetector.analyzeData();
        
        // Wait for the specified polling frequency before the next iteration
        std::this_thread::sleep_for(std::chrono::seconds(pollingFrequency));
    }


    return 0; // This line will never be reached technically
}