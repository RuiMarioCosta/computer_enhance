#include "repetition_tester.hpp"
#include "metrics.hpp"

#include <iostream>
#include <print>

__declspec(noinline) void escape_buffer(void* p, size_t n) {
  (void)p;
  (void)n;
}

Result::Result(u64 value)
    : test_count{value}, cpu_timer{value}, mem_page_faults{value},
      byte_count{value} {}

Result& Result::operator+=(Result rhs) {
  test_count += rhs.test_count;
  cpu_timer += rhs.cpu_timer;
  mem_page_faults += rhs.mem_page_faults;
  byte_count += rhs.byte_count;
  return *this;
}

Result operator+(Result lhs, Result const& rhs) {
  lhs += rhs;
  return lhs;
}

repetition_tester::repetition_tester(u64 target_byte_count,
                                     u64 cpu_timer_frequency,
                                     u32 seconds_to_try)
    : m_target_byte_count{target_byte_count},
      m_cpu_timer_frequency{cpu_timer_frequency},
      m_trial_duration_ticks{cpu_timer_frequency * seconds_to_try},
      m_test_started_at{ReadCPUTimer()} {}

void repetition_tester::begin() {
  ++m_open_block_count;

  m_result.mem_page_faults -= ReadOSPageFaultCount();
  m_result.cpu_timer -= ReadCPUTimer();
}

void repetition_tester::end() {
  m_result.cpu_timer += ReadCPUTimer();
  m_result.mem_page_faults += ReadOSPageFaultCount();

  ++m_close_block_count;
}

void repetition_tester::add_bytes_count(u64 bytes) {
  m_result.byte_count += bytes;
}

bool repetition_tester::is_testing() {
  auto current_cpu_time = ReadCPUTimer();

  if (m_open_block_count > 0) {
    if (m_open_block_count != m_close_block_count) {
      std::println(std::cerr, "Unbalanced begin/end");
      return false;
    }

    if (m_result.byte_count != m_target_byte_count) {
      std::println(std::cerr, "Processed byte count mismatch");
      return false;
    }

    m_result.test_count = 1;
    m_results.total += m_result;

    if (m_result.cpu_timer > m_results.max.cpu_timer) {
      m_results.max = m_result;
    }

    if (m_result.cpu_timer < m_results.min.cpu_timer) {
      m_results.min = m_result;
      _print_value("Min", m_results.min);
      std::print("                                   \r");
    }

    m_result = Result{};
  }

  if (current_cpu_time > m_test_started_at + m_trial_duration_ticks) {
    _print_value("Min", m_results.min);
    std::print("\n");
    _print_value("Max", m_results.max);
    std::print("\n");
    _print_value("Avg", m_results.total);
    std::print("\n");
    return false;
  }
  return true;
}

void repetition_tester::_print_value(std::string_view label,
                                     Result result) const {
  f64 cpu_timer = static_cast<f64>(result.cpu_timer) / result.test_count;
  f64 mem_page_faults =
      static_cast<f64>(result.mem_page_faults) / result.test_count;
  f64 byte_count = static_cast<f64>(result.byte_count) / result.test_count;

  f64 seconds = cpu_timer / m_cpu_timer_frequency;
  std::print("{}: {} ({:.4f}ms)", label, cpu_timer, 1000. * seconds);

  if (byte_count > 0) {
    f64 gib = 1024. * 1024. * 1024.;
    auto bandwidth = byte_count / (gib * seconds);
    std::print(" {:.4f}GiB/s", bandwidth);
  }

  if (mem_page_faults > 0) {
    std::print(" PF: {:.4f} ({:.4f}k/fault)", mem_page_faults,
               byte_count / mem_page_faults);
  }
}
