
#include <cassert>
#include <iostream>

#include "MessageQueue.hpp"

int main()
{
    std::cout << "------------------------------\n";
    std::cout << "       MessageQueue Tests\n";
    std::cout << "------------------------------\n\n";

    MessageQueue mq;

    // TEST 1: New queue should be empty
    std::cout << "TEST 1: Empty queue\n";

    assert(mq.empty());

    std::cout << "TEST 1 Passed\n\n";


    // TEST 2: Push and pop message
    std::cout << "TEST 2: Push and pop\n";

    mq.push("Hello World");

    auto message = mq.pop();

    assert(message.has_value());
    assert(*message == "Hello World");

    std::cout << "TEST 2 Passed\n\n";


    // TEST 3: Multiple messages and size
    std::cout << "TEST 3: Multiple messages\n";

    mq.push("Hello Boy");
    mq.push("Hello from Server");

    assert(mq.size() == 2);

    std::cout << "TEST 3 Passed\n\n";


    // TEST 4: FIFO order
    std::cout << "TEST 4: FIFO order\n";

    auto first = mq.pop();
    auto second = mq.pop();

    assert(first.has_value());
    assert(second.has_value());

    assert(*first == "Hello Boy");
    assert(*second == "Hello from Server");

    std::cout << "TEST 4 Passed\n\n";


    // TEST 5: Queue should be empty
    std::cout << "TEST 5: Empty after popping all messages\n";

    assert(mq.empty());
    assert(mq.size() == 0);

    std::cout << "TEST 5 Passed\n\n";


    // TEST 6: Pop from empty queue
    std::cout << "TEST 6: Pop from empty queue\n";

    auto emptyMessage = mq.pop();

    assert(!emptyMessage.has_value());

    std::cout << "TEST 6 Passed\n\n";


    std::cout << "------------------------------\n";
    std::cout << "    All Tests Passed!\n";
    std::cout << "------------------------------\n";

    return 0;
}

