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
   /      \
LRUCache  Persistence
   |          |
  RAM      appendonly.aof

```
## Current Features


- Key-value storage
- LRU Cache
- Configurable cache capacity
- TTL / key expiration
- SET
- GET
- DEL
- EXISTS
- CLEAR
- LRU eviction
- Thread-safe cache operations
- Concurrent stress testing
- ThreadSanitizer testing
- Google Benchmark setup
- AOF-style persistence
- Automatic startup recovery
- TTL-aware recovery
- Expired-key handling
- Malformed AOF handling
- Persistence testing
- Pub/Sub (channel-based messaging)
- SUBSCRIBE / UNSUBSCRIBE
- PUBLISH
- Per-client message queue (`MessageQueue`)
- Client lifecycle management (`ClientManager`)
- Thread-safe channel subscribe/unsubscribe/publish
- Pub/Sub integration testing
- Pub/Sub edge-case & concurrency stress testing


Project Structure
---
```bash
mini-redis/
├── include/
│   ├── cache/ 
│   └── pubsub/
│
├── src/
│   ├── cache/
│   └── pubsub/
│
├── tests/
│   ├── cache/
│   ├── persistence/
│   └── pubsub/
│
├── benchmarks/
├── CMakeLists.txt
└── README.md
```

Local build directories such as build/, build-tsan/, and build-release/ should not be committed to GitHub.

## Build

``` bash
cmake -S . -B build
cmake --build build
```


Run the main executable:

```
./build/mini_redis
```


## Testing

The current stress test uses multiple threads accessing the same KeyValueStore.

#### Each thread performs random:
```
SET
GET
DEL
EXISTS
```
operations.

## Current workload:

10 threads
×
100,000 operations
=
1,000,000 operations

The stress test completed successfully.

---
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


> Note: TSan has only been run against the cache stress test so far. The Pub/Sub concurrency test should also be run under TSan before Pub/Sub is considered fully verified.

## Pub/Sub

Mini Redis now supports basic channel-based messaging, made up of three components:

- **`MessageQueue`** — a thread-safe FIFO queue of pending messages for a single client.
- **`ClientManager`** — creates/removes clients and hands out each client's `MessageQueue`.
- **`PubSub`** — maps channel names to a set of subscribed client IDs, and delivers published messages to every subscriber's queue.

```
Publisher
    |
 PubSub (channel -> subscriber IDs)
    |
 ClientManager (clientId -> MessageQueue)
    |
 Subscriber's MessageQueue
```

Supported operations:

```
subscribe(channel, clientId)
unsubscribe(channel, clientId)
publish(channel, message)
```

All three classes use `std::mutex` internally, so subscribe/unsubscribe/publish are safe to call concurrently from multiple threads.

Run the Pub/Sub tests:

```bash
./build/pubsub_integration_test
./build/pubsub_edge_case_test
```

Pub/Sub is currently a standalone, in-process module — it is not yet wired into `main.cpp` or a network layer. That wiring happens in the Networking and Command Protocol phases below.

## Benchmark

The project contains Google Benchmark tests for the cache.

>Early benchmark results are not considered final because Google Benchmark itself was previously built in DEBUG mode.

>Final performance benchmarking will be performed after the major Redis features are implemented.

### Planned benchmarks:

``` bash
LRUCache Benchmark
KeyValueStore Benchmark
Concurrent Benchmark
```

## Roadmap

Phase 1 — Caching (completed)
---
```
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
```
Phase 2 — Persistence (completed)
---
```
Persistence
AOF-style append-only logging
SET/DEL persistence
Automatic startup recovery
TTL recovery
Expired-key handling
Malformed AOF handling
Persistence tests
```
Phase 3 — Pub/Sub (completed)
---
```
 Channels
 SUBSCRIBE
 UNSUBSCRIBE
 PUBLISH
 Subscriber management
 Thread-safe message delivery
```
Phase 4 — Replication (pending)
---
```
 Primary/Replica architecture
 Command propagation
 Initial synchronization
 Replica reconnect
 Replica state management
```
Phase 5 — Networking (pending)
---
```
 TCP server
 Client connections
 Multiple clients
 Request handling
 Response handling
```
Phase 6 — Redis-like Protocol (pending)
---
```
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
```
Eventually, the project may support RESP compatibility so existing Redis clients can communicate with Mini Redis.

Phase 7 — Final Testing (pending)
---
```
 Unit testing
 Integration testing
 Concurrent testing
 Stress testing
 Persistence testing
 Pub/Sub testing
 Replication testing
 Network testing
 Failure testing
```
Phase 8 — Performance (pending)
---
```
 Proper Release build
 Proper Google Benchmark Release build
 Single-thread benchmark
 Concurrent benchmark
 Performance profiling
 Find bottlenecks
 Optimize
 Benchmark again
 Compare before/after
```

### Final Goal

The final Mini Redis should be usable by another application over the network:

``` bash
                     Web Application / Backend
                              |
                         Redis Client
                              |
                          TCP Server
                              |
                       Command Parser
                              |
                      Mini Redis Server
                              |
            ┌─────────────────┼─────────────────┐
            |                 |                 |
        Data Layer        Messaging         Replication
            |                 |                 |
      KeyValueStore         Pub/Sub       Replica Servers
       /        \
      /          \
 LRUCache     Persistence
    |              |
   RAM        appendonly.aof

```
The main purpose of this project is to learn:
----
- C++
- Data structures
- Caching
- Concurrency
- Multithreading
- Persistence
- Networking
- Distributed systems
- Performance engineering