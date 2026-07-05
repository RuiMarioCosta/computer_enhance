#include "test_functions.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>

namespace {

void read_via_fread_impl(repetition_tester& tester, test_parameters& params,
                         std::vector<u8>& buffer) {
  FILE* file = std::fopen(params.file_name.c_str(), "rb");
  if (!file) {
    std::cerr << "ERROR: failed to open input file\n";
    return;
  }

  tester.begin();
  size_t read_count = std::fread(buffer.data(), 1, buffer.size(), file);
  tester.end();

  std::fclose(file);
  if (read_count != buffer.size()) {
    std::cerr << "ERROR: fread did not read entire file\n";
    return;
  }
  tester.add_bytes_count(read_count);
}

void read_via_ifstream_impl(repetition_tester& tester, test_parameters& params,
                            std::vector<u8>& buffer) {
  std::ifstream file{params.file_name, std::ios::binary};
  if (!file) {
    std::cerr << "ERROR: failed to open input file\n";
    return;
  }

  tester.begin();
  file.read(reinterpret_cast<char*>(buffer.data()),
            static_cast<std::streamsize>(buffer.size()));
  tester.end();

  if (file.gcount() != static_cast<std::streamsize>(buffer.size())) {
    std::cerr << "ERROR: ifstream read did not read entire file\n";
    return;
  }
  tester.add_bytes_count(buffer.size());
}

} // namespace

void read_via_fread_reuse_buffer(repetition_tester& tester,
                                 test_parameters& params) {
  read_via_fread_impl(tester, params, params.buffer);
}

void read_via_fread_new_buffer(repetition_tester& tester,
                               test_parameters& params) {
  std::vector<u8> buffer(params.buffer.size());
  read_via_fread_impl(tester, params, buffer);
}

void read_via_ifstream_reuse_buffer(repetition_tester& tester,
                                    test_parameters& params) {
  read_via_ifstream_impl(tester, params, params.buffer);
}

void read_via_ifstream_new_buffer(repetition_tester& tester,
                                  test_parameters& params) {
  std::vector<u8> buffer(params.buffer.size());
  read_via_ifstream_impl(tester, params, buffer);
}
