cmake_minimum_required(VERSION 3.8)

project(bf)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_program_options")
endif()

add_executable(bf src/main.cpp incl/bellman_ford.h incl/bellman_ford_test.h incl/graph_creator.h incl/graph_printer.h src/graph_creator.cpp src/graph_printer.cpp src/bellman_ford.cpp src/bellman_ford_test.cpp)