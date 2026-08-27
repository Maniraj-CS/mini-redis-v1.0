
#include "MessageQueue.hpp"

void MessageQueue::push(const std::string &message)
{

    std::lock_guard<std::mutex> lock(mtx);

    messages.push(message);
}

std::optional<std::string> MessageQueue::pop()
{

    std::lock_guard<std::mutex> lock(mtx);

    if (messages.empty())
        return std::nullopt;

    std::string message = messages.front();

    messages.pop();

    return message;
}

bool MessageQueue::empty()
{

    std::lock_guard<std::mutex> lock(mtx);

    if (messages.empty())
        return true;
    else
        return false;
}

size_t MessageQueue::size()
{

    std::lock_guard<std::mutex> lock(mtx);

    return messages.size();
}