#include <benchmark/benchmark.h>

#include <string>

#include "Fun.hpp"

static void BM_SearchFile(benchmark::State& state) {
  const fs::path directory = "/Users/zirtoshka/inf_sys";
  const std::string filename = "target_file.txt";

  std::cout << "Benchmarking search in directory: " << directory << std::endl;

  int iterations = state.range(0);

  for (auto _ : state) {
    for (int i = 0; i < iterations; i++) {
      searchFile(directory, filename);
    }
  }
}

// BENCHMARK(BM_SearchFile)->Arg(100000);
BENCHMARK(BM_SearchFile)->Arg(100);

BENCHMARK_MAIN();
