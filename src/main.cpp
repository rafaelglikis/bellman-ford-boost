#include <vector>
#include <iostream>
#include "../incl/graph_creator.h"
#include "../incl/bellman_ford.h"
#include "../incl/graph_printer.h"
#include "../incl/bellman_ford_test.h"
#include <boost/program_options.hpp>

unsigned long nodes;
unsigned long edges = 0;
int times;
int minWeight;
int maxWeight;

/**
 * Prints usage message.
*/
boost::program_options::variables_map parseArguments(int argc, char *const *argv)
{
    boost::program_options::options_description options("Usage");
    options.add_options()
            ("test", "run all tests")
            ("run", "runs with specified graph")
            ("benchmark", "runs benchmark with specified graph")
            ("help", "produce help message")
            ("random", "random connected graph")
            ("grid", "modified grid graph with negative cycle (worst case for bellman ford)")
            ("nodes,n", boost::program_options::value<unsigned long>(&nodes), "number of nodes (default=100)")
            ("edges,e", boost::program_options::value<unsigned long>(&edges), "number of edges for random graphs (default 20*nodes*log10(nodes))")
            ("times,t", boost::program_options::value<int>(&times), "number of times to run the benchmark")
            ("min-weight", boost::program_options::value<int>(&minWeight), "minimum edge weight (default=-100)")
            ("max-weight", boost::program_options::value<int>(&maxWeight), "maximum edge weight (default=10000)")
            ;

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, options), vm);
    notify(vm);

    if (vm.count("help") || argc < 2) {
        std::cout << options << "\n";
        exit(1);
    }

    return vm;
}


int main(int argc, char **argv)
{
    const int INF = (std::numeric_limits < int >::max)();
    nodes = 100;
    times = 1;
    minWeight = -100;
    maxWeight = 10000;

    boost::program_options::variables_map args = parseArguments(argc, argv);
    if (edges==0) {
        edges = static_cast<unsigned long>(20 * nodes * log10(nodes));
    }

    if (args.count("test")) {
        testAll();
    }
    else if (args.count("benchmark") && args.count("random")) {
        Graph G = createRandomGraph(nodes, edges, minWeight, maxWeight);
        std::cout << "[i] Benchmark random graph " << nodes << "x" << edges << std::endl;
        benchmark(G, times);
    }
    else if (args.count("benchmark") && args.count("grid")) {
        Graph G = createGridGraph(static_cast<int>(nodes), minWeight, maxWeight);
        std::cout << "[i] Benchmark grid graph " << nodes << "x" << nodes << std::endl;
        benchmark(G, times);
    }
    else if (args.count("run")) {
        Graph G;
        if (args.count("random")) {
            std::cout << "[i] Random graph " << nodes << "x" << edges << std::endl;
            G = createRandomGraph(nodes, edges, minWeight, maxWeight);
        }
        else if (args.count("grid")) {
            std::cout << "[i] Grid graph " << nodes << "x" << nodes << std::endl;
            G = createGridGraph(static_cast<int>(nodes), minWeight, maxWeight);
        }
        else {
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

    return 0;
}
