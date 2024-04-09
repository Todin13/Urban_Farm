# Install dependencies using package.json or similar file if available
# Since no package manager file is provided, we'll assume dependencies are installed via system package manager
# However, as per the instructions, we should not use sudo or install globally, so we will skip this step.

# Compile the codebase
g++ -std=c++11 -o plant_monitoring_service main.cpp RestServer.cpp DatabaseConnector.cpp AnomalyDetector.cpp JsonSerializer.cpp WarningReporter.cpp MetricsCollector.cpp -lpqxx -lcpprest

# Run the compiled binary
./plant_monitoring_service &
