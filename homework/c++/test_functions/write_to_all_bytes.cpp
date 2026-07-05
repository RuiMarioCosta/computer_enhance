#include "test_functions.hpp"

status write_to_all_bytes(repetition_tester& tester, test_parameters& params) {
  std::vector<u8> scratch;
  std::vector<u8>& buffer = params.working_buffer(scratch);

  auto scope = tester.time_scope();
  if (!scope) {
    return tester.last_status();
  }

  for (size_t index = 0; index < buffer.size(); ++index) {
    buffer[index] = static_cast<u8>(index);
  }

  status count_status = tester.count_bytes(buffer.size());
  if (!count_status.is_success()) {
    return count_status;
  }

  scope.reset();

  return tester.finalize_sample();
}
