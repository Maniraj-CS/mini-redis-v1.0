#pragma once

#include <string>
#include <chrono>

struct Value {
    std::string data;
    std::chrono::steady_clock::time_point expiry;
};