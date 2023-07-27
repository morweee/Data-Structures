// Merge Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

void MergeSort(std::vector<int>* numbers, int* comparison, int* memory_accesses);
void MergeSortRecurse(std::vector<int>* numbers, int i, int k, int* comparison, int* memory_accesses);
void Merge(std::vector<int>* numbers, int i, int j, int k, int* comparison, int* memory_accesses);