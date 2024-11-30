#include "Fun.hpp"

bool searchFile(const fs::path& directory, const std::string& filename) {
  try {
    if (!fs::exists(directory)) {
      return false;
    }

    if (!fs::is_directory(directory)) {
      return false;
    }
    for (auto it = fs::recursive_directory_iterator(
             directory, fs::directory_options::skip_permission_denied
         );
         it != fs::recursive_directory_iterator();
         ++it) {
      std::error_code ec;
      if (it->is_directory(ec) && ec) {
        it.disable_recursion_pending();
        continue;
      }

      if (it->is_fifo(ec) || it->is_other(ec) || it->is_socket(ec) || it->is_symlink(ec) ||
          it->is_character_file(ec) || ec) {
        continue;
      }

      try {
        if (fs::is_regular_file(*it, ec) && !ec && it->path().filename() == filename) {
          std::cout << "Found: " << it->path().string() << std::endl;
          return true;
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
  return false;
}

bool binarySearch(const std::vector<int>& arr, int target) {
  int left = 0;
  int right = arr.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
      return true;
    }
    if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return false;
}