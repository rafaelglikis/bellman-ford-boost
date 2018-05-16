#include "../incl/graph_creator.h"

Graph createRandomGraph(unsigned long verices, unsigned long edges, int cost_min, int cost_max)
{
    Graph G;

    // Generating Random connected graph
    boost::mt19937 gen;
    generate_random_graph(G, verices, edges, gen, false, false);
    boost::make_connected<Graph>(G);

    // Converting to EdgeList graph
    std::vector<std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor> > pairlist;
    for (auto ed : boost::make_iterator_range(boost::edges(G))) {
        pairlist.emplace_back(source(ed, G), target(ed, G));
    }
    Graph g(pairlist.begin(), pairlist.end(), num_vertices(G));

    // Adding weights
    EdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
        g[*ei].weight = randomRange(cost_min, cost_max);
    }

    return g;
}

Graph createGridGraph(int n, int cost_min, int cost_max)
{
    Graph G; 
    for (int i = 0; i < n*n-1; ++i) {
        if( i%n == n-1) continue;
            
        if (i >= n*(n/2) && i%n < n/2 )
        {
            if (i == n*n/2 + n/2 -1 || i == n*n/2 + n/2) {
                continue;
            } else if(randomRange(0,1)){
                add_edge(i, i+1, G);
            } else {
                add_edge(i+1, i, G);    
            }
            continue;
        }
        add_edge(i, i+1, G);
    }

    for (int i = 0; i < n*n-n; ++i) {
        if (i >= n*(n/2-1) && i%n < n/2 ) {
            if (i == n*n/2 - n/2 - 1) {
                continue;
            } else if(randomRange(0,1)) {
                add_edge(i, i+n, G);
            } else {
                add_edge(i+n, i, G);    
            }
            continue;
        }
        add_edge(i, i+n, G);
    }

    // Converting to EdgeList graph
    std::vector<std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor> > pairlist;
    for (auto ed : boost::make_iterator_range(boost::edges(G))) {
        pairlist.emplace_back(source(ed, G), target(ed, G));
    }
    Graph g(pairlist.begin(), pairlist.end(), num_vertices(G));

    // Adding weights
    EdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
        g[*ei].weight = randomRange(cost_min, cost_max);
    }

    add_edge(n*n/2 + n/2, n*n/2 + n/2 - 1, g);
    g[edge(n*n/2 + n/2, n*n/2 + n/2 - 1, g).first].weight = -10000;
    add_edge(n*n/2 + n/2 - 1, n*n/2 - n/2 - 1, g);
    g[edge(n*n/2 + n/2 - 1, n*n/2 - n/2 - 1, g).first].weight = -10000;

    return g;
}

Graph createTestGraph()
{
    typedef std::pair < int, int >E;
    const int n_edges = 10;

    E edge_array[] = { E(0, 3), E(0, 2), E(0, 1), E(1, 0), E(2, 3), E(2, 1), E(3, 1), E(3, 4), E(4, 0), E(4,2) };
    int weight[n_edges] = { -4, 8, 5, -2, 9, -3, 7, 2, 6, 7 };

    Graph g(edge_array, edge_array + n_edges, 5);

    EdgeIterator ei, ei_end;
    int i = 0;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
        g[*ei].weight = weight[i];

    return g;
}

Graph createPositiveTestGraph()
{
    typedef std::pair < int, int >E;
    const int n_edges = 10;

    E edge_array[] = { E(0, 3), E(0, 2), E(0, 1), E(1, 0), E(2, 3), E(2, 1), E(3, 1), E(3, 4), E(4, 0), E(4,2) };
    int weight[n_edges] = { 4, 8, 5, 2, 9, 3, 7, 2, 6, 7 };

    Graph g(edge_array, edge_array + n_edges, 5);

    EdgeIterator ei, ei_end;
    int i = 0;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
        g[*ei].weight = weight[i];

    return g;
}

Graph createTestGraphWithNegativeCycle()
{
    typedef std::pair < int, int >E;
    const int n_edges = 10;

    E edge_array[] = { E(0, 3), E(0, 2), E(0, 1), E(1, 0), E(2, 3), E(2, 1), E(3, 1), E(3, 4), E(4, 0), E(4,2) };
    int weight[n_edges] = { -4, 8, 5, -2, -9, -3, 7, -2, 6, 7 };

    Graph g(edge_array, edge_array + n_edges, 5);

    //WeightMap weight_pmap = get(&EdgeProperties::weight, g);

    EdgeIterator ei, ei_end;
    int i = 0;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
        g[*ei].weight = weight[i];

    return g;
}

int randomRange(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

