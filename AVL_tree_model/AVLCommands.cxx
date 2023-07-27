#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include <memory>
#include <string> 
#include <fstream>

#include "AVL.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);
    json data = json::parse(file);
    AVL v;
    
    for(auto it = data.begin(); it!=data.end(); ++it) //loop through each sample in the input json object
    {
        if(it.key()=="metadata") continue;
        int key = data[it.key()]["key"];
        v.Insert(key);
    }
    std::cout<<v.JSON();
    return 0;
}