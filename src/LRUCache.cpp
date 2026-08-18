#include "LRUCache.hpp"

LRUCache::LRUCache(size_t capacity) : capacity(capacity) {}

void LRUCache::put(const std::string &key, const std::string &value, int ttl)
{   

    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);

    if (it != cache.end())
    {

        it->second->value.data = value;
        it->second->value.expiry = std::chrono::steady_clock::now() + std::chrono::seconds(ttl);

        lruList.splice(lruList.begin(), lruList, it->second);
    }
    else
    {   
        Value item;

        item.data = value;
        item.expiry = std::chrono::steady_clock::now() + std::chrono::seconds(ttl);

        Node newNode{
            key,
            item
        };

        lruList.push_front(newNode);

        cache[key] = lruList.begin();

        if (lruList.size() > capacity)
        {
            auto lru = lruList.back();

            cache.erase(lru.key);
            lruList.pop_back();
        }
    }
}

std::string LRUCache::get(const std::string &key)
{  
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);

    if (it == cache.end())
    {
        return "";
    }

    auto node = it->second;

    // Key expired
    if (std::chrono::steady_clock::now() >= node->value.expiry)
    {
        lruList.erase(node);
        cache.erase(it);
        return "";
    }
    
    // // Key is recently used → move to front
    lruList.splice(lruList.begin(), lruList, it->second);

    return it->second->value.data;
}


void LRUCache::del(const std::string& key){
    
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);

    if(it == cache.end()) return;

    lruList.erase(it->second);
    cache.erase(it);
}

bool LRUCache::exists(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mtx);

    auto it = cache.find(key);

    if (it == cache.end()) {
        return false;
    }

    auto node = it->second;

    // Expired key ko exist nahi maana jayega
    if (std::chrono::steady_clock::now() >= node->value.expiry) {
        lruList.erase(node);
        cache.erase(it);
        return false;
    }

    return true;
}

void LRUCache::clear(){
    std::lock_guard<std::mutex> lock(mtx);

    lruList.clear();
    cache.clear();
}