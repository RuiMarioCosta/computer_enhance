#include "metrics.hpp"
#include "repetition_tester.hpp"
#include "test_functions.hpp"

[[nodiscard]] inline auto run_asm(auto&& func) {
  return [func](repetition_tester& tester, test_parameters& params) {
    auto const size = params.buffer.size();
    auto buffer = params.buffer.data();

    while (tester.is_testing()) {
      tester.begin();
      func(size, buffer);
      tester.end();

      escape_buffer(buffer, size);
      tester.add_bytes_count(size);
    }
  };
}

void MOVAllBytes(repetition_tester& tester, test_parameters& params) {
  run_asm(MOVAllBytesASM)(tester, params);
}

void NOPAllBytes(repetition_tester& tester, test_parameters& params) {
  run_asm(NOPAllBytesASM)(tester, params);
}

void CMPAllBytes(repetition_tester& tester, test_parameters& params) {
  run_asm(CMPAllBytesASM)(tester, params);
}

void DECAllBytes(repetition_tester& tester, test_parameters& params) {
  run_asm(DECAllBytesASM)(tester, params);
}
