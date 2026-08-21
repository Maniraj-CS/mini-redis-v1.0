#include "KeyValueStore.hpp"

KeyValueStore::KeyValueStore(size_t capacity , const std::string& filename) 
              : cache(capacity) , persistence(filename){
                persistence.load(*this);
              }

void KeyValueStore::set(const std::string &key, const std::string &value, int ttl){
    cache.put(key , value , ttl);
    persistence.logSet(key , value , ttl);
}

std::string KeyValueStore::get(const std::string &key){
     return cache.get(key);
}

void KeyValueStore::del(const std::string& key){
            cache.del(key);
            persistence.logDel(key);
}

bool KeyValueStore::exists(const std::string& key){
           return cache.exists(key);
}

void KeyValueStore::clear(){
    cache.clear();
}

void KeyValueStore::setInternal(const std::string &key , const std::string& value , int ttl){
       cache.put(key , value , ttl);
}

void KeyValueStore::delInternal(const std::string &key){
    cache.del(key);
}