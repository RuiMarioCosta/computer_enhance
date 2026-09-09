#include "buffer.hpp"

#include <stdexcept>

#include <Windows.h>

Buffer::Buffer(size_t size)
    : m_size{size},
      m_data{static_cast<u8*>(
          VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))} {
  if (!m_data) {
    throw std::runtime_error{"VirtualAlloc failed"};
  }
}

Buffer::Buffer(Buffer&& b) noexcept : m_size{b.m_size}, m_data{b.m_data} {
  b.m_data = nullptr;
};

Buffer& Buffer::operator=(Buffer&& b) noexcept {
  Buffer tmp{std::move(b)};
  swap(tmp);
  return *this;
};

Buffer::~Buffer() {
  if (m_data) {
    VirtualFree(m_data, 0, MEM_RELEASE);
  }
}

void Buffer::swap(Buffer& b) noexcept {
  using std::swap;
  swap(m_size, b.m_size);
  swap(m_data, b.m_data);
};

size_t Buffer::size() const noexcept { return m_size; }

u8* Buffer::data() noexcept { return m_data; }

u8 const* Buffer::data() const noexcept { return m_data; }

u8& Buffer::operator[](size_t i) noexcept { return m_data[i]; }
u8 const& Buffer::operator[](size_t i) const noexcept { return m_data[i]; }

BufferView AllocateBuffer(size_t size) {
  auto ptr = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (!ptr) {
    throw std::runtime_error{"VirtualAlloc failed"};
  }
  return BufferView{size, static_cast<u8*>(ptr)};
}

void FreeBuffer(BufferView* buffer) noexcept {
  if (buffer && buffer->data) {
    VirtualFree(buffer->data, 0, MEM_RELEASE);
    buffer->data = nullptr;
    buffer->size = 0;
  }
}
