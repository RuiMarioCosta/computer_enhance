#pragma once

#include "types.hpp"

#include <limits>
#include <string_view>

enum class TestMode {
  idle,
  testing,
  completed,
  error,
};

struct Result {
  u64 test_count;
  u64 cpu_timer;
  u64 mem_page_faults;
  u64 byte_count;

  Result() = default;
  explicit Result(u64 value);

  Result& operator+=(Result rhs);
};

struct TestResults {
  Result total{};
  Result min{std::numeric_limits<u64>::max()};
  Result max{};
};

class repetition_tester {
public:
  repetition_tester(u64 target_byte_count, u64 cpu_timer_frequency,
                    u32 seconds_to_try = 10);

  void begin();
  void end();
  void add_bytes_count(u64 bytes);
  [[nodiscard]] bool is_testing();

private:
  void _print_value(std::string_view label, Result result) const;

  u64 m_target_byte_count{};
  u64 m_cpu_timer_frequency{};
  u64 m_trial_duration_ticks{};
  // u32 m_warmup_sample_count{};
  u64 m_test_started_at{};

  u32 m_open_block_count{};
  u32 m_close_block_count{};

  Result m_result{};
  TestResults m_results{};
};
