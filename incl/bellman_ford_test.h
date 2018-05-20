#ifndef BF_BELLMAN_FORD_TEST_CPP_H
#define BF_BELLMAN_FORD_TEST_CPP_H

#include "../incl/graph_creator.h"
#include "../incl/graph_printer.h"

#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <LEDA/graph/templates/shortest_path.h>

// Tests
void testAll();
void testBF(Graph g);
void benchmark(Graph, int times);

#endif //BF_BELLMAN_FORD_TEST_CPP_H
