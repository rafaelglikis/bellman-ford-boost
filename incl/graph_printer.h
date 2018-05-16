#ifndef BF_GRAPH_PRINTER_H
#define BF_GRAPH_PRINTER_H

#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

#include "graph_creator.h"



template < typename Graph, typename ParentMap >
struct edge_writer
{
    edge_writer(const Graph & g, const ParentMap & p): m_g(g), m_parent(p){}

    template < typename Edge > void operator() (std::ostream&, const Edge&) const;

    const Graph & m_g;
    ParentMap m_parent;
};
template < typename Graph, typename Parent >
edge_writer < Graph, Parent > make_edge_writer(const Graph & g, const Parent & p);

// Graph Viz Printers
void printGraphVizToFile(Graph, std::string);
void printGraphShortestPathVizToFile(Graph, std::vector<std::size_t>, std::string);

// cli printers
void printGraphShortestPath(Graph, std::vector<int>, std::vector<std::size_t>);
void printGraph(Graph);

#endif //BF_GRAPH_PRINTER_H
