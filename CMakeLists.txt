cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(DiskretniSimRadl VERSION 0.1.0)

set(SOURCES_LIST main.cpp)

add_executable(kiv-cpp-sp-01 ${SOURCES_LIST})

include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)



if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif(CMAKE_BUILD_TYPE EQUAL "DEBUG")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
