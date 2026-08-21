#pragma once

#include <string>
#include "LRUCache.hpp"
#include "Persistence.hpp"

class KeyValueStore
{

private:
    LRUCache cache;
    Persistence persistence;


public:
    KeyValueStore(size_t capacity, const std::string &filename);

    /*set value*/
    void set(const std::string &key, const std::string &value, int ttl);

    /*get value*/
    std::string get(const std::string &key);

    /*Delete value*/
    void del(const std::string &key);

    /*check if exists the value or not*/
    bool exists(const std::string &key);

    void clear();

    void setInternal(
        const std::string &key,
        const std::string &value,
        int ttl);

    void delInternal(const std::string &key);
};