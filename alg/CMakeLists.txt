cmake_minimum_required(VERSION 3.18)

project(alg)
set(CMAKE_CXX_STANDARD 17)

file(GLOB src *.cpp)
add_subdirectory(googletest)
add_subdirectory(benchmark)

add_executable(alg ${src})
target_link_libraries(alg gtest_main benchmark::benchmark)

# add_executable(alg_bench ${src} bench.cxx)
# target_link_libraries(alg_bench gtest benchmark::benchmark)
