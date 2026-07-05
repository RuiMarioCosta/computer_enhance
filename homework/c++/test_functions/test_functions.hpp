#pragma once

#include "repetition_tester.hpp"

#include <functional>
#include <string>
#include <string_view>

struct test_parameters {
  std::string file_name;
  std::vector<u8> buffer;
};

struct test_function {
  std::string_view name;
  std::function<void(repetition_tester&, test_parameters&)> func;

  void operator()(repetition_tester& tester, test_parameters& params) const {
    func(tester, params);
  }
};

void write_to_all_bytes_reuse_buffer(repetition_tester& tester,
                                     test_parameters& params);
void write_to_all_bytes_new_buffer(repetition_tester& tester,
                                   test_parameters& params);

void read_via_fread_reuse_buffer(repetition_tester& tester,
                                 test_parameters& params);
void read_via_fread_new_buffer(repetition_tester& tester,
                               test_parameters& params);

void read_via_ifstream_reuse_buffer(repetition_tester& tester,
                                    test_parameters& params);
void read_via_ifstream_new_buffer(repetition_tester& tester,
                                  test_parameters& params);
