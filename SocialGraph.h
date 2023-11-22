#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#pragma once

class SocialGraph {
private:
    int max_num_user_IDs = 999999999; //User creation will get slow if number of users approaches this number.
    std::unordered_map<int, std::vector<std::pair<int, float>>> user_map; //The adjacency map itself
    std::vector<int> user_list; //Primarily for quick instantiation of users.
    int WeightedRandomDistribution(int input);
public:
    void ListAllUsers(); //DEBUG
    void DescribeUser(); //DEBUG
    void InstantiateUsers(int num_users);
    void DijkstrasAlgorithm(int source_id, int destination_id);
};

