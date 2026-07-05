#include "repetition_tester.hpp"

#include "metrics.hpp"
#include <iostream>
#include <print>

Result::Result(u64 value) : E{} { E.fill(value); }

repetition_tester::repetition_tester(u64 target_byte_count,
                                     u64 cpu_timer_frequency,
                                     u32 seconds_to_try)
    : m_target_byte_count{target_byte_count},
      m_cpu_timer_frequency{cpu_timer_frequency},
      m_trial_duration_ticks{cpu_timer_frequency * seconds_to_try} {}

void repetition_tester::start() {
  m_mode = TestMode::testing;
  m_test_started_at = ReadCPUTimer();
}

void repetition_tester::begin() {
  ++m_open_block_count;

  m_result.E[ResultType::MemPageFaults] -= ReadOSPageFaultCount();
  m_result.E[ResultType::CPUTimer] -= ReadCPUTimer();
}

void repetition_tester::end() {
  m_result.E[ResultType::CPUTimer] += ReadCPUTimer();
  m_result.E[ResultType::MemPageFaults] += ReadOSPageFaultCount();

  ++m_close_block_count;
}

void repetition_tester::add_bytes_count(u64 bytes) {
  m_result.E[ResultType::ByteCount] += bytes;
}

bool repetition_tester::is_testing() {
  if (m_mode != TestMode::testing) {
    return false;
  }

  if (m_open_block_count > 0) {
    if (m_open_block_count != m_close_block_count) {
      m_mode = TestMode::error;
      std::println(std::cerr, "Unbalanced begin/end");
      return false;
    }
  }
  return true;
}
