#include "../incl/bellman_ford.h"
#include <queue>
#include <stack>

/**
 * Bellman Ford algorithm.
 * Takes as argument a graph, a start node and a weight map,
 * and calculates the distance of each node, and it's predecessor.
*/
bool bellmanFord(const Graph& G, const int& s, const WeightMap& weight, 
    std::vector<unsigned long>& pred, std::vector<long>& dist)
{ 
    // init
    int N = num_vertices(G);
    int phase_count = 0;
    std::queue<int> Q;
    std::vector<bool> in_Q(N, false);
    const Vertex NIL =  boost::graph_traits<Graph>::null_vertex(); // end marker
    for (unsigned long i = 0; i < N; ++i) pred[i] = i;

    dist[s] = 0;
    Q.emplace(s); 
    in_Q[s] = true;
    Q.emplace(NIL);

    // basic step
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

    // postprocessing
    if (pred[s] != s) return false;

    // postprocessing init
    std::vector<bool> in_R(N, false);
    std::vector<bool> reached_from_node_in_U(N,false);

    // Creating a subgraph with all adges in shortest path
    Graph g;
    EdgeIterator ei, ei_end;
    for(boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (source(*ei,G) == pred[target(*ei,G)] ) {
            add_edge(source(*ei,G), target(*ei,G), g);
        }
    }
    // reachable by s -> in_R
    dfs(g, s, in_R);

    // update predecessors.
    for(boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (in_Q[v] && !reached_from_node_in_U[v]) {
            updatePred(G, v, in_R, reached_from_node_in_U, pred);
        }
    }
    
    return false;
}

/**
 * Depth First Search algorithm.
 * Given a graph and a start node marks all reachable nodes in found vector.
*/
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

/**
 * Bellman Ford postprocessing helper function.
 * Updates the predecessor of vertex @param(v) and it's successors.
*/
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

/**
 * Belman Ford Algorithm checker.
 * Given a Graph, a start node, and weight map 
 * and bellman ford results (predecessor and distance vectors)
 * checks the algorithm results.
 * If one contition fail, the program terminates.
 * If success, returns the labels V-, Vf, V+ for each vertex.
*/
std::vector<int> CHECK_BELLMAN_FORD(const Graph& G, const int& s, const WeightMap& weight, std::vector<unsigned long>& pred, std::vector<long>& dist)
{
    // init
    unsigned long  n_vertices = num_vertices(G);
    enum{ NEG_CYCLE = -2, ATT_TO_CYCLE = -1, FINITE = 0, PLUS = 1, CYCLE = 2, ON_CUR_PATH = 3, UNKNOWN = 4 };
    std::vector<int> label(n_vertices, UNKNOWN);
    std::vector<bool> reachable(n_vertices, false);

    dfs(G,s,reachable);
    VertexIterator v, v_end;
    for(boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
        if (*v != s) {
            // if v in V+, their is a path s-v
            assert( (pred[*v] == *v) == (reachable[*v] == false));
            if (reachable[*v] == false) label[*v]=PLUS;
        }
    }

    // classification of nodes
    if (pred[s]==s) label[s]=FINITE;
    for(boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
        if(label[*v]==UNKNOWN) {
            std::stack<Vertex> S;
            Vertex w = *v;
            while (label[w]==UNKNOWN) {
                label[w]=ON_CUR_PATH;
                S.push(w);
                w = pred[w];
            }
            // label all nodes on current path
            int t = label[w];
            if ( t == ON_CUR_PATH ) {
                Vertex z;
                do { 
                    z = S.top(); S.pop();
                    label[z] = CYCLE;
                } while ( z != w );

                while ( !S.empty() ) {
                    label[S.top()] = ATT_TO_CYCLE;
                    S.pop();
                }
            } else { // t is CYCLE, ATT_TO_CYCLE, or FINITE
                if ( t == CYCLE ) t = ATT_TO_CYCLE;
                while ( !S.empty() ) {
                    label[S.top()] = t;
                    S.pop();
                }
            }
        }
    }

    for(boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
         if ( label[*v] == CYCLE ){
            Vertex w = *v;
            long cycle_length = 0;
            do { 
                cycle_length += weight[edge(pred[w], w, G).first];
                label[w] = NEG_CYCLE;
                w = pred[w];
            } while (w != *v);
            // All cycles in P have negative weight
            assert(cycle_length < 0);
         }
    }

    if ( label[s] == FINITE ) assert(dist[s] == 0);
    EdgeIterator ei, ei_end;
    for (tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
        Vertex v = source(*ei, G);
        Vertex w = target(*ei, G);
        if ( label[w] == FINITE ) {
            // Their is no edge(v,w) where v in V- and w in Vf
            assert( label[v] == FINITE || label[v] == PLUS);
            // For each e(v,w) if v and w in Vf, dist[v] + weight[e] >= dist[w]
            if ( label[v] == FINITE ) {
                assert( dist[v] + weight[*ei] >= dist[w] );
                // For each vertex v in Vf,
                // If v=s then dist[v]=0
                // Else dist[v] = dist[u] + weight[edge(u,v)]
                if ( source(*ei, G) == pred[w] ) assert( dist[v] + weight[*ei] == dist[w] );
            }
        }
    }

    return label;
}