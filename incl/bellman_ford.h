#ifndef BF_BELLMAN_FORD
#define BF_BELLMAN_FORD

#include "../incl/graph_creator.h"
#include <vector>

bool bellmanFord(const Graph&, const int&, const WeightMap&, 
    std::vector<unsigned long>&, std::vector<long>&);

inline void updatePred(const Graph&, const int&, std::vector<bool>&, 
    std::vector<bool>&, std::vector<unsigned long>&);

inline void dfs(const Graph&, const int&, std::vector<bool>&);

std::vector<int> CHECK_BELLMAN_FORD(const Graph&, const int&, 
    const WeightMap&, std::vector<unsigned long>&, std::vector<long>&);

#endif //BF_BELLMAN_FORD
