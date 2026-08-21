#pragma once

#include <string>

class KeyValueStore;

class Persistence {
   private:
    std::string filename;

   public:
    Persistence(const std::string& filename);

    void logSet(const std::string& key, const std::string& value, int ttl);
    void logDel(const std::string& key);

    void load(KeyValueStore &storage);
};