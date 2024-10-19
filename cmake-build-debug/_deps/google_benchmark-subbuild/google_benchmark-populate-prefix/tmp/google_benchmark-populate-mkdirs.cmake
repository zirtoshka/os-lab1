# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-src"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-build"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/tmp"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src"
  "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/zirtoshka/os/макмакака/diy-shell-benchmark-24y-zirtoshka/cmake-build-debug/_deps/google_benchmark-subbuild/google_benchmark-populate-prefix/src/google_benchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
