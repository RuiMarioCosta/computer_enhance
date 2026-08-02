#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

#include <cstdlib>
#include <stdexcept>

void write_to_all_bytes_reuse_buffer(repetition_tester& tester,
                                     test_parameters& params) {
  auto size = params.buffer.size();
  // reuse buffer
  auto& buffer = params.buffer;

  while (tester.is_testing()) {
    tester.begin();
    for (size_t index = 0; index < size; ++index) {
      buffer[index] = static_cast<u8>(index);
    }
    tester.end();

    tester.add_bytes_count(size);
  }
}

void write_to_all_bytes_reuse_buffer_malloc(repetition_tester& tester,
                                            test_parameters& params) {
  auto size = params.buffer.size();
  // reuse buffer
  auto buffer = static_cast<u8*>(malloc(size));

  if (size > 0 && !buffer) {
    throw std::runtime_error{"Failed to allocate buffer"};
  }

  while (tester.is_testing()) {
    tester.begin();
    for (size_t index = 0; index < size; ++index) {
      buffer[index] = static_cast<u8>(index);
    }
    tester.end();

    escape_buffer(buffer, size);

    tester.add_bytes_count(size);
  }

  free(buffer);
}

void write_to_all_bytes_new_buffer(repetition_tester& tester,
                                   test_parameters& params) {
  auto size = params.buffer.size();
  while (tester.is_testing()) {
    // create a new buffer for every loop
    std::vector<u8> buffer;
    buffer.reserve(params.buffer.size());

    tester.begin();
    for (size_t index = 0; index < size; ++index) {
      buffer[index] = static_cast<u8>(index);
    }
    tester.end();

    tester.add_bytes_count(size);
  }
}
