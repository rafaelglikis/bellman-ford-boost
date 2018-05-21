#include <vector>
#include <iostream>

#include "../incl/graph_creator.h"
#include "../incl/bellman_ford.h"
#include "../incl/graph_printer.h"
#include "../incl/bellman_ford_test.h"


/**
 * Prints usage message.
*/
void usageMessage(std::string exe) 
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << exe << " run random <n>" << std::endl;
    std::cout << "  " << exe << " run grid <n>" << std::endl;
    std::cout << "  " << exe << " benchmark grid <n>" << std::endl;
    std::cout << "  " << exe << " benchmark <times> random <n>" << std::endl;
    std::cout << "  " << exe << " test" << std::endl;
}

int main(int argc, char **argv)
{
    const int INF = (std::numeric_limits < int >::max)();
    if (argc==2 && (std::string(argv[1])=="test")) {
        testAll();
    } 
    // **************************************************** BENCHMARK random
    else if (argc==5 && std::string(argv[1])=="benchmark" && std::string(argv[3])=="random") {
        int nodes = std::stoi(argv[4]);
        int edges = 20*nodes*log10(nodes);
        int times = std::stoi(argv[2]);
        Graph G = createRandomGraph(nodes, edges, -100, 10000);
        std::cout << "[i] Benchmark random graph " << nodes << "x" << edges << std::endl;
        benchmark(G, times);
    }
    // **************************************************** BENCHMARK grid
    else if (argc==4 && std::string(argv[1])=="benchmark" && std::string(argv[2])=="grid") {
        int nodes = std::stoi(argv[3]);
        Graph G = createGridGraph(nodes, -100, 10000);
        std::cout << "[i] Benchmark grid graph " << nodes << "x" << nodes << std::endl;
        benchmark(G, 1);
    }
    // **************************************************** RUN
    else if (argc==4 && std::string(argv[1])=="run" && (std::string(argv[2])=="random" || std::string(argv[2])=="grid")) {
        int nodes = std::stoi(argv[3]);
        Graph G;
        if(std::string(argv[2])=="random"){
            int edges = 20*nodes*log10(nodes);
            std::cout << "[i] Random graph " << nodes << "x" << edges << std::endl;
            G = createRandomGraph(nodes, edges, -100, 10000);
        } else if(std::string(argv[2])=="grid") {
            std::cout << "[i] Grid graph " << nodes << "x" << nodes << std::endl;
            G = createGridGraph(nodes,-100, 10000);
        } else {
            usageMessage(argv[0]);
            return 0;
        }
        unsigned long  N = num_vertices(G);
        std::vector<unsigned long> pred(N);
        std::vector<long> dist(N, INF);
        WeightMap weight = get(&EdgeProperties::weight, G);
        printGraphVizToFile(G, "graph.dot");
        bool res = bellmanFord(G, 0, weight, pred, dist);
        if (!res) {
            std::cout << "negative cycle detected" << std::endl;
        }
        std::vector<int> label = CHECK_BELLMAN_FORD(G, 0, weight, pred, dist);
        std::cout << "[+] Test OK!" << std::endl;
        printGraphShortestPath(G, dist, pred, label);
        printGraphShortestPathVizToFile(G, pred, "shortest-path.dot");
        std::cout << "Run the following (if you have graphviz installed) to see the graph images:" << std::endl;
        std::cout << "  neato -Tpng graph.dot -o graph.png && xdg-open  graph.png" << std::endl;
        std::cout << "  neato -Tpng shortest-path.dot -o shortest-path.png && xdg-open shortest-path.png" << std::endl;
    }
    else {
        usageMessage(argv[0]);
    }

    return 0;
}
