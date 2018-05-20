#include "../incl/bellman_ford.h"
#include <queue>

bool bellmanFord(const Graph& G, const int& s, const WeightMap& weight, 
    std::vector<unsigned long>& pred, std::vector<long>& dist)
{ 
    // TODO use emplace
    int N = num_vertices(G);
    int phase_count = 0;
    std::queue<int> Q;
    std::vector<bool> in_Q(N, false);
    const Vertex NIL =  boost::graph_traits<Graph>::null_vertex();
    
    dist[s] = 0;
    Q.emplace(s); 
    in_Q[s] = true;
    Q.emplace(NIL); // end marker

    Vertex v, u;
    OutEdgeIterator oei, oei_end;
    while(phase_count < N) {
        u = Q.front(); Q.pop();
        if ( u == NIL) { 
            phase_count++;
            if (Q.empty()) return true;
            Q.emplace(NIL);
            continue;
        } else {
            in_Q[u] = false;
        }

        long du = dist[u];

        for(boost::tie(oei, oei_end) = out_edges(u, G); oei != oei_end; ++oei) {
            v = target(*oei,G);
            long d = du + weight[*oei];
            if ( (pred[v] == v && v != s) || d < dist[v] ) { 
                dist[v] = d; 
                pred[v] = u;
                if ( !in_Q[v] ) { 
                    Q.emplace(v); 
                    in_Q[v] = true; 
                }
            }
        }   
    }

    if (pred[s] != s) return false;

    std::vector<bool> in_R(N, false);
    std::vector<bool> reached_from_node_in_U(N,false);

    EdgeIterator ei, ei_end;


    // ! DANGER AREA

    //forall_edges(e,G) 
    //    if (e != pred[G.target(e)]) ((graph*) &G)->hide_edge(e);

    Graph g;
    for(boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (source(*ei,G) == pred[target(*ei,G)] ) {
            add_edge(source(*ei,G), target(*ei,G), g);
        }
    }

    dfs(g, s, in_R);

    // ((graph*) &G)->restore_all_edges();

    // ! DANGER AREA

    for(boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (in_Q[v] && !reached_from_node_in_U[v]) {
            updatePred(G, v, in_R, reached_from_node_in_U, pred);
        }
    }
    
    return false;
}

inline void dfs(const Graph& G, const int& s, std::vector<bool>& found)
{
    found[s] = true;
    Vertex v;
    OutEdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = out_edges(s, G); ei != ei_end; ++ei) {
        v = target(*ei,G);
        if(!found[v]) dfs(G, v, found);
    }
}


inline void updatePred(const Graph& G, const int& v,
    std::vector<bool>& in_R,
    std::vector<bool>& reached_from_node_in_U,
    std::vector<unsigned long>& pred)
{ 
    reached_from_node_in_U[v] = true;
    OutEdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = out_edges(v, G); ei != ei_end; ++ei){
        unsigned long w = target(*ei, G);
        if (!reached_from_node_in_U[w]) { 
            if ( in_R[w] ) pred[w] = v;
            updatePred(G, w, in_R, reached_from_node_in_U, pred);
        }
    }
}