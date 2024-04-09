#include "RestServer.h"
#include "DatabaseConnector.h"
#include "MetricsCollector.h"

int main() {
    // Initialize the database connector
    DatabaseConnector dbConnector("host=localhost dbname=plant_monitoring user=postgres password=postgres");

    // Initialize the metrics collector
    MetricsCollector metricsCollector;

    // Initialize and start the REST server
    RestServer server(8080, dbConnector, metricsCollector);
    server.start();

    return 0;
}