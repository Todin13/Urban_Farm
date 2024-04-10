#include "RestServer.h"
#include "DatabaseConnector.h"
#include "MetricsCollector.h"

int main() {
    // Initialize the database connector
    const std::string connectionString = "host=localhost port=5433 dbname=urbanfarm user=admin password=urbanfarm123";
    DatabaseConnector dbConnector(connectionString);
    dbConnector.testQuery();
    
    // Initialize the metrics collector
    MetricsCollector metricsCollector;

    // Initialize and start the REST server
    RestServer server(8080, dbConnector, metricsCollector);
    server.start();

    return 0;
}