#pragma once

#include "types.hpp"

#include <array>
#include <limits>

enum class TestMode {
  idle,
  testing,
  completed,
  error,
};

enum ResultType {
  TestCount,

  CPUTimer,
  MemPageFaults,
  ByteCount,

  Count
};

struct Result {
  Result() = default;
  explicit Result(u64 value);

  std::array<u64, ResultType::Count> E;
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

  void start();
  void begin();
  void end();
  void add_bytes_count(u64 bytes);
  [[nodiscard]] bool is_testing();

private:
  u64 m_target_byte_count{};
  u64 m_cpu_timer_frequency{};
  u64 m_trial_duration_ticks{};
  // u32 m_warmup_sample_count{};
  u64 m_test_started_at{};

  TestMode m_mode{TestMode::idle};
  u32 m_open_block_count{};
  u32 m_close_block_count{};

  Result m_result{};
  TestResults m_results{};
};
