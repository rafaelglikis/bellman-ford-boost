#include <vector>
#include <iostream>

#include "../incl/graph_creator.h"
#include "../incl/bellman_ford.h"
#include "../incl/graph_printer.h"
#include "../incl/bellman_ford_test.h"

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
    const int INF = (std::numeric_limits < int >::max)();
    if (argc==2 && (std::string(argv[1])=="test")) {
        testAll();
    } else if (argc==4){
        int nodes = std::stoi(argv[3]);
        int edges = 20*nodes*log10(nodes);
        // **************************************************** BENCHMARK
        if (std::string(argv[1])=="benchmark" && std::string(argv[2])=="random") 
        {
            std::cout << "[i] Benchmark random graph " << nodes << "x" << edges << std::endl;
            benchmark(createRandomGraph(nodes, edges, -100, 10000), 2);
        } 
        else if (std::string(argv[1])=="benchmark" && std::string(argv[2])=="grid") 
        {
            std::cout << "[i] Benchmark grid graph " << nodes << "x" << nodes << std::endl;
            benchmark(createGridGraph(nodes, -100, 10000), 2);
        }
        // **************************************************** RUN 
        else if (std::string(argv[1])=="run" && std::string(argv[2])=="random") {
            std::cout << "[i] Random graph " << nodes << "x" << edges << std::endl;
            Graph G = createRandomGraph(nodes, edges, -100, 10000);
            unsigned long  n_vertices = num_vertices(G);
            std::vector<unsigned long> pred(n_vertices);
            std::vector<long> dist(n_vertices, INF);
            WeightMap weight_pmap = get(&EdgeProperties::weight, G);
            for (std::size_t i = 0; i < n_vertices; ++i) pred[i] = i;
            printGraphVizToFile(G, "graph.dot");
            bool res = bellmanFord(G, 0, weight_pmap, pred, dist);
             if (!res) {
                std::cout << "negative cycle detected" << std::endl;
            }
            std::vector<int> label = CHECK_BELLMAN_FORD(G, 0, weight_pmap, pred, dist);
            printGraphShortestPath(G, dist, pred, label);
            printGraphShortestPathVizToFile(G, pred, "graph.dot");
        } else if (std::string(argv[1])=="run" && std::string(argv[2])=="grid") {
            std::cout << "[i] Grid graph " << nodes << "x" << nodes << std::endl;
            Graph G = createGridGraph(nodes,-100, 10000);
            unsigned long  n_vertices = num_vertices(G);
            std::vector<unsigned long> pred(n_vertices);
            std::vector<long> dist(n_vertices, INF);
            WeightMap weight_pmap = get(&EdgeProperties::weight, G);
            for (std::size_t i = 0; i < n_vertices; ++i) pred[i] = i;
            printGraphVizToFile(G, "graph.dot");
            bool res = bellmanFord(G, 0, weight_pmap, pred, dist);
            if (!res) {
                std::cout << "negative cycle detected" << std::endl;
            }
            std::vector<int> label = CHECK_BELLMAN_FORD(G, 0, weight_pmap, pred, dist);
            printGraphShortestPath(G, dist, pred, label);
            printGraphShortestPathVizToFile(G, pred, "shortest-path.dot");
        }
    } else {
        usageMessage(argv[0]);
    }

    return 0;
}