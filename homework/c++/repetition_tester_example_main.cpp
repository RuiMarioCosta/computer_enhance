#define _CRT_SECURE_NO_WARNINGS

#include "metrics.hpp"
#include "parameters.hpp"
#include "repetition_tester.hpp"
#include "test_functions/test_functions.hpp"

#include <array>
#include <filesystem>
#include <iostream>
#include <string>

namespace {

f64 to_seconds(f64 ticks, u64 freq) {
  if (freq == 0) {
    return 0.0;
  }

  return ticks / static_cast<f64>(freq);
}

void print_line(char const* label, f64 cpu_ticks, f64 page_faults,
                f64 byte_count, u64 cpu_freq) {
  f64 seconds = to_seconds(cpu_ticks, cpu_freq);
  f64 milliseconds = 1000.0 * seconds;
  std::cout << label << ": " << cpu_ticks << " (" << milliseconds << "ms)";

  if (seconds > 0.0 && byte_count > 0.0) {
    constexpr f64 gibibyte = 1024.0 * 1024.0 * 1024.0;
    f64 gb_per_second = byte_count / (gibibyte * seconds);
    std::cout << " " << gb_per_second << "gb/s";
  }

  if (page_faults > 0.0) {
    f64 kb_per_fault = byte_count / (page_faults * 1024.0);
    std::cout << " PF: " << page_faults << " (" << kb_per_fault << "k/fault)";
  }

  std::cout << '\n';
}

void print_results(result_snapshot const& snapshot) {
  print_line("Min", static_cast<f64>(snapshot.min.cpu_timer_ticks),
             static_cast<f64>(snapshot.min.mem_page_faults),
             static_cast<f64>(snapshot.min.byte_count),
             snapshot.cpu_timer_frequency);
  print_line("Max", static_cast<f64>(snapshot.max.cpu_timer_ticks),
             static_cast<f64>(snapshot.max.mem_page_faults),
             static_cast<f64>(snapshot.max.byte_count),
             snapshot.cpu_timer_frequency);
  print_line("Avg", snapshot.average.cpu_timer_ticks,
             snapshot.average.mem_page_faults, snapshot.average.byte_count,
             snapshot.cpu_timer_frequency);
  std::cout << "Samples: " << snapshot.sample_count << "\n";
}

struct test_function {
  std::string_view name;
  overhead_test_func func;
};

} // namespace

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

  test_parameters params{input_path.string(), static_cast<size_t>(file_size)};

  constexpr std::array<test_function, 3> test_functions{{
      {"WriteToAllBytes", write_to_all_bytes},
      {"fread", read_via_fread},
      {"ifstream", read_via_ifstream},
  }};
  constexpr std::array<allocation_mode, 2> allocation_modes{{
      allocation_mode::reuse_buffer,
      allocation_mode::allocate_per_sample,
  }};

  std::array<std::array<repetition_tester, allocation_modes.size()>,
             test_functions.size()>
      testers{};

  for (;;) {
    for (size_t func_index = 0; func_index < test_functions.size();
         ++func_index) {
      for (size_t mode_index = 0; mode_index < allocation_modes.size();
           ++mode_index) {
        params.mode = allocation_modes[mode_index];
        repetition_tester& tester = testers[func_index][mode_index];
        test_function const& test = test_functions[func_index];

        std::cout << "\n--- " << describe_allocation_mode(params.mode) << " + "
                  << test.name << " ---\n";

        wave_config config{file_size, cpu_timer_freq};

        status start = tester.start_wave(config);
        if (!start.is_success()) {
          std::cerr << "ERROR: " << start.message << "\n";
          return 1;
        }

        while (tester.is_testing()) {
          status sample_status = test.func(tester, params);
          if (!sample_status.is_success() &&
              sample_status.code != status_code::wave_completed) {
            std::cerr << "ERROR: " << sample_status.message << "\n";
            return 1;
          }
        }

        std::optional<result_snapshot> snapshot = tester.result();
        if (!snapshot) {
          std::cerr << "ERROR: wave completed without result\n";
          return 1;
        }

        print_results(*snapshot);
      }
    }
  }
}
