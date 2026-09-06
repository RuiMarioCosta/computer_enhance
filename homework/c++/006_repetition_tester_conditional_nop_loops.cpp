#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <array>
#include <iostream>
#include <print>
#include <random>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class branch_pattern {
  NeverTaken,
  AlwaysTaken,
  Every2,
  Every3,
  Every4,
  CRTRandom,
  OSRandom,
};

std::unordered_map<branch_pattern, std::string_view> branch_name{
    {branch_pattern::NeverTaken, "NeverTaken"},
    {branch_pattern::AlwaysTaken, "AlwaysTaken"},
    {branch_pattern::Every2, "Every2"},
    {branch_pattern::Every3, "Every3"},
    {branch_pattern::Every4, "Every4"},
    {branch_pattern::CRTRandom, "CRTRandom"},
    {branch_pattern::OSRandom, "OSRandom"},
};

void fill_with_random_bytes(std::vector<u8>& buffer) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist{};

  for (auto& value : buffer) {
    value = dist(gen);
  }
}

void fill_with_branch_pattern(branch_pattern pattern, std::vector<u8>& buffer) {
  if (pattern == branch_pattern::OSRandom) {
    fill_with_random_bytes(buffer);
  } else {
    for (auto i = 0; i < buffer.size(); ++i) {
      u8 value = 0;

      switch (pattern) {
      case branch_pattern::NeverTaken: {
        value = 0;
      } break;
      case branch_pattern::AlwaysTaken: {
        value = 1;
      } break;
      case branch_pattern::Every2: {
        value = ((i % 2) == 0) ? 1 : 0;
      } break;
      case branch_pattern::Every3: {
        value = ((i % 3) == 0) ? 1 : 0;
      } break;
      case branch_pattern::Every4: {
        value = ((i % 4) == 0) ? 1 : 0;
      } break;
      case branch_pattern::CRTRandom: {
        value = rand();
      } break;
      default: {
        std::println("Unrecognized branch pattern.");
      } break;
      }
      buffer[i] = value;
    }
  }
}

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

  const std::array<asm_test_function, 1> test_functions{{
      {"ConditionalNOP", &ConditionalNOP},
  }};

  for (auto const& [pattern, pattern_name] : branch_name) {
    fill_with_branch_pattern(pattern, buffer);
    for (auto const& test_func : test_functions) {
      std::println("--- {}, {} ---", test_func.name, pattern_name);
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
}
