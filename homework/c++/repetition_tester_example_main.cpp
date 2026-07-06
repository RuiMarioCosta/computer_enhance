#include <print>
#define _CRT_SECURE_NO_WARNINGS

#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <array>
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " [existing filename]\n";
    return 1;
  }

  std::filesystem::path input_path{argv[1]};
  u64 file_size = std::filesystem::file_size(input_path);
  if (file_size == 0) {
    std::cerr << "ERROR: Test data size must be non-zero and file must exist\n";
    return 1;
  }

  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    std::cerr << "ERROR: Failed to detect CPU timer frequency\n";
    return 1;
  }

  test_parameters params{input_path.string(),
                         std::vector<u8>(static_cast<size_t>(file_size))};

  const std::array<test_function, 4> test_functions{{
      // {"Malloc ReuseBuffer + WriteToAllBytes",
      //  write_to_all_bytes_reuse_buffer_malloc},
      // {"ReuseBuffer + WriteToAllBytes", write_to_all_bytes_reuse_buffer},
      // {"NewBuffer + WriteToAllBytes", write_to_all_bytes_new_buffer},
      {"ReuseBuffer + fread", read_via_fread_reuse_buffer},
      {"NewBuffer + fread", read_via_fread_new_buffer},
      {"ReuseBuffer + ifstream", read_via_ifstream_reuse_buffer},
      {"NewBuffer + ifstream", read_via_ifstream_new_buffer},
  }};

  for (auto const& func : test_functions) {
    std::println("--- {} ---", func.name);
    repetition_tester tester{file_size, cpu_timer_freq};
    func(tester, params);
  }
}
