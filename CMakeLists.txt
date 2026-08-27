cmake_minimum_required(VERSION 3.10)

project(mini_redis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mini_redis 
   src/main.cpp
   src/KeyValueStore.cpp
   src/LRUCache.cpp
   src/Persistence.cpp
)

target_include_directories(mini_redis PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(benchmark REQUIRED)

add_executable(lru_benchmark
    benchmarks/LRUCacheBenchmark.cpp
    src/LRUCache.cpp
)

target_include_directories(lru_benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(lru_benchmark
    benchmark::benchmark
)

# for stress  testing
add_executable(cache_stress_test
    tests/CacheStressTest.cpp
    src/KeyValueStore.cpp
    src/LRUCache.cpp
    src/Persistence.cpp
)

target_include_directories(cache_stress_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

#for Persistence testing
add_executable(persistence_test 
    tests/PersistenceTest.cpp
    src/Persistence.cpp
    src/LRUCache.cpp
    src/KeyValueStore.cpp
)

target_include_directories(persistence_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # for PubSub testing

# add_executable(pubsub_test
#     tests/PubSubTest.cpp
#     src/PubSub.cpp
# )

# target_include_directories(pubsub_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # for PubSubConcurrency testing

# add_executable(concurrency_pubsub_test
#     tests/PubSubConcurrencyTest.cpp
#     src/PubSub.cpp
# )

# target_include_directories(concurrency_pubsub_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # for MessageQueue testiong

# add_executable(message_queue 
#     src/MessageQueue.cpp
#     tests/MessageQueueTest.cpp
# )

# target_include_directories(message_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)


# # for MessageQueue Concurrency testiong

# add_executable(curr_message_queue 
#     src/MessageQueue.cpp
#     tests/MessageQueueConcurrencyTest.cpp
# )

# target_include_directories(curr_message_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)

# # for ClientManager testiong

# add_executable(client_manager_test
#     src/ClientManager.cpp
#     src/MessageQueue.cpp
#     tests/ClientManagerTest.cpp
# )

# target_include_directories(client_manager_test PRIVATE ${CMAKE_SOURCE_DIR}/include)


# # for ClientManager Concurrency testiong

# add_executable(client_manager_concurrency_test
#     src/ClientManager.cpp
#     src/MessageQueue.cpp
#     tests/ClientManagerConcurrencyTest.cpp
# )

# target_include_directories(client_manager_concurrency_test PRIVATE ${CMAKE_SOURCE_DIR}/include)


# for PubSub Integration testiong

add_executable(pubsub_integration_test
    tests/PubSubIntegrationTest.cpp
    src/PubSub.cpp
    src/ClientManager.cpp
    src/MessageQueue.cpp
)

target_include_directories(pubsub_integration_test PRIVATE ${CMAKE_SOURCE_DIR}/include)