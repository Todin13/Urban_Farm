#include "RestServer.h"
#include "DatabaseConnector.h"
#include "MetricsCollector.h"
#include "AnomalyDetector.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    // Initialize the database connector
    const std::string connectionString = "host=localhost port=5433 dbname=urbanfarm user=admin password=urbanfarm123";
    DatabaseConnector dbConnector(connectionString);
    dbConnector.testQuery();
    dbConnector.fetchSensorData();
    
    // Initialize the metrics collector
    MetricsCollector metricsCollector;

    // Create an instance of AnomalyDetector
    AnomalyDetector anomalyDetector;

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