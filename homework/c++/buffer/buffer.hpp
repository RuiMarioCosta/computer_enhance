#pragma once

#include "types.hpp"

#include <cstddef>

class Buffer {
  size_t m_size{0};
  u8* m_data{nullptr};

public:
  explicit Buffer(size_t size);
  Buffer(Buffer const&) = delete;
  Buffer& operator=(Buffer const&) = delete;
  Buffer(Buffer&& b) noexcept;
  Buffer& operator=(Buffer&& b) noexcept;
  ~Buffer();

  void swap(Buffer& b) noexcept;

  [[nodiscard]] size_t size() const noexcept;
  [[nodiscard]] u8* data() noexcept;
  [[nodiscard]] u8 const* data() const noexcept;

  u8& operator[](size_t i) noexcept;
  u8 const& operator[](size_t i) const noexcept;
};

struct BufferView {
  size_t size{0};
  u8* data{nullptr};
};

[[nodiscard]] BufferView AllocateBuffer(size_t size);
void FreeBuffer(BufferView* buffer) noexcept;
