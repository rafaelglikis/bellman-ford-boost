#ifndef BF_GRAPH_H
#define BF_GRAPH_H

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/random.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/graph/make_connected.hpp"
#include "boost/graph/iteration_macros.hpp"


struct EdgeProperties {
    int weight;
};

// Graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, EdgeProperties> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// Iterators
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;

// Graph Properties
typedef boost::property_map<Graph, int EdgeProperties::*>::type WeightMap;

// Graph Funcitons
Graph createRandomGraph(unsigned long, unsigned long, int, int);
Graph createGridGraph(int, int, int);

// Test Graphs
Graph createTestGraph();
Graph createPositiveTestGraph();
Graph createTestGraphWithNegativeCycle();


// Helper Funtions
int randomRange(int, int);

#endif //BF_GRAPH_H
