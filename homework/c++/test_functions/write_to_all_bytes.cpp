#include "test_functions.hpp"

void write_to_all_bytes_reuse_buffer(repetition_tester& tester,
                                     test_parameters& params) {
  // reuse buffer
  auto buffer = params.buffer;

  while (tester.is_testing()) {
    tester.begin();
    for (size_t index = 0; index < buffer.size(); ++index) {
      buffer[index] = static_cast<u8>(index);
    }
    tester.end();

    tester.add_bytes_count(buffer.size());
  }
}

void write_to_all_bytes_new_buffer(repetition_tester& tester,
                                   test_parameters& params) {
  while (tester.is_testing()) {
    // create a new buffer for every loop
    std::vector<u8> buffer(params.buffer.size());

    tester.begin();
    for (size_t index = 0; index < buffer.size(); ++index) {
      buffer[index] = static_cast<u8>(index);
    }
    tester.end();

    tester.add_bytes_count(buffer.size());
  }
}
