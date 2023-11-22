#include <iostream>
#include <iomanip>
#include <chrono>
#include "SocialGraph.h"

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    SocialGraph graph;
    graph.InstantiateUsers(100000);
    graph.DescribeUser();
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Complete. Took " << duration * 0.000001 << " seconds." << std::endl;
    return 0;
}
