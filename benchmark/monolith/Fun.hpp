#ifndef FUN_HPP
#define FUN_HPP

#include <benchmark/benchmark.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void searchFile(const fs::path& directory, const std::string& filename);
bool binarySearch(const std::vector<int>& arr, int target);

#endif
