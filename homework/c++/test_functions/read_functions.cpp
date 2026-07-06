#include "test_functions.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>

void read_via_fread_reuse_buffer(repetition_tester& tester,
                                 test_parameters& params) {
  auto buffer = params.buffer;

  while (tester.is_testing()) {
#if _WIN32
    FILE* file = nullptr;
    if (fopen_s(&file, params.file_name.c_str(), "rb") != 0) {
      file = nullptr;
    }
#else
    FILE* file = std::fopen(params.file_name.c_str(), "rb");
#endif
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
}

void read_via_fread_new_buffer(repetition_tester& tester,
                               test_parameters& params) {
  while (tester.is_testing()) {
    auto const byte_count = params.buffer.size();
    auto buffer = static_cast<u8*>(malloc(byte_count));

    if (byte_count > 0 && !buffer) {
      std::cerr << "ERROR: failed to allocate read buffer\n";
      return;
    }

#if _WIN32
    FILE* file = nullptr;
    if (fopen_s(&file, params.file_name.c_str(), "rb") != 0) {
      file = nullptr;
    }
#else
    FILE* file = std::fopen(params.file_name.c_str(), "rb");
#endif
    if (!file) {
      std::cerr << "ERROR: failed to open input file\n";
      return;
    }

    tester.begin();
    size_t read_count = std::fread(buffer, 1, byte_count, file);
    tester.end();

    std::fclose(file);
    if (read_count != byte_count) {
      free(buffer);
      std::cerr << "ERROR: fread did not read entire file\n";
      return;
    }
    tester.add_bytes_count(read_count);
    free(buffer);
  }
}

void read_via_ifstream_reuse_buffer(repetition_tester& tester,
                                    test_parameters& params) {
  auto& buffer = params.buffer;

  while (tester.is_testing()) {
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
}

void read_via_ifstream_new_buffer(repetition_tester& tester,
                                  test_parameters& params) {
  while (tester.is_testing()) {
    auto const byte_count = params.buffer.size();
    auto buffer = static_cast<u8*>(malloc(byte_count));

    if (byte_count > 0 && !buffer) {
      std::cerr << "ERROR: failed to allocate read buffer\n";
      return;
    }

    std::ifstream file{params.file_name, std::ios::binary};
    if (!file) {
      free(buffer);
      std::cerr << "ERROR: failed to open input file\n";
      return;
    }

    tester.begin();
    file.read(reinterpret_cast<char*>(buffer),
              static_cast<std::streamsize>(byte_count));
    tester.end();

    if (file.gcount() != static_cast<std::streamsize>(byte_count)) {
      free(buffer);
      std::cerr << "ERROR: ifstream read did not read entire file\n";
      return;
    }
    tester.add_bytes_count(byte_count);
    free(buffer);
  }
}
