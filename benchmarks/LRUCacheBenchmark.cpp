#include <benchmark/benchmark.h>
#include "LRUCache.hpp"

static void BM_Get(benchmark::State& state)
{
    LRUCache cache(1000000);

    for (int i = 0; i < 1000000; i++) {
        cache.put("key" + std::to_string(i), "value", 1000);
    }

    for (auto _ : state) {
        for (int i = 0; i < 1000000; i++) {
            benchmark::DoNotOptimize(
                cache.get("key" + std::to_string(i))
            );
        }
    }
}

BENCHMARK(BM_Get);

BENCHMARK_MAIN();