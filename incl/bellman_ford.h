#ifndef BF_BELLMAN_FORD_H
#define BF_BELLMAN_FORD_H

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/relax.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/concept/assert.hpp>

#include "../incl/graph_creator.h"

namespace bf {
    using namespace boost;

    template<class EdgeListGraph, class Size, class WeightMap,
            class PredecessorMap, class DistanceMap,
            class BinaryFunction, class BinaryPredicate,
            class BellmanFordVisitor>
    bool bellman_ford_shortest_paths(EdgeListGraph &g, Size N,
                                     WeightMap weight,
                                     PredecessorMap pred,
                                     DistanceMap distance,
                                     BinaryFunction combine,
                                     BinaryPredicate compare,
                                     BellmanFordVisitor v)
    {
        EdgeIterator i, end;
        for (Size k = 0; k < N; ++k) {
            bool at_least_one_edge_relaxed = false;
            for (boost::tie(i, end) = edges(g); i != end; ++i) {
                v.examine_edge(*i, g);
                if (relax(*i, g, weight, pred, distance, combine, compare)) {
                    at_least_one_edge_relaxed = true;
                    v.edge_relaxed(*i, g);
                } else
                    v.edge_not_relaxed(*i, g);
            }
            if (!at_least_one_edge_relaxed)
                break;
        }

        for (boost::tie(i, end) = edges(g); i != end; ++i)
            if (compare(combine(get(distance, source(*i, g)), get(weight, *i)), get(distance, target(*i, g)))) {
                v.edge_not_minimized(*i, g);
                return false;
            } else
                v.edge_minimized(*i, g);

        return true;
    }
}

#endif //BF_BELLMAN_FORD_H
