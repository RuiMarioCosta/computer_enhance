#include "test_functions.hpp"

#include <cstdlib>

extern "C" {
void MOVAllBytesASM(u64 count, u8* data);
void NOPAllBytesASM(u64 count, u8* data);
void CMPAllBytesASM(u64 count, u8* data);
void DECAllBytesASM(u64 count, u8* data);
}

void MOVAllBytes(repetition_tester& tester, test_parameters& params) {
  auto const size = params.buffer.size();
  auto buffer = params.buffer.data();

  while (tester.is_testing()) {
    tester.begin();
    MOVAllBytesASM(size, buffer);
    tester.end();

    escape_buffer(buffer, size);
    tester.add_bytes_count(size);
  }
}

void NOPAllBytes(repetition_tester& tester, test_parameters& params) {
  auto const size = params.buffer.size();
  auto buffer = params.buffer.data();

  while (tester.is_testing()) {
    tester.begin();
    NOPAllBytesASM(size, buffer);
    tester.end();

    escape_buffer(buffer, size);
    tester.add_bytes_count(size);
  }
}

void CMPAllBytes(repetition_tester& tester, test_parameters& params) {
  auto const size = params.buffer.size();
  auto buffer = params.buffer.data();

  while (tester.is_testing()) {
    tester.begin();
    NOPAllBytesASM(size, buffer);
    tester.end();

    escape_buffer(buffer, size);
    tester.add_bytes_count(size);
  }
}

void DECAllBytes(repetition_tester& tester, test_parameters& params) {
  auto const size = params.buffer.size();
  auto buffer = params.buffer.data();

  while (tester.is_testing()) {
    tester.begin();
    NOPAllBytesASM(size, buffer);
    tester.end();

    escape_buffer(buffer, size);
    tester.add_bytes_count(size);
  }
}

