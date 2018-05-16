//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

#include "../incl/graph_creator.h"
#include "../incl/graph_printer.h"

using namespace boost;

const int INF = (std::numeric_limits < int >::max)();

int main()
{
    Graph g = createRandomGraph(10, 10, -100, 10000 );
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);
    unsigned long  n_vertices = num_vertices(g);

    std::vector<int> distance(n_vertices, INF);
    std::vector<std::size_t> pred(n_vertices);
    for (std::size_t i = 0; i < n_vertices; ++i) pred[i] = i;
    distance[0] = 0;

    bool r = bellman_ford_shortest_paths(g, n_vertices, weight_pmap, &pred[0], &distance[0],
                                         closed_plus<int>(), std::less<int>(), default_bellman_visitor());

//    bool r = bellman_ford_shortest_paths
//            (g, int (num_vertices(g)), weight_map(weight_pmap).distance_map(&distance[0]).
//                    predecessor_map(&pred[0]));

    if (r) {
        printGraphShortestPath(g, distance, pred);
    } else {
        std::cout << "negative cycle detected" << std::endl;
    }

    //printGraphShortestPath(g, pred);

    return EXIT_SUCCESS;
}