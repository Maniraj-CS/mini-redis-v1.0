#include "pubsub/PubSub.hpp"

#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

int main()
{
    // PubSub pubsub;

    // const int threadCount = 10;

    // std::vector<std::thread> threads;

    // // Subscribe
    // for (int i = 0; i < threadCount; i++)
    // {
    //     threads.emplace_back([&pubsub, i]()
    //     {
    //         pubsub.subscribe("news", i);
    //     });
    // }

    // for (auto& thread : threads)
    // {
    //     thread.join();
    // }

    // threads.clear();

    // // Unsubscribe
    // for (int i = 0; i < threadCount; i++)
    // {
    //     threads.emplace_back([&pubsub, i]()
    //     {
    //         pubsub.unsubscribe("news", i);
    //     });
    // }

    // for (auto& thread : threads)
    // {
    //     thread.join();
    // }

    // std::cout << "Concurrent subscribe/unsubscribe test completed!\n";

    return 0;
}