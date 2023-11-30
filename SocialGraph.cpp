#include "SocialGraph.h"
#pragma once

int SocialGraph::WeightedRandomDistribution(int input) { //Works for random input between 1-1000.
    return floor(2000 / sqrt(input)); //A nice distribution for followee/interaction counts
}

void SocialGraph::ListAllUsers() { //Debug, lists the user ID of every user.
    for (auto it = user_map.begin(); it != user_map.end(); it++) {
        std::cout << it->first << " {";
        for (int i = 0; i < it->second.size(); i++) {
            std::cout << "(" << it->second[i].first << " " << it->second[i].second << "), ";
        }
        std::cout << "}" << std::endl;
    }
}

void SocialGraph::DescribeUser() { //Debug, just lists every account the first instantiated user follows
    auto it = user_map.begin();
    std::cout << it->first << std::endl;
    for (int i = 0; i < it->second.size(); i++) {
        std::cout << "     " << it->second[i].first << ", " << it->second[i].second << std::endl;
    }
}

int SocialGraph::GetUserNum(int i) {
    return user_list[i];
}

int SocialGraph::GetUserFollowing(int i, int j) {
    return user_map[user_list[i]][j].first;
}

void SocialGraph::InstantiateUsers(int num_users) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> id_dist(1, max_num_user_IDs);

    //Step one: insert new users into user_map, with empty followee vectors and weights.
    for (int i = 0; i < num_users; i++) {
        while (true) { //If random ID is a duplicate, repeats until a unique user ID is found.
            int id = id_dist(rng);
            if (user_map.find(id) == user_map.end()) {
                std::vector<std::pair<int, float>> empty_vector;
                user_map.insert({id, empty_vector});
                user_list.push_back(id); //User list exists for quick ID access during later stages of user instantiation
                break;
            }
        }
    }

    std::uniform_int_distribution<std::mt19937::result_type> order_dist(0, user_map.size() - 1);
    std::uniform_int_distribution<std::mt19937::result_type> followees_rng_dist(10, 700);
    std::uniform_int_distribution<std::mt19937::result_type> interactions_rng_dist(10, 1000);
    int num_followees, num_interactions, sum_interactions = 0;

    //Step two: generate followee vectors for each user in user_map, with weights for number of interactions
    for (auto it = user_map.begin(); it != user_map.end(); it++) {
        num_followees = WeightedRandomDistribution(followees_rng_dist(rng));

        for (int i = 0; i < num_followees; i++) { //Initialize followed user IDs and weights with raw interaction numbers.
            num_interactions = WeightedRandomDistribution(interactions_rng_dist(rng)) - 60;
            sum_interactions += num_interactions;

            int order_num = order_dist(rng);
            it->second.emplace_back(user_list[order_num], num_interactions);
        }
        for (int i = 0; i < num_followees; i++) { //Second pass replaces weights with proportion of total interactions.
            it->second[i].second = it->second[i].second/sum_interactions;
        }
        num_followees = 0, num_interactions = 0, sum_interactions = 0;
    }
}

void SocialGraph::DijkstrasAlgorithm(int source_id, int destination_id) {
    std::priority_queue<std::pair<float, int>> next_up; //Priority queue storing (weight, id) pairs-- the largest weight pair is accessed first
    std::unordered_set<int> unvisited;
    std::unordered_map<int, float> strength; //Much like "distance" in the usual Dijkstra's Alg, except larger is better
    std::unordered_map<int, int> predecessors;
    std::unordered_map<int, int> jumps; //Number of edges a node is from the source

    //Initialization of data structures
    for (int i = 0; i < user_list.size(); i++) {
        unvisited.insert(user_list[i]);
        strength[user_list[i]] = -INFINITY;
        predecessors[user_list[i]] = -1;
        jumps[user_list[i]] = -1;
    }
    strength[source_id] = 1;
    jumps[source_id] = 0;
    next_up.push(std::make_pair(1, source_id));

    //Actual loop itself-- continues so long as next_up is nonempty
    while (true) {
        if (next_up.empty()) {
            break;
        }

        int current_id = next_up.top().second;
        next_up.pop();
        //If current_id has already been visited, skip this pass
        if (unvisited.find(current_id) == unvisited.end()) {
            continue;
        }
        unvisited.erase(current_id);

        //Checks each followee of current_id to see if
        for (int i = 0; i < user_map[current_id].size(); i++) {
            int target_id = user_map[current_id][i].first;
            float target_weight = user_map[current_id][i].second;

            //If route through current_id is stronger than previous route, tighten the edge
            if (strength[target_id] < strength[current_id] * (target_weight * pow(0.95, jumps[current_id] + 1))) {
                jumps[target_id] = jumps[current_id] + 1;
                predecessors[target_id] = current_id;
                strength[target_id] =  strength[current_id] * (target_weight * pow(0.95, jumps[current_id] + 1));

                //... and add the edge to the priority queue if it's unvisited
                if (unvisited.find(target_id) != unvisited.end()) {
                    next_up.push(std::make_pair(strength[target_id], target_id));
                }
            }
        }
    }
    if (strength[destination_id] == -INFINITY) {
        std::cout << "Destination node was unreachable from source node." << std::endl;
    } else {
        std::cout << "The source node has " << strength[destination_id] << " influence on the destination node." << std::endl;
    }
}

void SocialGraph::AStarAlgorithm(int source_id, int destination_id) {
    
}