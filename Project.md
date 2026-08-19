
---
```markdown
# Mini Redis — Project Context

This document contains the important architecture, decisions, progress, and roadmap for the Mini Redis project.

It should be updated whenever a major feature or architectural decision is completed.

---

# 1. Project Goal

Build a Redis-like server from scratch using C++20.

This is NOT intended to remain only a cache library.

The final goal is to create a real server that another backend or web application can connect to over the network, similar to how applications use Redis.

Target architecture:

```text
Web App / Backend
       |
   Redis Client
       |
    TCP Socket
       |
 Mini Redis Server
       |
 Command Parser
       |
 KeyValueStore
       |
   LRUCache
```

2. Technology

Current technology:

C++20
CMake
STL
unordered_map
list
thread
mutex
lock_guard
Google Benchmark
ThreadSanitizer
Linux / Ubuntu


3. Current Architecture

Important design decision:

KeyValueStore
      |
      v
   LRUCache
      |
      v
 ONE actual storage

KeyValueStore should NOT have another independent storage map.

KeyValueStore acts as the higher-level API.

LRUCache owns the actual cached data.

4. Value

The current Value structure contains:

data
expiry

TTL uses:

std::chrono::steady_clock::time_point

Expiration is checked when the key is accessed.

5. LRUCache

Current internal structure:

unordered_map<string, list<Node>::iterator>
+
list<Node>

The unordered_map provides fast key lookup.

The list maintains LRU order.

Front
  ↓
Most Recently Used
  |
  |
Least Recently Used
  ↓
Back

When the cache exceeds its capacity:

Back node
   ↓
Remove

When a key is accessed:

Key
 ↓
Move to front
6. KeyValueStore

Current API:

set()
get()
del()
exists()
clear()

Constructor:

KeyValueStore(size_t capacity);

KeyValueStore delegates operations to LRUCache.

Concept:

Application
     ↓
KeyValueStore
     ↓
LRUCache
     ↓
Actual storage
7. Thread Safety

Thread safety has been added to LRUCache.

A mutex protects cache operations:

std::lock_guard<std::mutex> lock(mtx);

Currently protected operations include:

put()
get()
del()
exists()
clear()

Important:

get() also needs the mutex because a successful GET modifies the LRU list.

For example:

GET key
   ↓
Find key
   ↓
Move node to front

So GET is not purely read-only.

8. Concurrent Stress Test

A mixed stress test has been implemented.

Current workload:

10 threads
×
100,000 operations per thread
=
1,000,000 operations

Each operation randomly chooses:

1 → SET
2 → GET
3 → DEL
4 → EXISTS

All threads access the SAME:

KeyValueStore

The test completed successfully.

9. ThreadSanitizer

A separate TSan build was created.

The mixed stress test was run using ThreadSanitizer.

Result:

No ThreadSanitizer data-race warning

This is good, but it does NOT prove the entire future system is race-free.

Every major concurrency change should be tested again.

10. Benchmark Status

Current benchmark:

benchmarks/LRUCacheBenchmark.cpp

It currently benchmarks direct LRUCache GET operations.

Previous approximate results:

~246 ms
~248.9 ms
~236 ms

However, Google Benchmark reported:

***WARNING*** Library was built as DEBUG.

Therefore these numbers are NOT final performance measurements.

Do not optimize based on these numbers.

The benchmark setup can remain for now.

Final benchmarking will happen after the major Redis features are implemented.

11. Planned Benchmarks

Eventually we want:

LRUCache Benchmark
       ↓
KeyValueStore Benchmark
       ↓
Concurrent Benchmark
       ↓
Full Server Benchmark

These should eventually be run using proper Release builds.

12. Completed Features
KeyValueStore + LRUCache integration     DONE


TTL / expiration                         DONE


LRU eviction                             DONE


SET                                      DONE


GET                                      DONE


DEL                                      DONE


EXISTS                                   DONE


CLEAR                                    DONE


Thread safety                            DONE


Basic concurrent test                    DONE


Mixed stress test                        DONE


ThreadSanitizer test                     DONE


Initial benchmark setup                  DONE


13. Remaining Roadmap
Phase A — Caching Foundation Cleanup

Before leaving the caching layer completely:

Review API behavior
Unit tests
Missing key tests
Expired key tests
Small capacity tests
Overwrite tests
TTL tests
LRU ordering tests
Delete tests
Clear tests


14. Phase B — Persistence

Next major feature.

Goal:

RAM
 |
 +---- Data
 |
 +---- Persistence file

When the server restarts:

Server stops
     ↓
Data stored on disk
     ↓
Server starts
     ↓
Data loaded

Need to decide:

File format
Serialization
When to write
When to load
TTL handling
Expired keys after restart
Corrupted file handling

Start with a simple implementation.

Do NOT try to reproduce Redis RDB/AOF perfectly at first.

15. Phase C — Pub/Sub

Implement:

SUBSCRIBE
UNSUBSCRIBE
PUBLISH

Basic architecture:

Publisher
    |
 Channel
    |
 Subscribers

Need:

Channel management
Subscriber management
Message delivery
Thread safety
Client lifecycle

Important:

Pub/Sub will eventually interact with the network layer.

16. Phase D — Replication

Basic model:

          Primary
         /       \
        /         \
 Replica 1       Replica 2

Need to implement:

Primary/replica relationship
Replication connection
Command propagation
Initial synchronization
Replica state
Reconnection

Start simple.

Do not try to build production-grade Redis replication immediately.

17. Phase E — TCP Server

This is one of the most important milestones.

Final goal:

Application
    |
 TCP connection
    |
Mini Redis Server

Need:

Socket creation
Bind
Listen
Accept
Client connection
Read
Write
Connection lifecycle
Multiple clients

Eventually multiple application clients should be able to communicate with Mini Redis simultaneously.

18. Phase F — Command Protocol

Start with simple commands:

SET key value
GET key
DEL key
EXISTS key

Then:

EXPIRE key seconds
TTL key
CLEAR

Then Pub/Sub:

SUBSCRIBE channel
UNSUBSCRIBE channel
PUBLISH channel message

Long-term goal:

Potential RESP compatibility.

If practical, existing Redis clients should be able to communicate with Mini Redis.

19. Final Architecture

Eventually:

                 Web Application
                        |
                   Backend/API
                        |
                  Redis Client
                        |
                     TCP
                        |
              +-------------------+
              |   Mini Redis      |
              |      Server       |
              +-------------------+
                        |
                 Command Parser
                        |
              +---------+---------+
              |                   |
        KeyValueStore          Pub/Sub
              |
          LRUCache
              |
        Actual Storage
              |
        +-----+------+
        |            |
   Persistence   Replication
20. Testing Strategy

Testing should happen in layers.

Unit Tests

Test individual components.

Value
LRUCache
KeyValueStore
Integration Tests

Test:

KeyValueStore
      ↓
LRUCache
Concurrent Tests

Multiple threads access the same storage.

Stress Tests

Large number of operations.

TSan

Detect possible data races.

Network Tests

Multiple clients communicate with the server.

Persistence Tests
Write
 ↓
Restart
 ↓
Read
Pub/Sub Tests
Publisher
 ↓
Channel
 ↓
Subscriber
Replication Tests
Primary
 ↓
Replica
21. Performance Strategy

Do NOT optimize too early.

Correct order:

Correctness
     ↓
Thread Safety
     ↓
Testing
     ↓
Complete Features
     ↓
Integration Testing
     ↓
Benchmark
     ↓
Profiling
     ↓
Find Bottleneck
     ↓
Optimize
     ↓
Benchmark Again

Never optimize based only on assumptions.

Use profiling to identify actual bottlenecks.

22. Development Rules
Rule 1 — Understand Before Coding

Before implementing a major feature, explain:

What are we building?
Why do we need it?
How does it work?
What problem does it solve?
What changes in our architecture?
How will we test it?

Keep explanations simple.

The developer should understand the concept instead of blindly copying code.

Rule 2 — One Feature at a Time

Do not implement five major components simultaneously.

Preferred workflow:

Explain
 ↓
Design
 ↓
Implement
 ↓
Build
 ↓
Test
 ↓
Understand result
 ↓
Next feature
Rule 3 — Keep Layers Separate

Target:

Network
   |
Protocol
   |
Server
   |
KeyValueStore
   |
LRUCache
   |
Storage

Do not mix networking logic into LRUCache.

Rule 4 — Preserve Important Architecture

Current important decision:

KeyValueStore
      ↓
LRUCache
      ↓
ONE actual storage

Do not accidentally create separate storage in KeyValueStore.

Rule 5 — Correctness Before Performance

First make it work correctly.

Then make it thread-safe.

Then test it.

Then benchmark.

Then profile.

Then optimize.

23. Current Next Step

Do NOT spend more time on final performance benchmarking yet.

The recommended order is:

Persistence
    ↓
Pub/Sub
    ↓
Replication
    ↓
TCP Server
    ↓
Command Protocol
    ↓
Full Integration Testing
    ↓
Final Stress Testing
    ↓
Proper Release Benchmark
    ↓
Concurrent Benchmark
    ↓
Profiling
    ↓
Optimization
24. Future Session Handoff

When continuing this project in a new conversation:

Read this PROJECT.md.
Inspect the current repository and code.
Check which roadmap phase is currently complete.
Do not assume old code is unchanged.
Explain the next feature before implementing it.
Preserve the current architecture unless there is a strong technical reason to change it.
Test meaningful changes.
Update this document after major features or architectural decisions.

The most important current context is:

Mini Redis is being built as a real Redis-like server,
not merely as a cache library.


Current foundation:


KeyValueStore
      ↓
LRUCache
      ↓
ONE actual storage


Thread safety:
DONE


Stress testing:
DONE


ThreadSanitizer:
DONE


Major features remaining:


Persistence
Pub/Sub
Replication
TCP Server
Command Protocol


Final benchmarking and profiling:
AFTER major features are complete