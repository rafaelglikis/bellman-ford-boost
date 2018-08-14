# Bellman Ford for boost graph library
The Bellman–Ford algorithm is an algorithm that computes shortest paths from a single source vertex to all of the other vertices in a weighted directed graph. It works with both positive and negative edge weights.

Boost graph libary has an implementation but doesn't work if the graph has negative cycle in it, so I implemented: 
* bool bellmanFord(const Graph&, const int&, const WeightMap&, std::vector<unsigned long>&, std::vector<long>&);
* std::vector<int> CHECK_BELLMAN_FORD(const Graph&, const int&, const WeightMap&, std::vector<unsigned long>&, std::vector<long>&);

The first is an implementation of the belman ford algorithm based on LEDA functionalities. The second is a function that checks the result of the first function for mistakes and proofs mathematically that the result is right. If the result is right returns an array of labels mapped to each vertex. The labels are the following:
* V- = { distance = -infinity } (accessed by negative cycle) 
* Vf = { -infinity < distance < +infinity } (found but not accessed by negative cycle)
* V+ = { distance = +infinity } (not found)

## Requirements
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