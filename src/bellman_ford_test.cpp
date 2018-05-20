#include "../incl/bellman_ford_test.h"
#include "../incl/bellman_ford.h"


void testBF(Graph G)
{
    const int INF = (std::numeric_limits < int >::max)();

    // **************************************************** RAFA
    // Preprocessing
    unsigned long  n_vertices = num_vertices(G);
    std::vector<unsigned long> pred(n_vertices);
    std::vector<long> dist(n_vertices, INF);
    WeightMap weight_pmap = get(&EdgeProperties::weight, G);
    for (std::size_t i = 0; i < n_vertices; ++i) pred[i] = i;
    printGraphVizToFile(G, "graph.dot");
    // Running
    bool r_rafa = bellmanFord(G, 0, weight_pmap, pred, dist);

    // **************************************************** LEDA
    // Preprocessing
    leda::GRAPH<unsigned, long>  g = convertToLeda(G);
    leda::edge_array<long> ledaWeight = g.edge_data();
    leda::node_array<leda::edge> ledaPred(g);  
    leda::node_array<long> ledaDist(g);
    leda::node ledaStartNode = g.first_node();
    // Running
    bool r_leda = leda::BELLMAN_FORD_B_T(g, ledaStartNode, ledaWeight, ledaDist, ledaPred);

    // **************************************************** TESTS
    // setup
    int i=0;
    leda::node v;
    // negative cycle test
    if (r_rafa != r_leda) {
        std::cout << "[-] Test negative cycle failed.." << std::endl;
        return;
    } 
    std::cout << "[+] Test negative cycle OK!" << std::endl;
    // dist test
    forall_nodes(v, g) {
        if (dist[i]!=ledaDist[v] && dist[i]!=INF && ledaDist[v]!=0) {
            std::cout << "[-] Test distances failed.." << std::endl;
            std::cout << "    Expected: " << ledaDist[v] <<  " Actual: " << dist[i] << " Vertex: " << i << std::endl;
            return;
        }
        ++i;
    }
    std::cout << "[+] Test distances OK!" << std::endl;
    // dist test
    i=0;
    forall_nodes(v, g) {
        if (ledaPred[v] != nil && pred[i] != g.source(ledaPred[v])->id()) {
            std::cout << "[-] Test pred failed.." << std::endl;
            std::cout << "    Expected: " << g.source(ledaPred[v])->id() <<  " Actual: " << pred[i] << " Vertex: " << i << std::endl;
            return;
        }
        ++i;
    }
    std::cout << "[+] Test pred OK!" << std::endl;
}

void benchmark(Graph g)
{
    // **************************************************** BOOST
    // Preprocessing
    unsigned long  N = num_vertices(g);
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);
    std::vector<int> distance(N, (std::numeric_limits < short >::max)());
    std::vector<std::size_t> parent(N);
    for (unsigned long i = 0; i < N; ++i) parent[i] = i;
    int startNode = 0; //randomRange(0, (int)N);
    distance[0] = 0;
    // Running
    clock_t begin = clock();
    bool r_boost = boost::bellman_ford_shortest_paths(
        g, int(N), weight_pmap, 
        &parent[0], &distance[0], 
        boost::closed_plus<int>(),
        std::less<int>(), 
        boost::default_bellman_visitor());
    clock_t end = clock();
    double elapsed_secs_boost = double(end- begin) / CLOCKS_PER_SEC;
    // Info
    std::cout << "      Time elapsed boost: " << elapsed_secs_boost << " seconds";
    if(!r_boost) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;
    
    // **************************************************** LEDA
    // Preprocessing
    leda::GRAPH<unsigned, long>  G = convertToLeda(g);
    leda::edge_array<long> ledaWeight = G.edge_data();
    leda::node_array<leda::edge> ledaPred(G);  
    leda::node_array<long> ledaDist(G);
    leda::node ledaStartNode = G.first_node();
    // Running
    begin = clock();
    bool r_leda = leda::BELLMAN_FORD_B_T(
        G, ledaStartNode, ledaWeight, ledaDist, ledaPred);
    end = clock();
    double elapsed_secs_leda = double(end- begin) / CLOCKS_PER_SEC;
    // Info
    std::cout << "      Time elapsed leda: " << elapsed_secs_leda << " seconds";
    if(!r_leda) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;

    // **************************************************** RAFA
    // Preprocessing
    std::vector<unsigned long> mParent(N);
    std::vector<long> mDistance(N, (std::numeric_limits < short >::max)());
    for (unsigned long i = 0; i < N; ++i) {
        mParent[i] = i;
    }
    // Running
    begin = clock();
    bool r_rafa = bellmanFord(g, 0, weight_pmap, mParent, mDistance);
    end = clock();
    double elapsed_secs_rafa = double(end - begin) / CLOCKS_PER_SEC;
    // Info
    std::cout << "      Time elapsed rafa: " << elapsed_secs_rafa << " seconds";
    if(!r_rafa) std::cout << " (negative cycle)" << std::endl;
    else std::cout << std::endl;
}