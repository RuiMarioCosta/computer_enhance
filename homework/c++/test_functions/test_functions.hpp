#pragma once

#include "repetition_tester.hpp"

#include <functional>
#include <string>
#include <string_view>

extern "C" {
void MOVAllBytesASM(u64 count, u8* data);
void NOPAllBytesASM(u64 count, u8* data);
void CMPAllBytesASM(u64 count, u8* data);
void DECAllBytesASM(u64 count, u8* data);

void NOP3x1AllBytes(u64 count, u8* data);
void NOP1x3AllBytes(u64 count, u8* data);
void NOP1x9AllBytes(u64 count, u8* data);

void ConditionalNOP(u64 count, u8* data);

void NOPAligned64(u64 count, u8* data);
void NOPAligned1(u64 count, u8* data);
void NOPAligned15(u64 count, u8* data);
void NOPAligned31(u64 count, u8* data);
void NOPAligned63(u64 count, u8* data);

void RATAdd(u64 count, u8* data);
void RATMovAdd(u64 count, u8* data);
}

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
void write_to_all_bytes_reuse_buffer_malloc(repetition_tester& tester,
                                            test_parameters& params);
void write_to_all_bytes_new_buffer(repetition_tester& tester,
                                   test_parameters& params);
void write_to_all_bytes_reuse_buffer_malloc_asm(repetition_tester& tester,
                                                test_parameters& params);

void read_via_fread_reuse_buffer(repetition_tester& tester,
                                 test_parameters& params);
void read_via_fread_new_buffer(repetition_tester& tester,
                               test_parameters& params);

void read_via_ifstream_reuse_buffer(repetition_tester& tester,
                                    test_parameters& params);
void read_via_ifstream_new_buffer(repetition_tester& tester,
                                  test_parameters& params);

void MOVAllBytes(repetition_tester& tester, test_parameters& params);
void NOPAllBytes(repetition_tester& tester, test_parameters& params);
void CMPAllBytes(repetition_tester& tester, test_parameters& params);
void DECAllBytes(repetition_tester& tester, test_parameters& params);
