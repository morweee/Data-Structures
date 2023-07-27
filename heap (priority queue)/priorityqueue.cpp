#include <iostream>
#include <cmath>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) : //constructor
    nodes_(max_size + 1, KeyValuePair()),
    max_size_(max_size),
    size_(0) 
    {
    }

nlohmann::json PriorityQueue::JSON() const {
    nlohmann::json result;
  for (size_t i = 1; i <= size()-1; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size()-1) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size()-1) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["maxHeapSize"] = max_size_;
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size()-1;
    return result;
}

void PriorityQueue::insert(Key k) {
    insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
    if(isEmpty())
    {
        // initialize the vector to size of 1 (the first index doesn't contain anything)
        size_ = 1;
        nodes_.resize(size_);
    }
    nodes_.insert(nodes_.begin()+size(), kv); // keeps on insert at the end of the vector, thus size keeps growing 1 after each insert 
    heapifyUp(size()-1); // heapify from the last element
}

KeyValuePair PriorityQueue::min() {
    // return the root
    return nodes_[ROOT]; // return the key of the root, which should be the mininum in the min heap
}

KeyValuePair PriorityQueue::removeMin() {
    // return the root, then remove it and heapify array
    KeyValuePair min_pair = nodes_[ROOT]; // first record the kv pair that is going to be returned
    nodes_[ROOT] = nodes_[size()-1]; // replace the root(min) with the last element in the queue
    nodes_.pop_back(); //delete the last element
    heapifyDown(ROOT); // recursively heapify down to rearrange the queue
    return min_pair; // return the removed pair
}

bool PriorityQueue::isEmpty() const {
    // return true if the heap is empty
    return size_ == 0;
}

size_t PriorityQueue::size() const {
    // return the size of the heap
    return nodes_.size();
}

void PriorityQueue::heapifyUp(size_t i) {
    // a recursive function until the heap structure is valid
    size_t parent = std::floor(i/2); // every parent locates at floor(x/2)
    if (parent>=1 && getKey(parent) > getKey(i)) // check if the key of the node is greater than its parent
    {
        swap(nodes_[parent], nodes_[i]); // if so, swap the element of parent and child
        heapifyUp(parent); // the new parent is the origianl child, check again until the min heap is valid
    }
}

void PriorityQueue::heapifyDown(size_t i) {
    // a recursive function until the heap structure is valid
    size_t smallest = i; // parent location, 
    size_t left = 2*i; // left child
    size_t right = 2*i +1; // right child
    // compare parent with its left and right children, and get the smallest child to swap with
    if (left < size() && getKey(left) < getKey(i)) smallest = left;
    if (right < size() && getKey(right) < getKey(smallest)) smallest = right;
    if (smallest != i) // if any child is smaller than parent
    {
        swap(nodes_[smallest], nodes_[i]); // swap the smallest child with parent
        heapifyDown(smallest); // recursively call heapify down until min heap is valid
    }
}

void PriorityQueue::removeNode(size_t i) {
    nodes_[i] = nodes_[size()-1]; // replace the element with the last element in the queue
    nodes_.pop_back(); // delete the last element
    heapifyDown(i); // recursivelt call heapify down until the min heap is valid
}

Key PriorityQueue::getKey(size_t i) {
    // return the key of the ith node in the heap
    return nodes_.at(i).first;
}
