cmake_minimum_required(VERSION 3.10) # Adjust the version as needed
project(UrbanFarm) # Replace with your actual project name

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories if there are specific subdirectories for headers
include_directories(${CMAKE_SOURCE_DIR})
include("C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake")
include_directories("C:/Program Files/vcpkg/installed/x64-windows/share")

# Find the packages installed via vcpkg
set(cpprestsdk_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/cpprestsdk")
set(ZLIB_LIBRARY "C:/Program Files/vcpkg/installed/x64-windows/lib/zlib.lib")
set(ZLIB_INCLUDE_DIR "C:/Program Files/vcpkg/installed/x64-windows/include/zlib.h")
set(unofficial-brotli_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/unofficial-brotli")
set(libpqxx_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/libpqxx")
set(ZLIB_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/zlib")


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