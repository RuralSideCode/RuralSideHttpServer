cmake_minimum_required(VERSION 3.10)

project(Server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND ServerSource main.cpp)

file(GLOB UtilInclude "${PROJECT_SOURCE_DIR}/Util/*.h" )
file(GLOB UtilSource "${PROJECT_SOURCE_DIR}/Util/*.cpp")

file(GLOB HTTPInclude "${PROJECT_SOURCE_DIR}/HTTP/*.h")
file(GLOB HTTPSource "${PROJECT_SOURCE_DIR}/HTTP/*.cpp")

list(APPEND ServerSource ${UtilInclude} ${UtilSource} ${HTTPInclude} ${HTTPSource})

add_executable(Server ${ServerSource})

include_directories(Server PRIVATE "${PROJECT_SOURCE_DIR}/Util/" "${PROJECT_SOURCE_DIR}/HTTP/")

