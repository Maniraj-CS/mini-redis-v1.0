#pragma once

#include <string>
#include <mutex>

class KeyValueStore;

class Persistence {
   private:
    std::string filename;
    std::mutex mutex;

   public:
    Persistence(const std::string& filename);

    void logSet(const std::string& key, const std::string& value, int ttl);
    void logDel(const std::string& key);

    void load(KeyValueStore &storage);
    void logClear();
};