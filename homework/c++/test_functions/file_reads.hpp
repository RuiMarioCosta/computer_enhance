#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include "buffer.hpp"
#include "types.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <stdexcept>

inline void allocate_and_touch(u64 buffer_size) {
  constexpr int MIN_MEMORY_PAGE_SIZE = 4096;
  Buffer buffer{buffer_size};

  u64 touch_count =
      (buffer.size() + MIN_MEMORY_PAGE_SIZE - 1) / MIN_MEMORY_PAGE_SIZE;
  for (u64 index = 0; index < touch_count; ++index) {
    buffer[index * MIN_MEMORY_PAGE_SIZE] = 0;
  }
}

inline void allocate_and_touch2(u64 buffer_size) {
  constexpr int MIN_MEMORY_PAGE_SIZE = 4096;
  auto buffer = AllocateBuffer(buffer_size);

  u64 touch_count =
      (buffer.size + MIN_MEMORY_PAGE_SIZE - 1) / MIN_MEMORY_PAGE_SIZE;
  for (u64 index = 0; index < touch_count; ++index) {
    buffer.data[index * MIN_MEMORY_PAGE_SIZE] = 0;
  }

  FreeBuffer(&buffer);
}

/*
 * Will copy the total file size but won't read the actual file. Assumes that a
 * buffer already containing the file data, scratch, it's as if the file data
 * was already in a file cache that the OS might have. It loops over the entire
 * file size copying the file data from the source buffer, scratch, into our
 * chunking buffer.
 */
inline void allocate_and_copy(u64 buffer_size, Buffer const& scratch) {
  Buffer buffer{buffer_size};
  auto remaining_size = scratch.size();
  u8 const* src = scratch.data();

  while (remaining_size) {
    auto read_size = std::min<size_t>(buffer.size(), remaining_size);
#if 0
    __movsb(buffer.data, src, read_size);
#else
    memcpy(buffer.data(), src, read_size);
#endif
    src += read_size;
    remaining_size -= read_size;
  }
}

/*
 * Actually does the read with Windows' own direct file APIs and it reads in
 * chunks of buffer_size.
 */
inline void open_allocate_and_read(u64 buffer_size,
                                   std::filesystem::path const& file) {
  HANDLE File = CreateFileW(file.c_str(), GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  Buffer buffer{buffer_size};
  if (File != INVALID_HANDLE_VALUE) {
    auto size_remaining = std::filesystem::file_size(file);
    while (size_remaining) {
      auto read_size = std::min<size_t>(buffer.size(), size_remaining);

      DWORD bytes_read = 0;
      if (!ReadFile(File, buffer.data(), read_size, &bytes_read, NULL) &&
          bytes_read != read_size) {
        throw std::runtime_error{"ReadFile failed"};
      }

      size_remaining -= read_size;
    }
  }
  CloseHandle(File);
}

/*
 * The same as open_allocate_and_read but with the C runtime library version.
 */
inline void open_allocate_and_fread(u64 buffer_size, char const* filepath) {
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
}
