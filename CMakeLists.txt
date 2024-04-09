cmake_minimum_required(VERSION 3.10) # Adjust the version as needed
project(UrbanFarm) # Replace with your actual project name

# Set the C++ standard to be used
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories if there are specific subdirectories for headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find the packages installed via vcpkg
find_package(cpprestsdk CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

# Add all the source files required for your executable
add_executable(${PROJECT_NAME}
    main.cpp
    AnomalyDetector.cpp
    DatabaseConnector.cpp
    JsonSerializer.cpp
    MetricsCollector.cpp
    RestServer.cpp
    WarningReporter.cpp
    # Add any other cpp files here
)

# Link against the libraries from vcpkg
target_link_libraries(${PROJECT_NAME}
    PRIVATE cpprestsdk::cpprest
    PRIVATE libpqxx::pqxx
    # Add any other libraries here
)
