cmake_minimum_required(VERSION 3.10)

project(mini_redis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mini_redis 
   src/main.cpp
   src/KeyValueStore.cpp
   src/LRUCache.cpp
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