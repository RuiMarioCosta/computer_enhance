#include <print>
#define _CRT_SECURE_NO_WARNINGS

#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <array>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    std::cerr << "ERROR: Failed to detect CPU timer frequency\n";
    return 1;
  }

  u64 file_size = 1024 * 1024 * 1024;
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, file_size);

  test_parameters params{.buffer =
                             std::vector<u8>(static_cast<size_t>(file_size))};

  struct asm_test_function {
    std::string_view name;
    void (*func)(u64, u8*);
  };

  const std::array<asm_test_function, 10> test_functions{{
      // {"ReuseBuffer + WriteToAllBytes", write_to_all_bytes_reuse_buffer},
      // {"MOVAllBytes", MOVAllBytes},
      // {"NOPAllBytes", NOPAllBytes},
      // {"CMPAllBytes", CMPAllBytes},
      // {"DECAllBytes", DECAllBytes},
      {"NOP3x1AllBytes", &NOP3x1AllBytes},
      {"NOP1x3AllBytes", &NOP1x3AllBytes},
      {"NOP1x9AllBytes", &NOP1x9AllBytes},
  }};

  auto const size = params.buffer.size();
  auto buffer = params.buffer.data();
  for (auto const& test_func : test_functions) {
    std::println("--- {} ---", test_func.name);
    repetition_tester tester{file_size, cpu_timer_freq};

    while (tester.is_testing()) {
      tester.begin();
      test_func.func(size, buffer);
      tester.end();

      escape_buffer(buffer, size);
      tester.add_bytes_count(size);
    }
  }
}
