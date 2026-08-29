
#include "pubsub/PubSub.hpp"
#include <iostream>


PubSub::PubSub(ClientManager& manager) : clientManager(manager) {
    // Constructor implementation (if needed)
}

/*
 Here subscriber function is used to add a client to the channel.
  If the client is already present in the channel, it does nothing.
  Otherwise, it adds the client to the channel's subscriber list.
*/

void PubSub::subscribe(const std::string &channel, int clientId)
{

    std::lock_guard<std::mutex> lock(mtx);
    channels[channel].insert(clientId);
}

/*
    Here unsubscriber function is used to remove a client from the channel.
    If the client is not present in the channel, it does nothing.
    Otherwise, it removes the client from the channel's subscriber list.
*/

void PubSub::unsubscribe(const std::string &channel, int clientId)
{

    std::lock_guard<std::mutex> lock(mtx);

    auto it = channels.find(channel);

    if (it == channels.end())
        return;

    it->second.erase(clientId);
}


/*
    Here publisher function is used to send a message to all clients subscribed to a channel.
    If the channel has no subscribers, it does nothing.
    Otherwise, it sends the message to all clients in the channel's subscriber list.
*/

std::unordered_set<int> PubSub::publish(const std::string &channel, const std::string &message)
{

    std::unordered_set<int> subscribers;

    /*
    Lock the mutex to ensure thread safety while accessing the channels map.
    This prevents data races when multiple threads are subscribing, unsubscribing, 
    or publishing messages concurrently. 
    */
     
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = channels.find(channel);

        if (it == channels.end())
            return {};

        subscribers = it->second;
    }

    

     for(const auto &clientId : subscribers) {
        auto queue = clientManager.getQueue(clientId);
        if(queue) {
            queue->push(message);
        }
     }

     return subscribers;
}
