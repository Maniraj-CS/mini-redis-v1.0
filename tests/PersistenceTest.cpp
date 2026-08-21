#include <cassert>
#include <cstdio>
#include <iostream>

#include "KeyValueStore.hpp"

int main()
{
    const std::string filename = "appendonly.aof";

    // Clean old test file
    std::remove(filename.c_str());

    // ==========================================
    // 1. SET -> Destroy -> Recover
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("name", "Maniraj", 100);
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("name") == "Maniraj");

        std::cout << "Test 1 passed: SET + Recovery\n";
    }

    // ==========================================
    // 2. Multiple SET
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("age", "20", 100);
        storage.set("city", "Kathmandu", 100);
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("age") == "20");
        assert(storage.get("city") == "Kathmandu");

        std::cout << "Test 2 passed: Multiple SET\n";
    }

    // ==========================================
    // 3. DEL Recovery
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.del("age");
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("age") == "");

        std::cout << "Test 3 passed: DEL Recovery\n";
    }

    // ==========================================
    // 4. No TTL Recovery
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("permanent", "hello", 0);
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("permanent") == "hello");

        std::cout << "Test 4 passed: No TTL Recovery\n";
    }

    // ==========================================
    // 5. TTL Recovery
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("temporary", "data", 5);
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("temporary") == "data");

        std::cout << "Test 5 passed: TTL Recovery\n";
    }

    // Clean test file
    std::remove(filename.c_str());

    std::cout << "\nAll Persistence tests passed! 🎉\n";

    return 0;
}