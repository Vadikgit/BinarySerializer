cmake_minimum_required (VERSION 3.27)
project(BinarySerializer CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

add_executable(codegen BinarySerializer.cpp)

file(COPY ${CMAKE_SOURCE_DIR}/types.h DESTINATION ${CMAKE_SOURCE_DIR}/test)
file(COPY ${CMAKE_SOURCE_DIR}/types.h DESTINATION ${PROJECT_BINARY_DIR}/include)