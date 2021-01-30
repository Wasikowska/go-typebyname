cmake_minimum_required(VERSION 3.18)

project(alg)
set(CMAKE_CXX_STANDARD 17)

file(GLOB src *.cpp)
add_subdirectory(googletest)

add_executable(alg ${src})
target_link_libraries(alg gtest_main)
