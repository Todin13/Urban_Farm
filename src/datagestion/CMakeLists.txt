cmake_minimum_required(VERSION 3.29)
project(GUI)

set(INCLUDE_PATHS ../../include)

add_executable(GUI main.cpp)

target_include_directories(GUI PUBLIC ${INCLUDE_PATHS})
