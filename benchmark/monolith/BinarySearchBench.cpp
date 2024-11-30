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

  int target;

  for (auto _ : state) {
    target = rand() % size;
    binarySearch(data, target);
  }
}

BENCHMARK(BM_BinarySearch)->Iterations(10000000000);
BENCHMARK_MAIN();
