# Compile the codebase
g++ -std=c++11 -o plant_monitoring_service main.cpp RestServer.cpp DatabaseConnector.cpp AnomalyDetector.cpp JsonSerializer.cpp WarningReporter.cpp MetricsCollector.cpp -lpqxx -lcpprest

# Run the compiled binary
./plant_monitoring_service &
