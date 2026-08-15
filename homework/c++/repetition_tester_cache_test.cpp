#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <array>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

struct test_params {
  std::string_view name;
  void (*func)(u64, u8*, u64);
};

int main(int argc, char* argv[]) {
  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    std::cerr << "ERROR: Failed to detect CPU timer frequency\n";
    return 1;
  }

  u64 file_size = 1024 * 1024 * 1024;
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, file_size);

  std::vector<u8> buffer(static_cast<size_t>(file_size));
  std::ranges::iota(buffer, 0);

  const std::array<test_params, 1> test_functions{{
      {"Read_32x8", &Read_32x8},
  }};

  auto masks =
      std::ranges::views::iota(10, 30) |
      std::ranges::views::transform([](auto i) { return (1 << i) - 1; });

  for (auto mask : masks) {
    for (auto const& test_func : test_functions) {
      std::println("--- {}, {} kib ---", test_func.name, mask);
      repetition_tester tester{file_size, cpu_timer_freq, 1};

      while (tester.is_testing()) {
        tester.begin();
        test_func.func(buffer.size(), buffer.data(), mask);
        tester.end();

        escape_buffer(buffer.data(), buffer.size());
        tester.add_bytes_count(buffer.size());
      }

      tester.save_to_file("cache_test.csv", mask);
    }
  }
}
