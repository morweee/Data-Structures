#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) 
{
    std::ifstream file1(argv[1]); // takes the file by the name of first command line argument
    std::ifstream file2(argv[2]); // takes the file by the name of second command line argument
    json data1 = json::parse(file1); // parse the first file into data1 json object
    json data2 = json::parse(file2); // parse the second file into data2 json object
    json output; // output is the json object to store all final results 
    int conflict_count=0; //conflict_count is the number of mismatch between two data
    auto it2 = data2.begin(); //the iterator of second data, used in later comparisons
    for(auto it1 = data1.begin(); it1!=data1.end(); ++it1) //loop through all samples in the first data
    {
        if(it1.key()=="metadata") continue; //if loop to metadata key, break 
        output[it1.key()]; // first add the sample name as the key in output
        std::vector<int> sample1 = data1[it1.key()].get<std::vector<int>>(); // store the vectors of two data with same sample names into data1
        std::vector<int> sample2 = data2[it2.key()].get<std::vector<int>>(); // and data2;
        output[it1.key()][argv[1]] = sample1; // add a key of first command line arguement, which is the name of the first file
        output[it1.key()]["Mismatches"]; // Mismatch is a key to store later mismatching values in samples
        if(sample1.size() == sample2.size()) //make sure two vectors have same size
        {
            for(unsigned int i=0; i<sample1.size(); ++i) //loop through each integer element in each sample
            {
                if(sample1[i]!=sample2[i]) //if any mismatch is found
                {
                    std::vector<int> diff; //diff is the temporary vector that stores the mismatch of first and second sample
                    diff.push_back(sample1[i]); // record the first sample element
                    diff.push_back(sample2[i]); // record the second sample element
                    output[it1.key()]["Mismatches"][std::to_string(i)] = diff; //push the mismatch values to a vector and store it in output
                }
                
            }
        }
        output[it1.key()][argv[2]] = sample2; // add sample two to output
        if(output[it1.key()]["Mismatches"].is_null()) output.erase(output.find(it1.key())); //if there's no content in "Mismatches," that means the two vectors from different data are exactly same and thus remove them from the key "mismatch"
        else  ++conflict_count; // thus erase that sample from the output json object.
        ++it2; // increment the iterator of the second sample, moving onto the next comparison
    }
    //add various data in the "metadata" key
    output["metadata"]["File1"]["arraySize"] = data1["metadata"]["arraySize"];
    output["metadata"]["File1"]["name"] = argv[1];
    output["metadata"]["File1"]["numSamples"] = data1["metadata"]["numSamples"];
    output["metadata"]["File2"]["arraySize"] = data2["metadata"]["arraySize"];
    output["metadata"]["File2"]["name"] = argv[2];
    output["metadata"]["File2"]["numSamples"] = data2["metadata"]["numSamples"];
    output["metadata"]["samplesWithConflictingResults"] = conflict_count;
    std::cout<< output.dump(2)<<std::endl;
}