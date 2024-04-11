cmake_minimum_required(VERSION 3.10) # Adjust the version as needed
project(UrbanFarm) # Replace with your actual project name

# Set the C++ standard to be used
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR})

# Adjust paths based on the operating system
if(WIN32)
    include("C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake")
    include_directories("C:/Program Files/vcpkg/installed/x64-windows/share")
    set(cpprestsdk_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/cpprestsdk")
    set(ZLIB_LIBRARY "C:/Program Files/vcpkg/installed/x64-windows/lib/zlib.lib")
    set(ZLIB_INCLUDE_DIR "C:/Program Files/vcpkg/installed/x64-windows/include/zlib.h")
    set(unofficial-brotli_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/unofficial-brotli")
    set(libpqxx_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/libpqxx")
    set(ZLIB_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/zlib")
elseif(UNIX)
    # Set the path to vcpkg CMake scripts
    set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")
    include(${CMAKE_TOOLCHAIN_FILE})

    # Include directories for installed libraries via vcpkg on Linux
    include_directories("/opt/vcpkg/installed/x64-linux/include")
    find_package(cpprestsdk CONFIG REQUIRED)
    find_package(libpqxx CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

# Find the packages installed via vcpkg or system libraries
# These are now inside the condition to ensure they are available and set properly
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

# Link against the libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE cpprestsdk::cpprest
    PRIVATE libpqxx::pqxx
    # Add any other libraries here
)
