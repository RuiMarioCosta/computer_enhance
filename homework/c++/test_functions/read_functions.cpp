#include "test_functions.hpp"

#include <cstdio>
#include <fstream>

status read_via_fread(repetition_tester& tester, test_parameters& params) {
  std::vector<u8> scratch;
  std::vector<u8>& buffer = params.working_buffer(scratch);

  auto scope = tester.time_scope();
  if (!scope) {
    return tester.last_status();
  }

  FILE* file = std::fopen(params.file_name.c_str(), "rb");
  if (!file) {
    return {status_code::invalid_operation, "failed to open input file"};
  }

  size_t read_count = std::fread(buffer.data(), 1, buffer.size(), file);
  std::fclose(file);
  if (read_count != buffer.size()) {
    return {status_code::invalid_operation, "fread did not read entire file"};
  }

  status count_status = tester.count_bytes(read_count);
  if (!count_status.is_success()) {
    return count_status;
  }

  scope.reset();

  return tester.finalize_sample();
}

status read_via_ifstream(repetition_tester& tester, test_parameters& params) {
  std::vector<u8> scratch;
  std::vector<u8>& buffer = params.working_buffer(scratch);

  auto scope = tester.time_scope();
  if (!scope) {
    return tester.last_status();
  }

  std::ifstream file{params.file_name, std::ios::binary};
  if (!file) {
    return {status_code::invalid_operation, "failed to open input file"};
  }

  file.read(reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(buffer.size()));
  if (file.gcount() != static_cast<std::streamsize>(buffer.size())) {
    return {status_code::invalid_operation,
            "ifstream read did not read entire file"};
  }

  status count_status = tester.count_bytes(buffer.size());
  if (!count_status.is_success()) {
    return count_status;
  }

  scope.reset();

  return tester.finalize_sample();
}
