
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

#include "pubsub/ClientManager.hpp"

int main()
{
    std::cout << "------------------------------\n";
    std::cout << " ClientManager Concurrency Test\n";
    std::cout << "------------------------------\n\n";

    ClientManager manager;

    const int threadCount = 10;
    const int clientsPerThread = 100;

    std::vector<std::thread> threads;


    // TEST 1: Concurrent addClient()
    std::cout << "TEST 1: Concurrent addClient()\n";

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&manager, i]()
        {
            for (int j = 0; j < clientsPerThread; j++)
            {
                int clientId = i * clientsPerThread + j;

                manager.addClient(clientId);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "TEST 1 Passed\n\n";


    // TEST 2: Concurrent getQueue()
    std::cout << "TEST 2: Concurrent getQueue()\n";

    threads.clear();

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&manager, i]()
        {
            for (int j = 0; j < clientsPerThread; j++)
            {
                int clientId = i * clientsPerThread + j;

                auto queue = manager.getQueue(clientId);

                assert(queue != nullptr);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "TEST 2 Passed\n\n";


    // TEST 3: Concurrent removeClient()
    std::cout << "TEST 3: Concurrent removeClient()\n";

    threads.clear();

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&manager, i]()
        {
            for (int j = 0; j < clientsPerThread; j++)
            {
                int clientId = i * clientsPerThread + j;

                manager.removeClient(clientId);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::cout << "TEST 3 Passed\n\n";


    // TEST 4: Verify all clients removed
    std::cout << "TEST 4: Verify removal\n";

    for (int i = 0; i < threadCount; i++)
    {
        for (int j = 0; j < clientsPerThread; j++)
        {
            int clientId = i * clientsPerThread + j;

            assert(manager.getQueue(clientId) == nullptr);
        }
    }

    std::cout << "TEST 4 Passed\n\n";


    std::cout << "------------------------------\n";
    std::cout << " All Concurrency Tests Passed!\n";
    std::cout << "------------------------------\n";

    return 0;
}

