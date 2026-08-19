# Mini Redis

A Redis-like in-memory data store built from scratch in C++20.

The goal of this project is to understand how Redis-like systems work internally by building the important components ourselves: storage, caching, concurrency, persistence, Pub/Sub, replication, networking, and performance engineering.

## Current Architecture

```text
Application / Backend
        |
    Redis Client
        |
     TCP Server
        |
  Command Parser
        |
  KeyValueStore
        |
    LRUCache
        |
   Actual Storage



Current Features


Key-value storage
LRU Cache
Configurable cache capacity
TTL / key expiration
SET
GET
DEL
EXISTS
CLEAR
LRU eviction
Thread-safe cache operations
Concurrent stress testing
ThreadSanitizer testing
Google Benchmark setup


Project Structure

mini-redis/
├── benchmarks/
├── docs/
├── include/
├── src/
├── tests/
├── CMakeLists.txt
├── README.md
└── PROJECT.md


Local build directories such as build/, build-tsan/, and build-release/ should not be committed to GitHub.

## Build

``` bash
cmake -S . -B build
cmake --build build
```


Run the main executable:

./build/mini_redis



## Testing

The current stress test uses multiple threads accessing the same KeyValueStore.

Each thread performs random:

SET
GET
DEL
EXISTS

operations.

Current workload:

10 threads
×
100,000 operations
=
1,000,000 operations

The stress test completed successfully.

## ThreadSanitizer

A separate TSan build was used to check for data races.

```bash
cmake -S . -B build-tsan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"


cmake --build build-tsan


./build-tsan/cache_stress_test
```

The current test completed without a ThreadSanitizer data-race warning.

## Benchmark

The project contains Google Benchmark tests for the cache.

Early benchmark results are not considered final because Google Benchmark itself was previously built in DEBUG mode.

Final performance benchmarking will be performed after the major Redis features are implemented.

### Planned benchmarks:

``` bash
LRUCache Benchmark
KeyValueStore Benchmark
Concurrent Benchmark
```

## Roadmap

Phase 1 — Caching (completed)
 KeyValueStore
 LRU Cache
 TTL
 LRU eviction
 SET
 GET
 DEL
 EXISTS
 CLEAR
 Basic thread safety
 Concurrent stress testing
 ThreadSanitizer testing

Phase 2 — Persistence (pending)
 Persistence
 Save data to disk
 Load data on startup
 TTL recovery
 Recovery/error handling
Phase 3 — Pub/Sub
 Channels
 SUBSCRIBE
 UNSUBSCRIBE
 PUBLISH
 Subscriber management
 Thread-safe message delivery

Phase 4 — Replication (pending)
 Primary/Replica architecture
 Command propagation
 Initial synchronization
 Replica reconnect
 Replica state management

Phase 5 — Networking (pending)
 TCP server
 Client connections
 Multiple clients
 Request handling
 Response handling

Phase 6 — Redis-like Protocol (pending)
 Command parser
 SET
 GET
 DEL
 EXISTS
 EXPIRE
 TTL
 CLEAR
 SUBSCRIBE
 UNSUBSCRIBE
 PUBLISH

Eventually, the project may support RESP compatibility so existing Redis clients can communicate with Mini Redis.

Phase 7 — Final Testing (pending)
 Unit testing
 Integration testing
 Concurrent testing
 Stress testing
 Persistence testing
 Pub/Sub testing
 Replication testing
 Network testing
 Failure testing

Phase 8 — Performance (pending)
 Proper Release build
 Proper Google Benchmark Release build
 Single-thread benchmark
 Concurrent benchmark
 Performance profiling
 Find bottlenecks
 Optimize
 Benchmark again
 Compare before/after


### Final Goal

The final Mini Redis should be usable by another application over the network:

``` bash
Web Application / Backend
          |
      Redis Client
          |
      TCP Connection
          |
    +-------------+
    |  Mini Redis |
    |   Server    |
    +-------------+
          |
    Command Parser
          |
    KeyValueStore
          |
      LRUCache

```
The main purpose of this project is to learn:

C++
Data structures
Caching
Concurrency
Multithreading
Persistence
Networking
Distributed systems
Performance engineering