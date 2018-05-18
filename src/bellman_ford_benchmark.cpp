#include "../incl/bellman_ford_benchmark.h"

void benchmark(Graph g)
{
    // boost preproccesing
    unsigned long  N = num_vertices(g);
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);
    std::vector<int> distance(N, (std::numeric_limits < short >::max)());
    std::vector<int> mDistance(N, (std::numeric_limits < short >::max)());
    std::vector<std::size_t> parent(N);
    std::vector<std::size_t> mParent(N);
    for (unsigned long i = 0; i < N; ++i){
        mParent[i] = i;
        parent[i] = i;
    }

    int startNode = randomRange(0, (int)N);
    distance[4] = startNode;
    mDistance[4] = startNode;

    // LEDA preprocessing
    leda::GRAPH<unsigned, long>  G = convertToLeda(g);
    leda::edge_array<long> ledaWeight = G.edge_data();
    leda::node_array<leda::edge> ledaPred(G);  
    leda::node_array<long> ledaDist(G);
    leda::node ledaStartNode = G.first_node();

    clock_t begin = clock();
    bool boost_result = boost::bellman_ford_shortest_paths(
        g, int(N), weight_pmap, 
        &parent[0], &distance[0], 
        boost::closed_plus<int>(),
        std::less<int>(), 
        boost::default_bellman_visitor());
    clock_t end = clock();
    double elapsed_secs_boost = double(end- begin) / CLOCKS_PER_SEC;

    begin = clock();
    bool my_result = bf::bellman_ford_shortest_paths(
        g, int(N), weight_pmap, 
        &mParent[0], &mDistance[0], 
        boost::closed_plus<int>(),
        std::less<int>(), 
        boost::default_bellman_visitor());
    end = clock();
    double elapsed_secs_rafa = double(end- begin) / CLOCKS_PER_SEC;

    begin = clock();
    bool no_negative_cycle = leda::BELLMAN_FORD_B_T(
        G, ledaStartNode, ledaWeight, ledaDist, ledaPred);
    end = clock();
    double elapsed_secs_leda = double(end- begin) / CLOCKS_PER_SEC;

    std::cout << "      Time elapsed boost: " << elapsed_secs_boost << " seconds" << std::endl;
    std::cout << "      Time elapsed leda: " << elapsed_secs_leda << " seconds" << std::endl;
    std::cout << "      Time elapsed rafa: " << elapsed_secs_rafa << " seconds" << std::endl;
}