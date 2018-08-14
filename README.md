# Bellman Ford for boost graph library

# Requirements
* boost graph library
* cmake

## Compile

    mkdir cmake-build
    cd cmake-build
    cmake ..
    cd ..
    cmake --build cmake-build

## Usage

    bf ACTION(S)* GRAPH(S)* [OPTIONS]*

Test:

      --test                run all tests

Actions:

      --run                 runs with specified graph
      --benchmark           runs benchmark with specified graph

Graphs:

      --random              random connected graph
      --grid                modified grid graph with negative cycle (worst case for bellman ford)

Options:

      -n [ --nodes ] arg    number of nodes
      -e [ --edges ] arg    number of edges
      -t [ --times ] arg    number of times to run the benchmark
      --min-weight arg      number of times to run the benchmark
      --max-weight arg      number of times to run the benchmark

Help:

      --help                produce help message