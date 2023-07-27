#include <iostream>
#include <string> 
#include <fstream>
#include <vector>
#include <ctime>
#include "json.hpp"
#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
    clock_t start, finish; // start and finish to calculate how much time each algorithm spends
    std::ifstream file(argv[1]); // takes the file by the name of first command line argument
    std::ofstream output("output.csv"); //store the results into a csv 
    std::string title = "Sample,InsertionSortTime,InsertionSortCompares,InsertionSortMemaccess,MergeSortTime,MergeSortCompares,MergeSortMemaccess,QuickSortTime,QuickSortCompares,QuickSortMemaccess\n"; //add the first row of csv as title of each column
    output << title; // print the result to the csv file 
    std::cout << title; // and to the console
    json data = json::parse(file); // data is the json object where the file is parsed into  
    double insert_time, merge_time, quick_time; //insert_time is the time spend by insertion, and the respective algorithms as well
    int comparison, memory_accesses; // comparison and memory_accesses are to record the time of comparisons and memory access performed in each algorithm respectively
    for(auto it = data.begin(); it!=data.end(); ++it) //loop through the data
    {
        if(it.key()=="metadata") continue; //if loop to metadata key, break 
        output<<it.key()<<","; // print the sample name to the csv file 
        std::cout << it.key()<<","; // and to the console
        //for insertion sort
        comparison=0, memory_accesses=0; // initialize and reset the counters to 0
        std::vector<int> sample_i = data[it.key()].get<std::vector<int>>(); // sample_i is the integer vector from the sample's json object value
        start = clock(); //start timing
        InsertionSort(&sample_i, &comparison, &memory_accesses); // execute algorithms
        finish = clock(); //end timing
        insert_time = (finish-start)/CLOCKS_PER_SEC; //calculate the total time spent
        output << std::to_string(insert_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << ","; // print the memory access and comparison times to the csv file
        std::cout << std::to_string(insert_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << ","; // and to the console
        //for merge sort
        comparison=0, memory_accesses=0; // initialize and reset the counters to 0
        std::vector<int> sample_m = data[it.key()].get<std::vector<int>>(); // sample_i is the integer vector from the sample's json object value
        start = clock(); //start timing
        MergeSort(&sample_m, &comparison, &memory_accesses); // execute algorithms
        finish = clock(); //end timing
        merge_time = (finish-start)/CLOCKS_PER_SEC; //calculate the total time spent
        output << std::to_string(merge_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << ","; // print the memory access and comparison times to the csv file
        std::cout << std::to_string(merge_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << ","; // and to the console

        //for quick sort
        comparison=0, memory_accesses=0; // initialize and reset the counters to 0
        std::vector<int> sample_q = data[it.key()].get<std::vector<int>>(); // sample_i is the integer vector from the sample's json object value
        start = clock(); //start timing
        QuickSort(&sample_q, &comparison, &memory_accesses); // execute algorithms
        finish = clock(); //end timing
        quick_time = (finish-start)/CLOCKS_PER_SEC; //calculate the total time spent
        output << std::to_string(quick_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << "\n"; // print the memory access and comparison times to the csv file
        std::cout << std::to_string(quick_time) << "," << std::to_string(comparison) << "," << std::to_string(memory_accesses) << "\n"; // and to the console
    }
    return 0;
}