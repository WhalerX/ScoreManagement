cmake_minimum_required(VERSION 4.1)

set(CMAKE_C_COMPILER "D:/MinGW/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/MinGW/bin/g++.exe")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

PROJECT(ScoreManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# ===================== OpenXlSX =====================
set(OpenXLSX_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenXLSX)
set(OpenXLSX_DIR ${OpenXLSX_ROOT}/lib/cmake/OpenXLSX)
find_package(OpenXLSX REQUIRED)

add_executable(app ${SRC})

target_link_libraries(app
    PRIVATE
    OpenXLSX::OpenXLSX
)
