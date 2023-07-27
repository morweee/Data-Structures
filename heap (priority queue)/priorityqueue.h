#ifndef _PRIORITYQUEUE_H_
#define _PRIORITYQUEUE_H_

#include <vector>
#include <algorithm>
#include <utility>
#include "json.hpp"

typedef double                  Key;
typedef std::pair<int, int>     Value;
typedef std::pair<Key, Value>   KeyValuePair; 

class PriorityQueue {
  private:
    std::vector<KeyValuePair>   nodes_;
    size_t                      max_size_;
    size_t                      size_;
    const static size_t         ROOT = 1;
    
    void heapifyUp(size_t i);
    void heapifyDown(size_t i);
    void removeNode(size_t i);
    Key getKey(size_t i);

  public:
    PriorityQueue(std::size_t max_nodes);
    void insert(Key k);
    void insert(KeyValuePair kv);
    KeyValuePair min();
    KeyValuePair removeMin();
    bool isEmpty() const;
    size_t size() const;
    nlohmann::json JSON() const;
};  // class PriorityQueue

#endif  // _PRIORITYQUEUE_H_