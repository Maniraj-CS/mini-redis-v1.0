
#include <cassert>
#include <iostream>

#include "ClientManager.hpp"
#include "PubSub.hpp"

int main()
{
    std::cout << "------------------------------\n";
    std::cout << "    Pub/Sub Integration Test\n";
    std::cout << "------------------------------\n\n";

    ClientManager manager;
    PubSub pubsub(manager);


    // TEST 1: Add clients
    std::cout << "TEST 1: Add clients\n";

    manager.addClient(1);
    manager.addClient(2);

    assert(manager.getQueue(1) != nullptr);
    assert(manager.getQueue(2) != nullptr);

    std::cout << "TEST 1 Passed\n\n";


    // TEST 2: Subscribe clients
    std::cout << "TEST 2: Subscribe clients\n";

    pubsub.subscribe("news", 1);
    pubsub.subscribe("news", 2);

    auto subscribers = pubsub.publish("news", "Hello");

    assert(subscribers.size() == 2);

    std::cout << "TEST 2 Passed\n\n";


    // TEST 3: Published message reaches subscribers
    std::cout << "TEST 3: Message delivery\n";

    auto queue1 = manager.getQueue(1);
    auto queue2 = manager.getQueue(2);

    auto message1 = queue1->pop();
    auto message2 = queue2->pop();

    assert(message1.has_value());
    assert(message2.has_value());

    assert(*message1 == "Hello");
    assert(*message2 == "Hello");

    std::cout << "TEST 3 Passed\n\n";


    // TEST 4: Unsubscribe client 1
    std::cout << "TEST 4: Unsubscribe client\n";

    pubsub.unsubscribe("news", 1);

    pubsub.publish("news", "Second Message");

    auto client1Message = queue1->pop();
    auto client2Message = queue2->pop();

    assert(!client1Message.has_value());
    assert(client2Message.has_value());

    assert(*client2Message == "Second Message");

    std::cout << "TEST 4 Passed\n\n";


    // TEST 5: Unknown channel
    std::cout << "TEST 5: Unknown channel\n";

    auto unknown = pubsub.publish("sports", "Football");

    assert(unknown.empty());

    std::cout << "TEST 5 Passed\n\n";


    std::cout << "------------------------------\n";
    std::cout << "    All Tests Passed!\n";
    std::cout << "------------------------------\n";

    return 0;
}
