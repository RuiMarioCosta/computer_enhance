#include "parameters.hpp"

char const* describe_allocation_mode(allocation_mode mode) {
  switch (mode) {
  case allocation_mode::reuse_buffer:
    return "ReuseBuffer";
  case allocation_mode::allocate_per_sample:
    return "AllocatePerSample";
  }

  return "Unknown";
}

test_parameters::test_parameters(std::string_view name, size_t buffer_size)
    : file_name{name}, buffer(buffer_size),
      mode{allocation_mode::reuse_buffer} {}

std::vector<u8>& test_parameters::working_buffer(std::vector<u8>& scratch) {
  if (mode == allocation_mode::allocate_per_sample) {
    scratch.resize(buffer.size());
    return scratch;
  }

  return buffer;
}
