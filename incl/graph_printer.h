#ifndef BF_GRAPH_PRINTER_H
#define BF_GRAPH_PRINTER_H

#include <vector>
#include "graph_creator.h"

/**
 * Edge writer for graphviz to visualize shortest path.
 * Source: https://www.boost.org/doc/libs/1_62_0/libs/graph/example/bellman-example.cpp 
*/
template <typename Graph, typename ParentMap>
struct edge_writer
{
    edge_writer(const Graph & g, const ParentMap & p): m_g(g), m_parent(p){}

    template < typename Edge > void operator() (std::ostream&, const Edge&) const;

    const Graph & m_g;
    ParentMap m_parent;
};
template <typename Graph, typename Parent>
edge_writer < Graph, Parent > make_edge_writer(const Graph & g, const Parent & p);

// Graph Viz Printers
void printGraphViz(Graph);
void printGraphVizToFile(Graph, std::string);
void printGraphShortestPathViz(Graph, std::vector<unsigned long>);
void printGraphShortestPathVizToFile(Graph, std::vector<unsigned long>, std::string);

// cli printers
void printGraphShortestPath(Graph, std::vector<long>, std::vector<unsigned long>,  std::vector<int>);

#endif //BF_GRAPH_PRINTER_H
