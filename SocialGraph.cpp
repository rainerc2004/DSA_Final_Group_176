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

void SocialGraph::DescribeUser(int user) { //Lists every account the input user follows
    auto it = user_map.find(user);
    std::cout << it->first << std::endl;
    for (int i = 0; i < it->second.size(); i++) {
        std::cout << "     " << it->second[i].first << ", " << it->second[i].second << std::endl;
    }

}
int SocialGraph::ValidUser(int user) {
    auto it = user_map.count(user);
    return it;
}

int SocialGraph::GetUserNum(int i) {
    return user_list[i];
}

int SocialGraph::GetUserFollowing(int i, int j) {
    return user_map[user_list[i]][j].first;
}

void SocialGraph::InstantiateUsers(int num_users) {
    user_count = num_users;
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
            if (strength[target_id] < strength[current_id] * target_weight) {
                jumps[target_id] = jumps[current_id] + 1;
                predecessors[target_id] = current_id;
                strength[target_id] =  strength[current_id] * target_weight;

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

void SocialGraph::BellmanFordAlgorithm(int source_id, int destination_id) {
    int vertexcount = user_count;
    float destination[user_count];
    int sourceindex, destinationindex = -1;

    //Records source index and destination index, and instantiates "destination" vector holding strengths
    for (int i = 0; i < user_count; i++) {
        int tempid = user_list.at(i);
        if (tempid==source_id) {
            sourceindex = i;
        }
        if (tempid==destination_id) {
            destinationindex = i;
        }
        destination[i] = -1000.0;
    }
    destination[sourceindex] = 1.0;

    //Tightens edges |V| - 1 many times
    for (int i = 1; i <= vertexcount - 1; i++) {
        //Iterates through each vertex's edges (all edges on the graph)
        for (int k = 1; k < vertexcount; k++) {
            int tempid = user_list.at(k);
            int tempedge = user_map[tempid].size();
            for (int j = 0; j < tempedge; j++) {
                //Finds index of the endpoint of each edge, along with the edge's associated weight
                int edge_endpoint = user_map.at(tempid).at(j).first;
                auto edge_endpoint_it = std::find(user_list.begin(), user_list.end(), edge_endpoint);
                int edge_endpoint_index = edge_endpoint_it - user_list.begin();
                float weight = user_map.at(tempid).at(j).second;
                //Actual edge tightening comparison function
                if ((destination[k] != -1000) && (destination[k] * weight > destination[edge_endpoint_index])) {
                    destination[edge_endpoint_index] = destination[k] * weight;
                }
            }
        }
    }
    std::cout<< "The source node has " << destination[destinationindex] << " influence on the destination node." << std::endl;
}

void SocialGraph::FloydWarshallAlgorithm(int source_id, int destination_id) {
    //Instantiates a |users| x |users| matrix of strengths between two nodes
    std::vector<std::vector<float>> v(user_count, std::vector<float>(user_count, -INFINITY));
    for (int i = 0; i < user_count; i++) {
        for (int j = 0; j < user_count; j++) {
            if (i == j)
                v[i][j] = 0;
            else {
                for (int z = 0; z < user_map[user_list.at(i)].size(); z++) {
                    if (user_map[user_list.at(i)].at(z).first == user_list.at(j)) {
                        v[i][j] = user_map[user_list.at(i)].at(z).second;
                        break;
                    }
                }
            }
        }
    }

    //The edge-strengthening part of Floyd-Warshall:
    int k, i, j = 0;
    for (k = 0; k < user_count; k++) {
        for (i = 0; i < user_count; i++) {
            for (j = 0; j < user_count; j++) {
                if ((v[i][j] < v[i][k] * v[k][j]) && (v[k][j] != -INFINITY) && (v[i][k] != -INFINITY)) {
                    v[i][j] = v[i][k] * v[k][j];
                }
            }
        }
    }

    //Finds the indices of source_id and destination_id so their strengths can be accessed from "v"
    int source_index, destination_index = -5;
    for (int y = 0; y < user_list.size(); y++) {
        if (user_list[y] == source_id) {
            source_index = y;
        }
        if (user_list[y] == destination_id) {
            destination_index = y;
        }
        if (source_index != -5 && destination_index != -5) {
            break;
        }
    }

    //Returns strength from source to destination
    std::cout << "The source node has " << v[source_index][destination_index] << " influence on the destination node." << std::endl;
}