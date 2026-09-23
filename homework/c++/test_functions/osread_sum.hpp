#pragma once

#include "buffer.hpp"
#include "types.hpp"

#include <cstdio>
#include <filesystem>
#include <stdexcept>

inline u64 sum_64s(size_t data_size, void* data) {
  auto src = static_cast<u64*>(data);
  u64 sum0 = 0;
  u64 sum1 = 0;
  u64 sum2 = 0;
  u64 sum3 = 0;
  u64 sum_count = data_size / (4 * 8);
  while (sum_count--) {
    sum0 += src[0];
    sum1 += src[1];
    sum2 += src[2];
    sum3 += src[3];
    src += 4;
  }

  u64 result = sum0 + sum1 + sum2 + sum3;
  return result;
}

inline u64 open_allocate_and_fread(u64 buffer_size, char const* filepath) {
  u64 result = 0;
  auto file = fopen(filepath, "rb");
  Buffer buffer{buffer_size};
  if (file) {
    auto size_remaining = std::filesystem::file_size(filepath);
    while (size_remaining) {
      auto read_size = std::min<size_t>(buffer.size(), size_remaining);

      if (std::fread(buffer.data(), read_size, 1, file) != 1) {
        throw std::runtime_error{"fread failed"};
      }

      size_remaining -= read_size;
    }
  }
  std::fclose(file);
  return result;
}

inline u64 open_allocate_and_sum(u64 buffer_size, char const* filepath) {
  u64 result = 0;
  auto file = fopen(filepath, "rb");
  Buffer buffer{buffer_size};
  if (file) {
    auto size_remaining = std::filesystem::file_size(filepath);
    while (size_remaining) {
      auto read_size = std::min<size_t>(buffer.size(), size_remaining);

      if (std::fread(buffer.data(), read_size, 1, file) != 1) {
        throw std::runtime_error{"fread failed"};
      }

      result += sum_64s(read_size, buffer.data());

      size_remaining -= read_size;
    }
  }
  std::fclose(file);
  return result;
}

// TODO: implement the read with io ring
inline u64 ioring_allocate_and_fread(u64 buffer_size, char const* filepath) {
  u64 result = 0;
  return result;
}

// TODO: implement the read with one thread reading and another summing
inline u64 open_allocate_and_sum_overlapped(u64 buffer_size,
                                            char const* filepath) {
  u64 result = 0;
  return result;
}
