include(FetchContent)

FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.9.0
)

set(BENCHMARK_ENABLE_GTEST_TESTS ON)

FetchContent_MakeAvailable(google_benchmark)
