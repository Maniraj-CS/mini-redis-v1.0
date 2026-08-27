
#pragma once

#include <queue>
#include <mutex>
#include <string>
#include <optional>




class MessageQueue {

    private:

    std::queue<std::string> messages;

    std::mutex mtx;

    public:

    void push(const std::string &message);
    std::optional<std::string> pop();
    bool empty();
    size_t size();
};