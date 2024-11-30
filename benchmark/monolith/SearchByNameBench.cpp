#include <benchmark/benchmark.h>

#include <string>

#include "Fun.hpp"

static void BM_SearchFile(benchmark::State& state) {
  const fs::path directory = "/Users/zirtoshka/inf_sys";
  const std::string filename = "target_file.txt";

  std::cout << "Benchmarking search in directory: " << directory << std::endl;


  for (auto _ : state) {
      searchFile(directory, filename);
  }
}

BENCHMARK(BM_SearchFile)->Iterations(100);
BENCHMARK_MAIN();
