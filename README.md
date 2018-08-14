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

## How to use it
* Create graph
* Ask for shortest paths
* Check the results
* Ask for labels

### Example
    
    #include <vector>
    #include <iostream>
    #include "../incl/graph_creator.h"
    #include "../incl/bellman_ford.h"
    #include "../incl/graph_printer.h"
    
    int main(int argc, char **argv)
    {
        const int INF = (std::numeric_limits < int >::max)();
        
        // Create an edge list graph 
        typedef std::pair <int, int>E;
        const int n_edges = 10;
        E edge_array[] = { E(0, 3), E(0, 2), E(0, 1), E(1, 0), E(2, 3), E(2, 1), E(3, 1), E(3, 4), E(4, 0), E(4,2) };
        Graph G(edge_array, edge_array + n_edges, 5);
        
        // Add positive and negative weights
        int weight[n_edges] = { -4, 8, 5, -2, 9, -3, 7, 2, 6, 7 }; 
        EdgeIterator ei, ei_end;
        int i = 0;
        for (tie(ei, ei_end) = edges(G); ei != ei_end; ++ei, ++i)
            G[*ei].weight = weight[i];
            
        // Print graph in graphViz format
        printGraphViz(G);
    
        // Initialize algorithm structures
        unsigned long  N = num_vertices(G);
        std::vector<unsigned long> pred(N);
        std::vector<long> dist(N, INF); // Always with INF
        WeightMap weightMap = get(&EdgeProperties::weight, G);
        
        // Run bellman ford
        bool res = bellmanFord(G, 0, weightMap, pred, dist);
        
        // Run checker
        std::vector<int> label = CHECK_BELLMAN_FORD(G, 0, weightMap, pred, dist);
        std::cout << "[+] Test OK!" << std::endl;
        
        // Print shorthest path in graphviz format
        printGraphShortestPathViz(G, pred);
        // Print bellman ford results
        printGraphShortestPath(G, dist, pred, label);
        
        return 0;
     }

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