cmake_minimum_required(VERSION 3.8)

# Set this project's name.
project("acm-examples")

# Set the C++ standard to use to build this target.
set(CMAKE_CXX_STANDARD 11)

add_executable(ipc main.cpp master.cpp slave.cpp worker.cpp msg.cpp)
target_link_libraries(ipc rt)
