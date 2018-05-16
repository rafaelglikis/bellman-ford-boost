cmake_minimum_required(VERSION 3.8)
project(hw2)

set(CMAKE_CXX_STANDARD 11)

include_directories(incl)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

add_executable(hw2
        src/main.cpp
        src/graph_creator.cpp incl/graph_creator.h
        src/graph_printer.cpp incl/graph_printer.h
        src/bellman_ford.cpp incl/bellman_ford.h
        src/bellman_ford_test.cpp incl/bellman_ford_test.h)
