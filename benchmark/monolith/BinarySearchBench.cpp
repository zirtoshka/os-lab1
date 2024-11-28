#include <benchmark/benchmark.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "Fun.hpp"

static void BM_BinarySearch(benchmark::State& state) {
  const int size = 100000000;
  std::vector<int> data(size);
  std::iota(data.begin(), data.end(), 0);

  const int target = size / 2;

  int repetitions = state.range(0);

  for (auto _ : state) {
    for (int i = 0; i < repetitions; ++i) {
      benchmark::DoNotOptimize(binarySearch(data, target));
    }
  }
}

BENCHMARK(BM_BinarySearch)->Arg(100000000);
BENCHMARK_MAIN();
