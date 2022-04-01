cmake_minimum_required(VERSION 3.10)

add_compile_options(-pthread)

project(Server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)

list(APPEND ServerSource main.cpp)

file(GLOB UtilInclude "${PROJECT_SOURCE_DIR}/Util/*.h" )
file(GLOB UtilSource "${PROJECT_SOURCE_DIR}/Util/*.cpp")

file(GLOB HTTPInclude "${PROJECT_SOURCE_DIR}/HTTP/*.h")
file(GLOB HTTPSource "${PROJECT_SOURCE_DIR}/HTTP/*.cpp")

list(APPEND ServerSource ${UtilInclude} ${UtilSource} ${HTTPInclude} ${HTTPSource})

add_executable(Server ${ServerSource})

#Threading packages
find_package (Threads REQUIRED)
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})

include_directories(Server PRIVATE "${PROJECT_SOURCE_DIR}/Util/" "${PROJECT_SOURCE_DIR}/HTTP/")

