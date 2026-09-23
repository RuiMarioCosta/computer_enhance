#define NOMINMAX
#include <Windows.h>

#include "buffer.hpp"
#include "csv_table.hpp"
#include "metrics.hpp"
#include "osread_sum.hpp"
#include "repetition_tester.hpp"

#include <array>
#include <filesystem>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>

#include <fileapi.h>

Buffer read_file(std::filesystem::path const& path) {
  std::ifstream ifs{path, std::ios::binary};
  Buffer buffer{std::filesystem::file_size(path)};
  if (!ifs.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
    throw std::runtime_error{"read failed"};
  }
  return buffer;
}

struct test_params {
  std::string_view name;
  std::move_only_function<u64(u64) const> func;
};

int main(int argc, char* argv[]) {
  std::filesystem::path main = argv[0];
  std::filesystem::path file = argv[1];
  SetProcessAffinity(0);
  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    throw std::runtime_error{"ERROR: Failed to detect CPU timer frequency\n"};
  }
  auto file_size = std::filesystem::file_size(file);
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, file_size);

  const std::array test_functions{
      test_params{"open_allocate_and_fread",
                  [&file](u64 size) {
                    return open_allocate_and_fread(size, file.string().c_str());
                  }},
      test_params{"open_allocate_and_sum",
                  [&file](u64 size) {
                    return open_allocate_and_sum(size, file.string().c_str());
                  }},
  };
  csv_table csv{"ReadBufferSize",
                {"open_allocate_and_fread", "open_allocate_and_sum"}};

  auto buffer_sizes =
      std::ranges::views::iota(18u) |
      std::ranges::views::transform([](auto i) { return 1u << i; }) |
      std::ranges::views::take_while(
          [file_size](auto value) { return value <= file_size; });

  auto buffer = read_file(file);
  auto reference_sum = sum_64s(file_size, buffer.data());

  for (auto const buffer_size : buffer_sizes) {
    std::vector<f64> bandwidths;
    bandwidths.reserve(test_functions.size());

    for (auto const& test_func : test_functions) {
      std::println("\n--- {}, {} kib ---", test_func.name, buffer_size / 1024);
      repetition_tester tester{file_size, cpu_timer_freq, 1};
      while (tester.is_testing()) {
        tester.begin();
        auto sum = test_func.func(buffer_size);
        tester.end();

        tester.add_bytes_count(file_size);

        if (sum != reference_sum) {
          std::println(std::cerr, "Checksum mismatch");
        }
      }

      bandwidths.push_back(tester.bandwidth());
    }

    csv.add_row(std::to_string(buffer_size / 1024) + "k", bandwidths);
  }

  csv.write_to_file(main.stem().string() + ".csv");
}
