#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <cmath>
#include <queue>
#pragma once

class SocialGraph {
private:
    int max_num_user_IDs = 999999999; //User creation will get slow if number of users approaches this number.
    int user_count = 0;
    std::unordered_map<int, std::vector<std::pair<int, float>>> user_map; //The adjacency map itself
    std::vector<int> user_list; //Primarily for quick instantiation of users.
    int WeightedRandomDistribution(int input);
public:
    void ListAllUsers(); //DEBUG FUNCTION
    void DescribeUser(int user);
    int ValidUser(int user);
    int GetUserNum(int i); //DEBUG FUNCTION
    int GetUserFollowing(int i, int j); //DEBUG FUNCTION
    void InstantiateUsers(int num_users);
    void DijkstrasAlgorithm(int source_id, int destination_id);
    void FloydWarshallAlgorithm(int source_id, int destination_id);
};

