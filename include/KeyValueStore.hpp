#pragma once


#include <string>
#include "LRUCache.hpp"

class KeyValueStore{

    private:

     LRUCache cache;

    public:
       
       KeyValueStore(size_t capacity);

       /*set value*/
       void set(const std::string& key, const std::string& value , int ttl);
    
       /*get value*/
        std::string get(const std::string& key);
       
       /*Delete value*/
       void del(const std::string& key);

       /*check if exists the value or not*/
       bool exists(const std::string& key);

       void clear();
    
};