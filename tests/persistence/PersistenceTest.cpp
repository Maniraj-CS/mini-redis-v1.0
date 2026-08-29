#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "cache/KeyValueStore.hpp"

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

    // ==========================================
    // 6. Expired Key -> Restart -> Should NOT Recover
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("expired", "data", 2);
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("expired") == "");

        std::cout << "Test 6 passed: Expired key not recovered\n";
    }

    // ==========================================
    // 7. Unknown Command
    // ==========================================

    // Create AOF manually
    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET valid hello 9999999999\n";
        file << "FOO something\n";
        file << "SET another world 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("valid") == "hello");
        assert(storage.get("another") == "world");
        assert(storage.get("something") == "");

        std::cout << "Test 7 passed: Unknown command skipped\n";
    }

    // ==========================================
    // 8. Malformed SET
    // ==========================================

    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET valid hello 9999999999\n";
        file << "SET broken\n";
        file << "SET another world 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("valid") == "hello");
        assert(storage.get("broken") == "");
        assert(storage.get("another") == "world");

        std::cout << "Test 8 passed: Malformed SET skipped\n";
    }

    // ==========================================
    // 9. Invalid Timestamp
    // ==========================================

    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET valid hello 9999999999\n";
        file << "SET broken Maniraj abc\n";
        file << "SET another world 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("valid") == "hello");
        assert(storage.get("broken") == "");
        assert(storage.get("another") == "world");

        std::cout << "Test 9 passed: Invalid timestamp skipped\n";
    }

    // ==========================================
    // 10.  Malformed DEL
    // ==========================================

    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET name Maniraj 9999999999\n";
        file << "DEL\n";
        file << "SET city Kathmandu 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("name") == "Maniraj");
        assert(storage.get("city") == "Kathmandu");

        std::cout << "Test 10 passed: Malformed DEL skipped\n";
    }

    // ==========================================
    // 11.  Empty & Garbage Lines
    // ==========================================

    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET name Maniraj 9999999999\n";
        file << "\n";
        file << "THIS IS GARBAGE\n";
        file << "\n";
        file << "SET city Kathmandu 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("name") == "Maniraj");
        assert(storage.get("city") == "Kathmandu");

        std::cout << "Test 11 passed: Empty and garbage lines skipped\n";
    }

    // ==========================================
    // 11.  Extra Arguments
    // ==========================================
    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET valid hello 9999999999\n";
        file << "SET broken value 9999999999 extra\n";
        file << "SET another world 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("valid") == "hello");
        assert(storage.get("broken") == "");
        assert(storage.get("another") == "world");

        std::cout << "Test 12 passed: Extra arguments skipped\n";
    }

    // ==========================================
    // 13. DEL Extra Arguments
    // ==========================================

    std::remove(filename.c_str());

    {
        std::ofstream file(filename);

        file << "SET name Maniraj 9999999999\n";
        file << "DEL name extra\n";
        file << "SET city Kathmandu 9999999999\n";
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("name") == "Maniraj");
        assert(storage.get("city") == "Kathmandu");

        std::cout << "Test 13 passed: DEL extra arguments skipped\n";
    }

    // ==========================================
    // 14. CLEAR Recovery
    // ==========================================

    {
        KeyValueStore storage(100, filename);

        storage.set("name", "Maniraj", 100);
        storage.set("age", "20", 100);

        storage.clear();
    }

    {
        KeyValueStore storage(100, filename);

        assert(storage.get("name") == "");
        assert(storage.get("age") == "");

        std::cout << "Test 14 passed: CLEAR Recovery\n";
    }

    // Clean test file
    std::remove(filename.c_str());

    std::cout << "\nAll Persistence tests passed! 🎉\n";

    return 0;
}