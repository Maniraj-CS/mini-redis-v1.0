
#include <cassert>
#include <iostream>
#include <memory>

#include "ClientManager.hpp"

int main()
{
    std::cout << "------------------------------\n";
    std::cout << "     ClientManager Tests\n";
    std::cout << "------------------------------\n\n";

    ClientManager manager;


    // TEST 1: Add client
    std::cout << "TEST 1: Add client\n";

    manager.addClient(1);

    auto queue = manager.getQueue(1);

    assert(queue != nullptr);

    std::cout << "TEST 1 Passed\n\n";


    // TEST 2: Client's queue should work
    std::cout << "TEST 2: Client queue\n";

    queue->push("Hello Client 1");

    auto message = queue->pop();

    assert(message.has_value());
    assert(*message == "Hello Client 1");

    std::cout << "TEST 2 Passed\n\n";


    // TEST 3: Unknown client
    std::cout << "TEST 3: Unknown client\n";

    auto unknownQueue = manager.getQueue(99);

    assert(unknownQueue == nullptr);

    std::cout << "TEST 3 Passed\n\n";


    // TEST 4: Remove client
    std::cout << "TEST 4: Remove client\n";

    manager.removeClient(1);

    auto removedQueue = manager.getQueue(1);

    assert(removedQueue == nullptr);

    std::cout << "TEST 4 Passed\n\n";


    // TEST 5: shared_ptr lifetime
    std::cout << "TEST 5: shared_ptr lifetime\n";

    manager.addClient(2);

    auto clientQueue = manager.getQueue(2);

    assert(clientQueue != nullptr);

    clientQueue->push("Message after manager access");

    manager.removeClient(2);

    // ClientManager removed the client,
    // but our shared_ptr still keeps the queue alive.
    auto remainingMessage = clientQueue->pop();

    assert(remainingMessage.has_value());
    assert(*remainingMessage == "Message after manager access");

    std::cout << "TEST 5 Passed\n\n";


    std::cout << "------------------------------\n";
    std::cout << "    All Tests Passed!\n";
    std::cout << "------------------------------\n";

    return 0;
}

