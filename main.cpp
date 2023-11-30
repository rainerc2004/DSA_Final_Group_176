#include <iostream>
#include <iomanip>
#include <chrono>
#include "SocialGraph.h"

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    SocialGraph graph;
    graph.InstantiateUsers(100000);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Graph generation complete. Took " << duration * 0.000001 << " seconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    graph.DijkstrasAlgorithm(graph.GetUserNum(0), graph.GetUserNum(1));
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Dijkstra's algorithm complete. Took " << duration * 0.000001 << " seconds." << std::endl;
    return 0;

}
