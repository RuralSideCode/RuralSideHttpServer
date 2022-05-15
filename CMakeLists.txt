cmake_minimum_required(VERSION 3.10)

add_compile_options(-pthread)

project(Server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB UtilInclude "${PROJECT_SOURCE_DIR}/Util/*.h" )
file(GLOB UtilSource "${PROJECT_SOURCE_DIR}/Util/*.cpp")

file(GLOB HTTPInclude "${PROJECT_SOURCE_DIR}/HTTP/*.h")
file(GLOB HTTPSource "${PROJECT_SOURCE_DIR}/HTTP/*.cpp")

file(GLOB CLIInclude "${PROJECT_SOURCE_DIR}/CLI/*.h")
file(GLOB CLISource "${PROJECT_SOURCE_DIR}/CLI/*.cpp")

file(GLOB ServiceMonitorInclude "${PROJECT_SOURCE_DIR}/ServiceMonitoring/*.h")
file(GLOB ServiceMonitorSource "${PROJECT_SOURCE_DIR}/ServiceMonitoring/*.cpp")

list(APPEND ServerSource ${UtilInclude} ${UtilSource} ${HTTPInclude} ${HTTPSource} ${CLIInclude} ${CLISource} ${ServiceMonitorInclude} ${ServiceMonitorSource})

add_executable(Server main.cpp ${ServerSource})
add_executable(Tests tests.cpp ${ServerSource})

#Threading packages
find_package (Threads REQUIRED)
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Tests ${CMAKE_THREAD_LIBS_INIT})

#JSON parser
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(Server nlohmann_json::nlohmann_json)
target_link_libraries(Tests nlohmann_json::nlohmann_json)

#UnitTest++
find_package(UnitTest++ REQUIRED)
target_link_libraries(Tests UnitTest++)

list(APPEND INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/Util/" "${PROJECT_SOURCE_DIR}/HTTP/" "${PROJECT_SOURCE_DIR}/CLI/" "${PROJECT_SOURCE_DIR}/Configuration/" "${PROJECT_SOURCE_DIR}/ServiceMonitoring/")

include_directories(Server PRIVATE  ${INCLUDE_DIRECTORIES})
include_directories(Tests PRIVATE ${INCLUDE_DIRECTORIES})
