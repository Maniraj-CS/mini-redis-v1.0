
#include "pubsub/PubSub.hpp"

#include <iostream>
#include <cassert>


int main(){
//    std::cout << "========== Pub/Sub Tests ==========\n\n"; 
   
//    PubSub pubsub; 

//    // -------------------------------------------------- 
//    // Test 1: Subscribe clients to a channel 
//    // -------------------------------------------------- 

//    pubsub.subscribe("news", 1); 
//    pubsub.subscribe("news", 2); 
//    auto subscribers = pubsub.publish("news", "Hello"); 
//    assert(subscribers.size() == 2); 
//    assert(subscribers.find(1) != subscribers.end()); 
//    assert(subscribers.find(2) != subscribers.end()); 
//    std::cout << "Test 1: Subscribe PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 2: Duplicate subscription 
//    // -------------------------------------------------- 

//    pubsub.subscribe("news", 1); 
//    subscribers = pubsub.publish("news", "Hello"); 

//    assert(subscribers.size() == 2); 
//    assert(subscribers.find(1) != subscribers.end()); 
//    assert(subscribers.find(2) != subscribers.end()); 

//    std::cout << "Test 2: Duplicate Subscribe PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 3: Unsubscribe 
//    // -------------------------------------------------- 
   
//    pubsub.unsubscribe("news", 1); 
//    subscribers = pubsub.publish("news", "Hello"); 

//    assert(subscribers.size() == 1); 
//    assert(subscribers.find(1) == subscribers.end());
//    assert(subscribers.find(2) != subscribers.end()); 

//    std::cout << "Test 3: Unsubscribe PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 4: Unsubscribe unknown client 
//    // -------------------------------------------------- 
   
//    pubsub.unsubscribe("news", 99); 
//    subscribers = pubsub.publish("news", "Hello"); 

//    assert(subscribers.size() == 1); 
//    assert(subscribers.find(2) != subscribers.end()); 

//    std::cout << "Test 4: Unknown Client Unsubscribe PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 5: Unknown channel 
//    // -------------------------------------------------- 
   
//    auto unknownChannel = pubsub.publish("sports", "Football"); 

//    assert(unknownChannel.empty()); 
   
//    std::cout << "Test 5: Unknown Channel PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 6: Remove remaining subscriber 
//    // -------------------------------------------------- 
   
//    pubsub.unsubscribe("news", 2); 
//    subscribers = pubsub.publish("news", "Hello"); 

//    assert(subscribers.empty()); 

//    std::cout << "Test 6: Empty Channel PASSED\n"; 
   
//    // -------------------------------------------------- 
//    // Test 7: Multiple channels 
//    // -------------------------------------------------- 
//    pubsub.subscribe("tech", 10); 
//    pubsub.subscribe("tech", 11); 
//    pubsub.subscribe("sports", 20); 

//    auto techSubscribers = pubsub.publish("tech", "New Tech"); 
//    auto sportsSubscribers = pubsub.publish("sports", "Football"); 


//    assert(techSubscribers.size() == 2); 
//    assert(techSubscribers.find(10) != techSubscribers.end()); 
//    assert(techSubscribers.find(11) != techSubscribers.end()); 
   
//    assert(sportsSubscribers.size() == 1); 
//    assert(sportsSubscribers.find(20) != sportsSubscribers.end()); 


//    std::cout << "Test 7: Multiple Channels PASSED\n"; 
//    std::cout << "\n========== All Tests Passed ==========\n"; 
   return 0;
}