#ifndef BF_BELLMAN_FORD_BENCHMARK_H
#define BF_BELLMAN_FORD_BENCHMARK_H

#include "../incl/graph_creator.h"
#include "../incl/bellman_ford.h"

#include <boost/graph/bellman_ford_shortest_paths.hpp>

#include <LEDA/graph/templates/shortest_path.h>

#define RUNTIMES 10

void benchmark(Graph);

#endif //BF_BELLMAN_FORD_BENCHMARK_H
