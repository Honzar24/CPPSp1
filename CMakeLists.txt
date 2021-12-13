cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set	(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")

project(DiskretniSimRadl VERSION 0.1.0)

set(SOURCES_LIST main.cpp src/Object.cpp src/Rect.cpp src/Circle.cpp)

add_executable(kiv-cpp-sp-01 ${SOURCES_LIST})

include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)
