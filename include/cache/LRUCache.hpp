#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <chrono>
#include <mutex>
#include "Value.hpp"

class LRUCache
{
private:
    struct Node
    {
        std::string key;
        Value value;
    };

    std::list<Node> lruList;
    std::unordered_map<std::string, std::list<Node>::iterator> cache;

    size_t capacity;

    std::mutex mtx; //for thread safety

public:
    LRUCache(size_t capacity);

    void put(const std::string &key, const std::string &value, int ttl);
    std::string get(const std::string &key);
    void del(const std::string &key);
    bool exists(const std::string &key);
    void clear();
};