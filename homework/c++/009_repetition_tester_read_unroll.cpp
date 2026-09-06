#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <array>
#include <iostream>
#include <print>
#include <string_view>
#include <vector>

int main(int argc, char* argv[]) {
  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    std::cerr << "ERROR: Failed to detect CPU timer frequency\n";
    return 1;
  }

  u64 file_size = 1024 * 1024 * 1024;
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, file_size);

  std::vector<u8> buffer(static_cast<size_t>(file_size));

  struct asm_test_function {
    std::string_view name;
    void (*func)(u64, u8*);
  };

  const std::array<asm_test_function, 4> test_functions{{
      {"Read_x1", &Read_x1},
      {"Read_x2", &Read_x2},
      {"Read_x3", &Read_x3},
      {"Read_x4", &Read_x4},
  }};

  for (auto const& test_func : test_functions) {
    std::println("--- {} ---", test_func.name);
    repetition_tester tester{file_size, cpu_timer_freq};

    while (tester.is_testing()) {
      tester.begin();
      test_func.func(buffer.size(), buffer.data());
      tester.end();

      escape_buffer(buffer.data(), buffer.size());
      tester.add_bytes_count(buffer.size());
    }
  }
}
