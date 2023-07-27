#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include "priorityqueue.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) 
{
    std::ifstream file(argv[1]); // read the input file from the first command line
    json op = json::parse(file); // parse the input as a json object
    size_t max_sz = op["metadata"]["maxHeapSize"]; // grab the max size from the input column metadata,
    PriorityQueue pq(max_sz); // and create a Priority queue object with the max size
    int num_op = 0; // accounting for the number of operations in the output
    for(auto it = op.begin(); it!=op.end(); ++it) //loop through the data 
    {
        if(it.key()=="metadata") continue; // don't count metadata as input
        if(op[it.key()]["operation"] == "insert") // if the operation is insert, do the following
        {
            Key key = (op[it.key()]["key"]); // only if the operation is "insert," we create a double to store the number
            pq.insert(key);
        } 
        else if(op[it.key()]["operation"] == "removeMin") pq.removeMin(); // if the operation is remove, do the following
        ++num_op; // after action is done
    }
    json output = pq.JSON(); //get the final json object
    output["metadata"]["numOperations"] = num_op; // the column numOperations is the num_op recorded in the former loop
    std::cout << output.dump(2) << "\n"; //print the json object to the console
}
