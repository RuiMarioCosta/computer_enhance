#include "types.hpp"

#include <string>

struct test_params {
  std::string_view name;
  void (*func)(u64);
};
