#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

#include "../incl/graph_creator.h"
#include "../incl/graph_printer.h"
#include "../incl/bellman_ford_test.h"
#include "../incl/bellman_ford_benchmark.h"

using namespace boost;

const int INF = (std::numeric_limits < int >::max)();

int main(int argc, char **argv)
{
    if (argc==2 && (std::string(argv[1])=="test")){
        std::cout << "[i] Running all tests" << std::endl;
        std::cout << "[i] Testing Graph with positive weights" << std::endl;
        testBF(createTestGraph());
        std::cout << "[i] Testing Graph with positive and negative weights" << std::endl;
        testBF(createPositiveTestGraph());
        std::cout << "[i] Testing Graph with negative cycle" << std::endl;
        testBF(createTestGraphWithNegativeCycle());
    
        return 0;

    } else if (argc==2 && std::string(argv[1])=="benchmark"){
        std::cout << "[i] Running all benchmarks" << std::endl;
        std::cout << "[i] Random Graphs" << std::endl;
        std::cout << "[i] Graph 1000x20*1000*log(1000)" << std::endl;
        benchmark(createRandomGraph(1000, 20*1000*log(1000), -100, 10000));
        std::cout << "[i] Graph 4000x20*4000*log(4000)" << std::endl;
        benchmark(createRandomGraph(4000, 20*4000*log(4000), -100, 10000));
        std::cout << "[i] Graph 9000x20*9000*log(9000)" << std::endl;
        benchmark(createRandomGraph(9000, 20*9000*log(9000), -100, 10000));

        std::cout << "[i] Grid Graphs" << std::endl;
        std::cout << "[i] Graph 100*100" << std::endl;
        benchmark(createGridGraph(100, -100, 10000));
        std::cout << "[i] Graph 200x200" << std::endl;
        benchmark(createGridGraph(200, -100, 10000));
        std::cout << "[i] Graph 300x300" << std::endl;
        benchmark(createGridGraph(200, -100, 10000));

        return 0;
    }

    Graph g = createTestGraph();
    
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);
    unsigned long  n_vertices = num_vertices(g);

    std::vector<int> distance(n_vertices, INF);
    std::vector<std::size_t> b_pred(n_vertices);
    for (std::size_t i = 0; i < n_vertices; ++i) b_pred[i] = i;
    distance[0] = 0;

    bool r = bellman_ford_shortest_paths(g, n_vertices, weight_pmap,
                                         &b_pred[0], &distance[0],
                                         closed_plus<int>(),
                                         std::less<int>(),
                                         default_bellman_visitor());


    if (r) {
        printGraphShortestPath(g, distance, b_pred);
    } else {
        std::cout << "negative cycle detected" << std::endl;
    }

    leda::GRAPH<unsigned, long>  G = convertToLeda(g);
    leda::edge_array<long> cost = G.edge_data();
    leda::node_array<leda::edge> pred(G);  
    leda::node_array<long> dist(G);

    leda::node startNode = G.first_node();

    bool no_negative_cycle = leda::BELLMAN_FORD_B_T(G, startNode, cost, dist, pred);

    leda::node v;
    forall_nodes(v,G) {
        G.print_node(v);
        if (pred[v]==nil) {
            std::cout << " is unreachable." << std::endl;
        }
        else {
            std::cout << " " << dist[v] << " "; 
            G.print_edge(pred[v]);
            std::cout << std::endl;
        }
    }

    return 0;
}