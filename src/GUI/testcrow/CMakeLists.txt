cmake_minimum_required(VERSION 3.29)
project(UrbanFarmGUIWeb)
set(CMAKE_CXX_STANDARD 20)
add_executable(UrbanFarmGUIWeb main.cpp)

add_subdirectory(../../../libs/libpqxx-7.9.0 build-pqxx)
target_link_libraries(UrbanFarmGUIWeb PRIVATE pqxx)
target_include_directories(my_project PRIVATE ../../../include)