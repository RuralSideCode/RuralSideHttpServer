cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND ServerSource main.cpp)
file(GLOB UtilInclude "${PROJECT_SOURCE_DIR}/Util/*.h" )
file(GLOB UtilSource "${PROJECT_SOURCE_DIR}/Util/*.cpp")
list(APPEND ServerSource ${UtilInclude} ${UtilSource})

add_executable(Server ${ServerSource})

include_directories(Server PRIVATE "${PROJECT_SOURCE_DIR}/Util/")

