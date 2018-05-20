#include "../incl/graph_printer.h"

template < typename Graph, typename Parent >
edge_writer < Graph, Parent >
make_edge_writer(const Graph & g, const Parent & p)
{
    return edge_writer < Graph, Parent > (g, p);
}

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
 * // http://dreampuf.github.io/GraphvizOnline/
 * @param filename
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

void printGraphShortestPath(Graph g, std::vector<long> distance, std::vector<std::size_t> pred)
{
    const int INF = (std::numeric_limits < int >::max)();

    for (int i = 0; i < num_vertices(g); ++i) {
        std::cout << "[" << std::setw(4) << i << "]";
        if (distance[i] == INF){
            std::cout << ": " << std::setw(6) << "Inf";
        } else {
            std::cout << ": " << std::setw(6) << distance[i];
        }
        if (pred[i]!=i) {
            std::cout << " " << "[" << std::setw(4) << pred[i]  << "]";
        }
        std::cout << std::endl;
    }
}

void printLedaGraphShortestPath(leda::graph& g, leda::node_array<long>& ledaDist, leda::node_array<leda::edge>& ledaPred)
{
    leda::node v;
    forall_nodes(v,g) {
        g.print_node(v);
        std::cout << std::setw(10) << ledaDist[v] << "  ";
        if(ledaPred[v] !=nil) g.print_edge(ledaPred[v]);
        std::cout << std::endl;
    }
}