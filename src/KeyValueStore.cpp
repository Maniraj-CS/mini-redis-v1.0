#include "KeyValueStore.hpp"

KeyValueStore::KeyValueStore(size_t capacity) : cache(capacity){}

void KeyValueStore::set(const std::string &key, const std::string &value, int ttl){
    cache.put(key , value , ttl);
}

std::string KeyValueStore::get(const std::string &key){
     return cache.get(key);
}

void KeyValueStore::del(const std::string& key){
            cache.del(key);
}

bool KeyValueStore::exists(const std::string& key){
           return cache.exists(key);
}

void KeyValueStore::clear(){
    cache.clear();
}