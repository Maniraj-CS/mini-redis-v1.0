#include "Persistence.hpp"
#include "KeyValueStore.hpp"

#include <fstream>
#include <sstream>

Persistence::Persistence(const std::string &filename) : filename(filename) {};

void Persistence::logSet(const std::string &key, const std::string &value, int ttl)
{

    std::ofstream file(filename, std::ios::app);
    if (!file)
        return;

    if (ttl <= 0)
    {
        file << "SET " << key << " " << value << " 0\n";
        return;
    }

    auto expiry = std::chrono::system_clock::now() + std::chrono::seconds(ttl);

    auto expiryTimestamp =
        std::chrono::duration_cast<std::chrono::seconds>(
            expiry.time_since_epoch())
            .count();

    file << "SET " << key << " " << value << " " << expiryTimestamp << "\n";
}

void Persistence::logDel(const std::string &key)
{
    std::ofstream file(filename, std::ios::app);

    file << "DEL " << key << "\n";
}

void Persistence::load(KeyValueStore &storage)
{

    std::ifstream file(filename);

    if (!file)
        return;

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string command;

        ss >> command;

        if (command == "SET")
        {

            std::string key;
            std::string value;
            long long expiryTimestamp;

            ss >> key >> value >> expiryTimestamp;

            if (expiryTimestamp == 0)
            {
                storage.setInternal(key, value, 0);
                continue;
            }

            auto now = std::chrono::system_clock::now();
            auto currentTimestamep =
                std::chrono::duration_cast<std::chrono::seconds>(
                    now.time_since_epoch())
                    .count();

            long long remaningTTL = expiryTimestamp - currentTimestamep;

            if (remaningTTL <= 0)
            {
                continue;
            }

            storage.setInternal(key, value, remaningTTL);
        }
        else if (command == "DEL")
        {

            std::string key;

            ss >> key;

            storage.delInternal(key);
        }
    }
}