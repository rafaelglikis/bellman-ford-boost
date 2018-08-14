#include <iostream>
#include <fstream>

#include "../incl/graph_printer.h"
#include <iomanip>
/**
 * Makes an edge writer.
 * Source: https://www.boost.org/doc/libs/1_62_0/libs/graph/example/bellman-example.cpp 
*/
template <typename Graph, typename Parent>
edge_writer <Graph, Parent> make_edge_writer(const Graph & g, const Parent & p)
{
    return edge_writer < Graph, Parent > (g, p);
}

/**
 * Writes an edge to the given file.
 * Source: https://www.boost.org/doc/libs/1_62_0/libs/graph/example/bellman-example.cpp 
*/
template < typename Graph, typename ParentMap >
template < typename Edge >
void edge_writer<Graph, ParentMap>::operator() (std::ostream & out, const Edge & e) const
{
    out << "[label=\"" << get(boost::edge_weight, m_g, e) << "\"";
    typename boost::graph_traits < Graph >::vertex_descriptor
            u = source(e, m_g), v = target(e, m_g);
    if (m_parent[v] == u)
        out << ", color=\"black\"";
    else
        out << ", color=\"grey\"";
    out << "]";
}


/**
 * Prints a graph viz script for the given graph and it's shortest paths to the given file,
 * given the graph and the vartex predecessor vector 
*/
void printGraphShortestPathVizToFile(Graph g, std::vector<unsigned long> pred, std::string filename)
{
    EdgeIterator ei, ei_end;

    std::ofstream dot_file(filename);
    dot_file << "digraph D {\n"
             << "  rankdir=LR\n"
             << "  size=\"5,3\"\n"
             << "  ratio=\"fill\"\n"
             << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    Vertex u,v;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        Edge e = *ei;
        u = source(e, g);
        v = target(e, g);
        dot_file << u << " -> " << v << "[label=\"" << get(get(&EdgeProperties::weight, g), e) << "\"";
        if (pred[v] == u) dot_file << ", color=\"black\"";
        else dot_file << ", color=\"grey\"";
        dot_file << "]";
    }
    dot_file << "}";
}

/**
 * Prints a graph viz script for the given graph and it's shortest paths to the given file,
 * given the graph and the vartex predecessor vector
*/
void printGraphShortestPathViz(Graph g, std::vector<unsigned long> pred)
{
    EdgeIterator ei, ei_end;

    std::cout << "digraph D {\n"
                 << "  rankdir=LR\n"
                 << "  size=\"5,3\"\n"
                 << "  ratio=\"fill\"\n"
                 << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    Vertex u,v;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        Edge e = *ei;
        u = source(e, g);
        v = target(e, g);
        std::cout << "  " << u << " -> " << v << "[label=\"" << get(get(&EdgeProperties::weight, g), e) << "\"";
        if (pred[v] == u) std::cout << ", color=\"black\"";
        else std::cout << ", color=\"grey\"" ;
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
}

/**
 * Prints a graph viz script for the given graph to the given file.
*/
void printGraphVizToFile(Graph G, std::string filename)
{
    std::ofstream gout;
    gout.open(filename);

    gout << "digraph G" << std::endl;
    gout << "{" << std::endl;

    VertexIterator v, v_end;
    for(boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
        gout << "   " << *v << ";" << std::endl;
    }

    EdgeIterator e, e_end;
    for(boost::tie(e, e_end) = boost::edges(G); e != e_end; ++e) {
        gout << "   " << source(*e, G) << " -> " << target(*e, G);
        gout << " [label=" << G[*e].weight << "];" << std::endl;
    }

    gout << "}" << std::endl;
}

/**
 * Prints a graph viz script for the given graph.
*/
void printGraphViz(Graph G)
{
    std::cout << "digraph G" << std::endl;
    std::cout << "{" << std::endl;

    VertexIterator v, v_end;
    for(boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
        std::cout << "   " << *v << ";" << std::endl;
    }

    EdgeIterator e, e_end;
    for(boost::tie(e, e_end) = boost::edges(G); e != e_end; ++e) {
        std::cout << "   " << source(*e, G) << " -> " << target(*e, G);
        std::cout << " [label=" << G[*e].weight << "];" << std::endl;
    }

    std::cout << "}" << std::endl;
}

/**
 * Prints the bellman ford, and checker results.
*/
void printGraphShortestPath(Graph g, std::vector<long> distance, std::vector<unsigned long> pred, std::vector<int> label)
{
    const int INF = (std::numeric_limits < int >::max)();

    std::cout << std::setw(6) << "node" 
        << std::setw(10) << "dist" 
        << std::setw(7) << "pred" 
        << std::setw(5) << " label" << std::endl;

    for (int i = 0; i < num_vertices(g); ++i) {
        std::cout << "[" << std::setw(4) << i << "]";
        if (distance[i] == INF) std::cout << std::setw(10) << "Inf";
        else std::cout << std::setw(10) << distance[i];
        
        if (pred[i]!=i) std::cout << " " << "[" << std::setw(4) << pred[i]  << "]";
        else std::cout << std::setw(7) << "";
        
        if (label[i]==0) std::cout << std::setw(5) << "Vf";
        else if(label[i]<0) std::cout << std::setw(5) << "V-";
        else if(label[i]>0) std::cout << std::setw(5) << "V+";
        
        std::cout << std::endl;
    }
}
