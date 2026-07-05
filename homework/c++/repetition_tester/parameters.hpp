#pragma once

#include "repetition_tester.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

enum class allocation_mode {
  reuse_buffer,
  allocate_per_sample,
};

char const* describe_allocation_mode(allocation_mode mode);

struct test_parameters {
  std::string file_name;
  std::vector<u8> buffer;
  allocation_mode mode{allocation_mode::reuse_buffer};

  test_parameters(std::string_view name, size_t buffer_size);
  std::vector<u8>& working_buffer(std::vector<u8>& scratch);
};
