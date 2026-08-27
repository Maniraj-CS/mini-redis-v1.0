
#include "pubsub/MessageQueue.hpp"


#include <iostream>
#include <vector>
#include <thread>
#include <cassert>

int main(){
     

    std::cout << "------------------------------\n"; 
    std::cout << " MessageQueue Concurrency Test\n"; 
    std::cout << "------------------------------\n\n";

    MessageQueue mq;


    const int threadCount = 10;
    const int messagePerThread = 100;


    std::vector<std::thread> threads;

    for(int i = 0 ; i < threadCount ; i++){
        threads.emplace_back([&mq , i](){
             for(int j = 0 ; j < messagePerThread ; j++){
                mq.push("Message from thread " + std::to_string(i));
             }
        });
    }


    for(auto &thread : threads) thread.join();

    assert(mq.size() == threadCount * messagePerThread);

    std::cout << "TEST 1 Passed\n\n";

    std::cout << "TEST 2: Concurrent pop\n";

    threads.clear();

    for (int i = 0; i < threadCount; i++) { 
        threads.emplace_back([&mq]() { 
            for (int j = 0; j < messagePerThread; j++) { 
                mq.pop(); 
            } 
        }); 
    }


    for(auto & thread: threads) thread.join();

    assert(mq.empty()); 
    assert(mq.size() == 0); 
    
    std::cout << "TEST 2 Passed\n\n"; 


    std::cout << "------------------------------\n"; 
    std::cout << " All Concurrency Tests Passed!\n"; 
    std::cout << "------------------------------\n";

    return 0;
}