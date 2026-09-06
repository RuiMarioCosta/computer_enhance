#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <numeric>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace test1 {

struct test_params {
  std::string_view name;
  void (*func)(u64, u8*, u64);
};

void test(u64 cpu_timer_freq, std::vector<u8>& buffer) {
  const std::vector<test_params> test_functions{{
      {"Read_32x8", &Read_32x8},
  }};

  auto masks =
      std::ranges::views::iota(10, 30) |
      std::ranges::views::transform([](auto i) { return (1 << i) - 1; });

  for (auto mask : masks) {
    for (auto const& test_func : test_functions) {
      std::println("--- {}, {} kib ---", test_func.name, mask);
      repetition_tester tester{buffer.size(), cpu_timer_freq, 2};

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

} // namespace test1

namespace test2 {

struct test_params {
  std::string_view name;
  void (*func)(u64, u8*, u64);
};

void test(u64 cpu_timer_freq, std::vector<u8>& buffer) {
  const std::vector<test_params> test_functions{{
      {"DoubleLoopRead_32x8", &DoubleLoopRead_32x8},
  }};

  auto number_of_samples = 32;
  auto inner_loop_size = 256; // 256 bytes due to the ymm0 reads
  auto start = 45 * 1024;
  auto end = 54 * 1024;
  auto diff = (end - start) / (number_of_samples - 1); // include end point
  auto inner_counts =
      std::ranges::views::iota(0) |
      std::ranges::views::take(number_of_samples) |
      std::ranges::views::transform([start, diff, inner_loop_size](auto i) {
        return (start + i * diff) / inner_loop_size;
      });

  for (auto const& test_func : test_functions) {
    for (auto const inner_count : inner_counts) {
      auto size = inner_count * inner_loop_size;
      auto outer_count = buffer.size() / size;
      std::println("\n--- {}, {} outer, {} inner, 256 bytes = {} kib ---",
                   test_func.name, outer_count, inner_count, size / 1024);
      repetition_tester tester{outer_count * size, cpu_timer_freq, 2};

      while (tester.is_testing()) {
        tester.begin();
        test_func.func(outer_count, buffer.data(), inner_count);
        tester.end();

        escape_buffer(buffer.data(), buffer.size());
        tester.add_bytes_count(outer_count * size);
      }

      tester.save_to_file("cache_test.csv", size);
    }
  }
}

} // namespace test2

int main(int argc, char* argv[]) {
  // P - core 0
  // E - core 2
  // LP - core 20
  SetProcessAffinity(0);

  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    throw std::runtime_error{"ERROR: Failed to detect CPU timer frequency\n"};
  }

  u64 file_size = 1024 * 1024 * 1024;
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, file_size);

  std::vector<u8> buffer(static_cast<size_t>(file_size));
  std::ranges::iota(buffer, 0);

  // test1::test(cpu_timer_freq, buffer);
  test2::test(cpu_timer_freq, buffer);
}
