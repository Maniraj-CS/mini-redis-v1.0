#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>

#include "MessageQueue.hpp"

class ClientManager
{
private:
    std::unordered_map<int, std::shared_ptr<MessageQueue>> clientQueues;

    std::mutex mtx;

public:
    void addClient(int clientId);
    void removeClient(int clientId);

    std::shared_ptr<MessageQueue>  getQueue(int clientId);
};