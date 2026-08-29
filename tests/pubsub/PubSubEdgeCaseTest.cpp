
#include <iostream>
#include <thread>
#include <vector>

#include "pubsub/PubSub.hpp"
#include "pubsub/ClientManager.hpp"

int main()
{
    std::cout << "----------------------------------------\n";
    std::cout << "      Pub/Sub Stress Test\n";
    std::cout << "----------------------------------------\n";

    ClientManager manager;
    PubSub pubsub(manager);

    const int clientCount = 100;
    const int threadCount = 10;

    // ----------------------------------------
    // Create clients
    // ----------------------------------------

    for (int clientId = 1; clientId <= clientCount; clientId++)
    {
        manager.addClient(clientId);
    }

    std::vector<std::thread> threads;

    // ----------------------------------------
    // TEST 1:
    // Concurrent subscribe
    // ----------------------------------------

    std::cout << "\nTEST 1: Concurrent Subscribe\n";

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&pubsub, i]()
        {
            int start = i * 10 + 1;
            int end = start + 9;

            for (int clientId = start; clientId <= end; clientId++)
            {
                pubsub.subscribe("news", clientId);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    threads.clear();

    std::cout << "TEST 1 Passed\n";


    // ----------------------------------------
    // TEST 2:
    // Concurrent Publish
    // ----------------------------------------

    std::cout << "\nTEST 2: Concurrent Publish\n";

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&pubsub, i]()
        {
            for (int j = 0; j < 100; j++)
            {
                pubsub.publish(
                    "news",
                    "Message from publisher " + std::to_string(i)
                );
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    threads.clear();

    std::cout << "TEST 2 Passed\n";


    // ----------------------------------------
    // TEST 3:
    // Concurrent Subscribe / Unsubscribe
    // ----------------------------------------

    std::cout << "\nTEST 3: Concurrent Subscribe / Unsubscribe\n";

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&pubsub, i]()
        {
            int clientId = i + 1;

            for (int j = 0; j < 1000; j++)
            {
                pubsub.subscribe("random", clientId);
                pubsub.unsubscribe("random", clientId);
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    threads.clear();

    std::cout << "TEST 3 Passed\n";


    // ----------------------------------------
    // TEST 4:
    // Concurrent Queue Access
    // ----------------------------------------

    std::cout << "\nTEST 4: Concurrent Queue Access\n";

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back([&manager, i]()
        {
            int clientId = i + 1;

            auto queue = manager.getQueue(clientId);

            if (!queue)
                return;

            for (int j = 0; j < 1000; j++)
            {
                queue->push("Direct queue message");

                if (!queue->empty())
                {
                    queue->pop();
                }
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    threads.clear();

    std::cout << "TEST 4 Passed\n";


    // ----------------------------------------
    // Final
    // ----------------------------------------

    std::cout << "\n----------------------------------------\n";
    std::cout << "All Pub/Sub Stress Tests Passed!\n";
    std::cout << "----------------------------------------\n";

    return 0;
}

/*

### Ye kya test kar raha hai?

```text
TEST 1
10 threads
   ↓
100 clients
   ↓
subscribe()
```

```text
TEST 2
10 threads
   ↓
100 publish() each
   ↓
MessageQueue
```

```text
TEST 3
10 threads
   ↓
subscribe ↔ unsubscribe
```

```text
TEST 4
10 threads
   ↓
MessageQueue push/pop
```

### ⚠️ Ek important baat

Is test mein **exact message count assert nahi kiya** hai.

Reason:

```text
T1 → publish()
T2 → unsubscribe()
T3 → subscribe()
```

in operations ka exact ordering concurrent execution mein fixed nahi hota.

Is test ka main objective:

> **Concurrent access ke under crash/data race/container corruption nahi hona chahiye.**

Normal build pass hone ke baad isi executable ko **TSan build** se run karna sabse important hai. 🔥
*/