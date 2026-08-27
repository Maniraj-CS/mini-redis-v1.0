

#include "pubsub/ClientManager.hpp"


void ClientManager::addClient(int clientId){
     std::lock_guard<std::mutex> lock(mtx);
     clientQueues[clientId] = std::make_shared<MessageQueue>();
}

void ClientManager::removeClient(int clientId){
    std::lock_guard<std::mutex> lock(mtx);
    clientQueues.erase(clientId);
}

std::shared_ptr<MessageQueue> ClientManager::getQueue(int clientId){
    std::lock_guard<std::mutex> lock(mtx);
    auto it = clientQueues.find(clientId);

    if(it != clientQueues.end()){
        return it->second;
    }
    else{
        return nullptr;
    }

}