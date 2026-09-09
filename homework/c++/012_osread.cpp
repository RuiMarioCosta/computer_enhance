#define NOMINMAX
#include <Windows.h>

#include "buffer.hpp"
#include "file_reads.hpp"
#include "metrics.hpp"
#include "repetition_tester.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <print>
#include <ranges>
#include <stdexcept>
#include <string_view>

#include <fileapi.h>

struct test_params {
  std::string_view name;
  std::move_only_function<void(u64) const> func;
};

Buffer read_file(std::filesystem::path const& path) {
  std::ifstream ifs{path, std::ios::binary};
  Buffer buffer{std::filesystem::file_size(path)};
  if (!ifs.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
    throw std::runtime_error{"read failed"};
  }
  return buffer;
}

BufferView read_file2(std::filesystem::path const& path) {
  std::ifstream ifs{path, std::ios::binary};
  auto buffer = AllocateBuffer(std::filesystem::file_size(path));
  if (!ifs.read(reinterpret_cast<char*>(buffer.data), buffer.size)) {
    throw std::runtime_error{"read failed"};
  }
  return buffer;
}
int main(int argc, char* argv[]) {
  std::filesystem::path filename = argv[1];

  SetProcessAffinity(0);

  u64 cpu_timer_freq = GetCPUFreq();
  if (cpu_timer_freq == 0) {
    throw std::runtime_error{"ERROR: Failed to detect CPU timer frequency\n"};
  }

  Buffer buffer = read_file(filename);
  BufferView bufferView = read_file2(filename);
  std::println("CPU freq: {}, file size: {}", cpu_timer_freq, buffer.size());

  const std::array test_functions{
      test_params{"allocate_and_touch",
                  [](u64 size) { allocate_and_touch(size); }},
      test_params{"allocate_and_touch2",
                  [](u64 size) { allocate_and_touch2(size); }},
      test_params{"allocate_and_copy",
                  [&buffer](u64 size) { allocate_and_copy(size, buffer); }},
      test_params{
          "open_allocate_and_read",
          [&filename](u64 size) { open_allocate_and_read(size, filename); }},
      test_params{"open_allocate_and_fread",
                  [&filename](u64 size) {
                    open_allocate_and_fread(size, filename.string().c_str());
                  }},
  };

  auto buffer_sizes =
      std::ranges::views::iota(0) |
      std::ranges::views::transform([](auto i) { return 1 << i; }) |
      std::ranges::views::filter(
          [size = static_cast<int>(buffer.size())](auto i) {
            return 256 * 1024 <= i && i <= size;
          });

  for (auto const buffer_size : buffer_sizes) {
    for (auto const& test_func : test_functions) {
      std::println("\n--- {}, {} kib ---", test_func.name, buffer_size / 1024);
      repetition_tester tester{buffer.size(), cpu_timer_freq, 10};
      while (tester.is_testing()) {
        tester.begin();
        test_func.func(buffer_size);
        tester.end();

        tester.add_bytes_count(buffer.size());
      }

      // tester.save_to_file("cache_test.csv", file_size);
    }
  }
}
