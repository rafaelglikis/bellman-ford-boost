#include "../incl/bellman_ford_test.h"

void test(Graph g)
{
    unsigned long  N = num_vertices(g);
    WeightMap weight_pmap = get(&EdgeProperties::weight, g);

    std::vector<int> distance(N, (std::numeric_limits < short >::max)());
    std::vector<int> mDistance(N, (std::numeric_limits < short >::max)());
    std::vector<std::size_t> parent(N);
    std::vector<std::size_t> mParent(N);
    for (unsigned long i = 0; i < N; ++i){
        mParent[i] = i;
        parent[i] = i;
    }

    distance[4] = 0;
    mDistance[4] = 0;

    bool boost_result = boost::bellman_ford_shortest_paths(g, int(N), weight_pmap,
                                                           &parent[0], &distance[0],
                                                           boost::closed_plus<int>(),
                                                           std::less<int>(),
                                                           boost::default_bellman_visitor());

    bool my_result = bf::bellman_ford_shortest_paths(g, int(N), weight_pmap,
                                                        &mParent[0], &mDistance[0],
                                                        boost::closed_plus<int>(),
                                                        std::less<int>(),
                                                        boost::default_bellman_visitor());


    if (boost_result != my_result) {
        std::cout << "[-] Test failed" << std::endl;
        return;
    }
    std::cout << "[+] Negative Cycle detection OK" << std::endl;

    if(!boost_result) {
        std::cout << "[i] Necative cycle detected.. Exiting test.." << std::endl;
        return;
    }

    for (int i = 0; i < N; ++i){
        if(mParent[i] != parent[i]) {
            std::cout << "[-] Parent node test failed" << std::endl;
            return;
        };
    }
    std::cout << "[+] Parent node test OK" << std::endl;

    for (int i = 0; i < N; ++i){
        if(mDistance[i] != distance[i]) {
            std::cout << "[-] Distance test failed" << std::endl;
            return;
        };
    }
    std::cout << "[+] Distance test OK" << std::endl;
}
