#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include <algorithm>
#include "priorityqueue.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) 
{
    std::ifstream file(argv[1]); // read the input file from the first command line
    json data = json::parse(file); // parse the input as a json object
    json output; // final json object to be printed
    size_t max_sz = data["metadata"]["numPlayers"]; // grab the max size from the input column metadata,
    PriorityQueue pq(max_sz); // and create a Priority queue object with the max size
    std::vector<Value> id; // a vector of pairs of two int to store player combinations
    int it_id = 0; // iterator for id vector
    for (auto& it : data["teamStats"]) // looop  through the given input data
    {
        double win_p  = it["winPercentage"]; // get the winPercentage of each combination
        id.push_back(std::make_pair(it["playerOne"], it["playerTwo"])); // store two players id in vector
        Key diff = fabs(win_p - 50.0); // calculate how close are their winPercentage to 50
        KeyValuePair kv = std::make_pair(diff, id[it_id]); // make the above data into a pair, 
        pq.insert(kv); // and insert in the priority queue
        ++it_id; // iterate to next player combination
    }
    // after inserting and heapifying, start removing unnecessary data
    int sz = data["metadata"]["numPlayers"]; // get size
    std::vector<int> cand; // list of candidates
    std::vector<Value> final_team; // final players combinations
    for(int i=0; i<sz; ++i)
    {
        cand.push_back(i); // push all candidates' id inside the list
    }
    while(!cand.empty()) // as long one of the candidates is available for team comp
    {
        KeyValuePair player_kv = pq.removeMin(); // get the combination from pq that has closest win rate to 50
        int first = player_kv.second.first, second = player_kv.second.second; // get their id
        if(std::find(cand.begin(), cand.end(), first) != cand.end() && std::find(cand.begin(), cand.end(), second) != cand.end()) // if both of them are still available
        {
            cand.erase(std::remove(cand.begin(), cand.end(), first), cand.end()); // remove the player 1 id since it is in a new combination
            cand.erase(std::remove(cand.begin(), cand.end(), second), cand.end()); // remove the player 2 id since it is in a new combination
            Value team = std::make_pair(first, second); // make these two into a pair, 
            final_team.push_back(team); // push the team comp to the vector
        }
    }
    output["teams"] = final_team; // output json object reocrd the vector of all final teams and player id
    std::cout<<output.dump(2)<<"\n"; // print output to the console
}