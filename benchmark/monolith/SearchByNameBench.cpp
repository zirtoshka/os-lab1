#include <benchmark/benchmark.h>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

void searchFile(const fs::path& directory, const std::string& filename) {
    try {
        if (!fs::exists(directory)) {
            std::cerr << "Directory does not exist: " << directory << std::endl;
            return;
        }

        if (!fs::is_directory(directory)) {
            std::cerr << "Path is not a directory: " << directory << std::endl;
            return;
        }

        for (auto it = fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied);
             it != fs::recursive_directory_iterator(); ++it) {
            std::error_code ec;

            if (it->is_directory(ec) && ec) {
                std::cerr << "Restricted directory: " << it->path() << " (" << ec.message() << ")" << std::endl;
                it.disable_recursion_pending(); 
                continue;
            }

            if (it->is_fifo(ec) || it->is_other(ec) || it->is_socket(ec) ||
                it->is_symlink(ec) || it->is_character_file(ec) || ec) {
                std::cerr << "Strange file: " << it->path() << std::endl;
                continue;
            }


            try {
                if (fs::is_regular_file(*it, ec) && !ec && it->path().filename() == filename) {
                    std::cout << "Found: " << it->path().string() << std::endl;
                    benchmark::DoNotOptimize(it->path().string()); 
                    return; 
                }
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error accessing file: " << e.what() << std::endl;
                continue; 
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing the directory: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    std::cout << "File not found! " << std::endl;
}

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

// Register the benchmark with a range of arguments
BENCHMARK(BM_SearchFile)->Arg(100000); 
BENCHMARK_MAIN();
