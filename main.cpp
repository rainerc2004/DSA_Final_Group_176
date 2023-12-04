#include <iostream>
#include <iomanip>
#include <chrono>
#include "SocialGraph.h"
int main() {
    int users, selection, random, userA, userB;
    bool exit = false;
    bool valid = false;

    std::cout << "Welcome to our Project: Social Graph" << std::endl;
    while(!valid) {
        std::cout << "Please input the number of users between 1000 and 100000:" << std::endl;
        std::cin >> users;
        std::cout << std::endl;
        if(users < 1000 || users > 100000) {
            std::cout << "That is not a valid amount of users" << std::endl;
        }
        else {
            valid = true;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    SocialGraph graph;
    graph.InstantiateUsers(users);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Graph generation complete. Took " << duration * 0.000001 << " seconds." << std::endl;

    while(!exit) {
        std::cout << "Please select one of the options below:" << std::endl;
        std::cout << "1. Get a random ID from the generated graph" << std::endl;
        std::cout << "2. Get a particular ID's adjacent nodes and weights" << std::endl;
        std::cout << "3. Traverse the generated graph using Dijkstra's Algorithm" << std::endl;
        std::cout << "4. Traverse the generated graph using the Bellman Ford Algorithm" << std::endl;
        std::cout << "5. Traverse the generated graph using the Floyd-Warshall Algorithm" << std::endl;
        std::cout << "6. Exit the program" << std::endl;
        std::cin >> selection;
        std::cout << std::endl;

        if (selection == 1) {
            random = rand() % users;
            random =  graph.GetUserNum(random);
            std::cout << "Randomly selected ID:" << random << std::endl;

        } else if (selection == 2) {
            valid = false;
            while(!valid) {
                std::cout << "What ID would you like to find out more about?" << std::endl;
                std::cin >> selection;
                std::cout << std::endl;
                if(selection == -1){
                    valid = true;
                }
                else if(graph.ValidUser(selection) == 0) {
                    std::cout << "That is not a valid user. If you do not know the ID of a user type -1." << std::endl;
                }
                else {
                    graph.DescribeUser(selection);
                    valid = true;
                }
            }

        } else if (selection == 3) {
            valid = false;
            while(!valid) {
                std::cout << "Please type the two IDs you want to perform Dijkstra's algorithm on." << std::endl;
                std::cout << "Separate the two users with a space." << std::endl;
                std::cin >> userA >> userB;
                std::cout << std::endl;
                if (userA == -1 || userB == -1) {
                    valid = true;
                }
                else if (graph.ValidUser(userA) == 0 || graph.ValidUser(userB) == 0) {
                    std::cout << "One of your users is not a valid user. If you do not know the ID of a user type -1 twice." << std::endl;
                }
                else {
                    start = std::chrono::high_resolution_clock::now();
                    graph.DijkstrasAlgorithm(userA, userB);
                    end = std::chrono::high_resolution_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                    std::cout << "Dijkstra's algorithm complete. Took " << duration * 0.000001 << " seconds." << std::endl;
                    valid = true;
                }
            }

        } else if (selection == 4) {
            valid = false;
            while(!valid) {
                std::cout << "Please type the two IDs you want to perform the Bellman Ford algorithm on." << std::endl;
                std::cout << "Separate the two users with a space." << std::endl;
                std::cin >> userA >> userB;
                std::cout << std::endl;
                if (userA == -1 || userB == -1) {
                    valid = true;
                }
                else if (graph.ValidUser(userA) == 0 || graph.ValidUser(userB) == 0) {
                    std::cout << "One of your users is not a valid user. If you do not know the ID of a user type -1 twice." << std::endl;
                }
                else {
                    start = std::chrono::high_resolution_clock::now();
                    graph.BellmanFordAlgorithm(userA, userB);
                    end = std::chrono::high_resolution_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                    std::cout << "Bellman Ford algorithm complete. Took " << duration * 0.000001 << " seconds." << std::endl;
                    valid = true;
                }
            }

        }else if (selection == 5) {
            valid = false;
            while(!valid) {
                std::cout << "Please type the two IDs you want to perform the Floyd-Warshall algorithm on." << std::endl;
                std::cout << "Separate the two users with a space." << std::endl;
                std::cin >> userA >> userB;
                std::cout << std::endl;
                if (userA == -1 || userB == -1) {
                    valid = true;
                }
                else if (graph.ValidUser(userA) == 0 || graph.ValidUser(userB) == 0) {
                    std::cout << "One of your users is not a valid user. If you do not know the ID of a user type -1 twice." << std::endl;
             }
                else {
                start = std::chrono::high_resolution_clock::now();
                graph.FloydWarshallAlgorithm(userA, userB);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Floyd-Warshall algorithm complete. Took " << duration * 0.000001 << " seconds." << std::endl;
                valid = true;
                }
            }
        }else if (selection == 6) {
            std::cout << "Thank you for using our program. Have a great day!" << std::endl;
            exit = true;
        } else {
            std::cout << "Input was an unexpected value." << std::endl;
        }
    }

    return 0;
}
