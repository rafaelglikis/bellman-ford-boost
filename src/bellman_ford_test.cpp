#include "../incl/bellman_ford_test.h"
#include "../incl/bellman_ford.h"

void testAll()
{
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
}

void testBF(Graph G)
{
    const int INF = (std::numeric_limits < int >::max)();
    int i;
    // init
    unsigned long  n_vertices = num_vertices(G);
    std::vector<unsigned long> pred(n_vertices);
    std::vector<long> dist(n_vertices, INF);
    WeightMap weight = get(&EdgeProperties::weight, G);
    // Running
    bool r_rafa = bellmanFord(G, 0, weight, pred, dist);
    CHECK_BELLMAN_FORD(G, 0, weight, pred, dist);
    std::cout << "[+] Test OK!" << std::endl;
}

void benchmark(Graph g, int times)
{
    std::cout << "[i] " << times << " time(s)" << std::endl;
    int i;
    // setup
    unsigned long  N = num_vertices(g);

    // **************************************************** BOOST
    // init
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);
    std::vector<int> b_dist(N, (std::numeric_limits < short >::max)());
    std::vector<std::size_t> b_pred(N);
    // run
    bool r_boost;
    clock_t begin = clock();
    for(i = 0; i<times ; ++i){
        for (unsigned long i = 0; i < N; ++i) b_pred[i] = i;
        int s = (times == 1) ? 0 : randomRange(0, (int)N);
        b_dist[s] = 0;
        r_boost = boost::bellman_ford_shortest_paths(
            g, int(N), weight_pmap, 
            &b_pred[0], &b_dist[0], 
            boost::closed_plus<int>(),
            std::less<int>(), 
            boost::default_bellman_visitor());
        if(!r_boost) break;
    }    
    clock_t end = clock();
    double elapsed_secs_boost = double(end-begin) / CLOCKS_PER_SEC / (i+1);
    // Info
    std::cout << "      Average time boost: " << elapsed_secs_boost << " seconds";
    if(!r_boost) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;
    
    // **************************************************** LEDA
    // init
    leda::GRAPH<unsigned, long>  G = convertToLeda(g);
    leda::edge_array<long> l_weight = G.edge_data();
    leda::node_array<leda::edge> l_pred(G);  
    leda::node_array<long> l_dist(G);
    // run
    bool r_leda;
    begin = clock();
    for(i = 0; i<times ; ++i) {    
        leda::node s =  (times == 1) ? G.first_node() : G.choose_node();
        r_leda = leda::BELLMAN_FORD_B_T(G, s, l_weight, l_dist, l_pred);
    }
    end = clock();
    double elapsed_secs_leda = double(end- begin) / CLOCKS_PER_SEC / times;
    // Info
    std::cout << "      Average time leda: " << elapsed_secs_leda << " seconds";
    if(!r_leda) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;

    // **************************************************** RAFA
    // init
    std::vector<unsigned long> pred(N);
    std::vector<long> dist(N, (std::numeric_limits < short >::max)());
    // run
    begin = clock();
    bool r_rafa;
    for(i = 0; i<times ; ++i) {
        int s = (times == 1) ? 0 : randomRange(0, (int)N);
        r_rafa = bellmanFord(g, s, weight_pmap, pred, dist);
    }

    end = clock();
    CHECK_BELLMAN_FORD(g, 0, weight_pmap, pred, dist); // Checking only the last result
    double elapsed_secs_rafa = double(end - begin) / CLOCKS_PER_SEC / times;
    // Info
    std::cout << "      Average time rafa: " << elapsed_secs_rafa << " seconds";
    if(!r_rafa) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;
}