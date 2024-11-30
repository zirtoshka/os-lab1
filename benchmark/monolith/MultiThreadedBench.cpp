#include <benchmark/benchmark.h>

#include <filesystem>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "Fun.hpp"

namespace fs = std::filesystem;

void binarySearchTask() {
  const int size = 100000000;
  std::vector<int> data(size);
  std::iota(data.begin(), data.end(), 0);
  const int target = rand() % size;
  binarySearch(data, target);
}

void searchFileTask(const fs::path& directory, const std::string& filename) {
  searchFile(directory, filename);
}

static void BM_MultiThreadedTask(benchmark::State& state) {
  fs::path directory = "/Users/zirtoshka/inf_sys";
  std::string filename = "target_file.txt";

  for (auto _ : state) {
    std::thread binarySearchThread(binarySearchTask);
    std::thread fileSearchThread(searchFileTask, directory, filename);

    binarySearchThread.join();
    fileSearchThread.join();
  }
}

BENCHMARK(BM_MultiThreadedTask)->Iterations(100)->Threads(1);

BENCHMARK_MAIN();
