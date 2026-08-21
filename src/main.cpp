#include <iostream>
#include <thread>
#include <vector>
#include "KeyValueStore.hpp"

void worker(KeyValueStore& storage, int id)
{
    for (int i = 0; i < 1000; i++)
    {
        std::string key = "key" + std::to_string(id);

        storage.set(key, "value", 100);

        storage.get(key);

        storage.exists(key);
    }
}

int main()
{
    KeyValueStore storage(100,"appendonly.aof");

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++)
    {
        threads.emplace_back(worker, std::ref(storage), i);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "Concurrent test completed!" << std::endl;

    return 0;
}