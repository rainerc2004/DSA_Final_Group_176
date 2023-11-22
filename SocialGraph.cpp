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
    std::uniform_int_distribution<std::mt19937::result_type> rng_dist(10, 700);
    int num_followees, num_interactions, sum_interactions = 0;

    //Step two: generate followee vectors for each user in user_map, with weights for number of interactions
    for (auto it = user_map.begin(); it != user_map.end(); it++) {
        num_followees = WeightedRandomDistribution(rng_dist(rng));

        for (int i = 0; i < num_followees; i++) { //Initialize followed user IDs and weights with raw interaction numbers.
            num_interactions = WeightedRandomDistribution(rng_dist(rng));
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