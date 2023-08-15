cmake_minimum_required(VERSION 3.2)
project(malloc)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)
set(name app)
add_executable(app ${SOURCES} src/main.cpp)

