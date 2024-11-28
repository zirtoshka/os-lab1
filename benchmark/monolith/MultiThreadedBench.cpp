#include <benchmark/benchmark.h>

#include <filesystem>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "Fun.hpp"

namespace fs = std::filesystem;

void binarySearchTask(int repetitions) {
  const int size = 100000000;
  std::vector<int> data(size);
  std::iota(data.begin(), data.end(), 0);
  const int target = size / 2;

  for (int i = 0; i < repetitions; ++i) {
    benchmark::DoNotOptimize(binarySearch(data, target));
  }
}

void searchFileTask(const fs::path& directory, const std::string& filename, int iterations) {
  for (int i = 0; i < iterations; ++i) {
    searchFile(directory, filename);
  }
}

static void BM_MultiThreadedTask(benchmark::State& state) {
  int repetitions = state.range(0);
  fs::path directory = "/Users/zirtoshka/inf_sys";
  std::string filename = "target_file.txt";

  for (auto _ : state) {
    std::thread binarySearchThread(binarySearchTask, repetitions);
    std::thread fileSearchThread(searchFileTask, directory, filename, repetitions);

    binarySearchThread.join();
    fileSearchThread.join();
  }
}

// BENCHMARK(BM_MultiThreadedTask)->Arg(100000000)->Threads(1);  // Однопоточная
// BENCHMARK(BM_MultiThreadedTask)->Arg(100000000)->Threads(2);  // 2хпоточная
// BENCHMARK(BM_MultiThreadedTask)->Arg(100000000)->Threads(4);  // 4хпоточная

BENCHMARK(BM_MultiThreadedTask)->Arg(100)->Threads(4);  

BENCHMARK_MAIN();
