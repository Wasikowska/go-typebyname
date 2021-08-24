cmake_minimum_required(VERSION 3.18)

project(leet)
set(CMAKE_CXX_STANDARD 17)

file(GLOB src src/*.cpp)

add_executable(leet ${src})
