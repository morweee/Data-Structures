#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
    std::ifstream file(argv[1]); // takes the file by the name of first command line argument
    json data = json::parse(file); // data is the parse json object where the file is parsed into  
    json output; // output is the json object to store all results 
    int sample_w_inversion = 0; // sample_w_inversion gets how many times inversion occurs

    for(auto it = data.begin(); it!=data.end(); ++it) //loop through each sample in the input json object
    {
        if(it.key()=="metadata") continue; //if loop to metadata key, break 
        output[it.key()]["ConsecutiveInversions"]; // ConsecutiveInversions is a key to store later inverted values in the sample
        std::vector<int> sample = data[it.key()].get<std::vector<int>>(); // sample gets the integer vector from json object values
        bool found = false; // found is set to be false unless inversion is found
        for(unsigned int i = 0; i < sample.size()-1; ++i) //loop through each integer element in each sample
        {
            if(sample[i] > sample[i+1]) // if the the value at former posision is larger than that of the later position
            {
                std::vector<int> inversion; // inversion is a temporary
                inversion.push_back(sample[i]); //push back the first (larger) value
                inversion.push_back(sample[i+1]); // push back the second (smaller) value
                found = true; // since inversion is found, set found = true 
                output[it.key()]["ConsecutiveInversions"][std::to_string(i)] = inversion; 
            }
        }
        if(found) output[it.key()]["sample"] = sample; // create sample key to store values only if inversion is found 
    }

    for(auto it = output.begin(); it!=output.end(); ++it) // iterate through the key "sample__" in output json object, 
    {
        if(output[it.key()]["ConsecutiveInversions"].is_null()) output.erase(output.find(it.key())); // if there is no content, it means that there are no consecutive inversion in that sample,
        else ++sample_w_inversion; // thus erase that sample from the output json object.
    }
    output["metadata"]["arraySize"] = data["metadata"]["arraySize"];
    output["metadata"]["file"] = argv[1];
    output["metadata"]["numSamples"] = data["metadata"]["numSamples"];
    output["metadata"]["samplesWithInversions"] = sample_w_inversion;
    std::cout<< output.dump(2)<<std::endl;
}