#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

#include "../incl/graph_creator.h"
#include "../incl/bellman_ford.h"
#include "../incl/graph_printer.h"
#include "../incl/bellman_ford_test.h"


const int INF = (std::numeric_limits < int >::max)();

void usageMessage(std::string exe) 
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << exe << " run random <nodes>" << std::endl;
    std::cout << "  " << exe << " run grid <nodes>" << std::endl;
    std::cout << "  " << exe << " benchmark grid <nodes>" << std::endl;
    std::cout << "  " << exe << " benchmark random <nodes>" << std::endl;
    std::cout << "  " << exe << " test" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc==2 && (std::string(argv[1])=="test")) {
        std::cout << "[i] Running all tests" << std::endl;
        std::cout << "[i] Testing Graph with positive weights" << std::endl;
        testBF(createTestGraph());
        std::cout << "[i] Testing Graph with positive and negative weights" << std::endl;
        testBF(createPositiveTestGraph());
        std::cout << "[i] Testing Graph with negative cycle" << std::endl;
        testBF(createTestGraphWithNegativeCycle());
        std::cout << "[i] Random Graph" << std::endl;
        testBF(createRandomGraph(1000, 20*1000*log10(1000), -100, 10000));
        std::cout << "[i] Grid Graph" << std::endl;
        testBF(createGridGraph(100,-100, 10000));
    } else if (argc==4){
        int nodes = std::stoi(argv[3]);
        int edges = 20*nodes*log10(nodes);
        if (std::string(argv[1])=="benchmark" && std::string(argv[2])=="random") {
            std::cout << "[i] Benchmark random graph " << nodes << "x" << edges << std::endl;
            benchmark(createRandomGraph(nodes, edges, -100, 10000));
        } else if (std::string(argv[1])=="benchmark" && std::string(argv[2])=="grid") {
            std::cout << "[i] Benchmark grid graph " << nodes << "x" << nodes << std::endl;
            benchmark(createGridGraph(nodes, -100, 10000));
        } else if (std::string(argv[1])=="run" && std::string(argv[2])=="random") {
            std::cout << "[i] Random graph " << nodes << "x" << edges << std::endl;
            Graph G = createRandomGraph(nodes, edges, -100, 10000);
            unsigned long  n_vertices = num_vertices(G);
            std::vector<unsigned long> mPred(n_vertices);
            std::vector<long> mDistance(n_vertices, INF);
            WeightMap weight_pmap = get(&EdgeProperties::weight, G);
            for (std::size_t i = 0; i < n_vertices; ++i) mPred[i] = i;
            printGraphVizToFile(G, "graph.dot");
            bool res = bellmanFord(G, 0, weight_pmap, mPred, mDistance);
             if (!res) {
                std::cout << "negative cycle detected" << std::endl;
            }
            printGraphShortestPath(G, mDistance, mPred);
            printGraphShortestPathVizToFile(G, mPred, "graph.dot");
        } else if (std::string(argv[1])=="run" && std::string(argv[2])=="grid") {
            std::cout << "[i] Grid graph " << nodes << "x" << nodes << std::endl;
            Graph G = createGridGraph(nodes,-100, 10000);
            unsigned long  n_vertices = num_vertices(G);
            std::vector<unsigned long> mPred(n_vertices);
            std::vector<long> mDistance(n_vertices, INF);
            WeightMap weight_pmap = get(&EdgeProperties::weight, G);
            for (std::size_t i = 0; i < n_vertices; ++i) mPred[i] = i;
            printGraphVizToFile(G, "graph.dot");
            bool res = bellmanFord(G, 0, weight_pmap, mPred, mDistance);
            if (!res) {
                std::cout << "negative cycle detected" << std::endl;
            }
            printGraphShortestPath(G, mDistance, mPred);
            printGraphShortestPathVizToFile(G, mPred, "shortest-path.dot");
        }
    } else {
        usageMessage(argv[0]);
    }
    // int nodes = 5;
    // std::cout << "[i] Grid graph " << nodes << "x" << nodes << std::endl;
    // Graph G = createGridGraph(nodes,-100, 10000);
    // unsigned long  n_vertices = num_vertices(G);
    // std::vector<unsigned long> mPred(n_vertices);
    // std::vector<long> mDistance(n_vertices, INF);
    // WeightMap weight_pmap = get(&EdgeProperties::weight, G);
    // for (std::size_t i = 0; i < n_vertices; ++i) mPred[i] = i;
    // printGraphVizToFile(G, "graph.dot");
    // bool res = bellmanFord(G, 0, weight_pmap, mPred, mDistance);
    // if (!res) {
    //     std::cout << "negative cycle detected" << std::endl;
    // }
    // printGraphShortestPath(G, mDistance, mPred);
    // printGraphShortestPathVizToFile(G, mPred, "shortest-path.dot");


    // leda::GRAPH<unsigned, long>  g = convertToLeda(G);
    // leda::edge_array<long> ledaWeight = g.edge_data();
    // leda::node_array<leda::edge> ledaPred(g);  
    // leda::node_array<long> ledaDist(g);
    // leda::node ledaStartNode = g.first_node();
    // // Running
    // bool r_leda = leda::BELLMAN_FORD_B_T(g, ledaStartNode, ledaWeight, ledaDist, ledaPred);
    // printLedaGraphShortestPath(g, ledaDist, ledaPred);

    // return 0;
}