#include "repetition_tester.hpp"

#include "metrics.hpp"

#include <limits>

u64 seconds_to_ticks(f64 seconds, u64 cpu_timer_frequency) {
  if (seconds <= 0.0 || cpu_timer_frequency == 0) {
    return 0;
  }

  f64 ticks = seconds * static_cast<f64>(cpu_timer_frequency);
  f64 max_ticks = static_cast<f64>(std::numeric_limits<u64>::max());
  if (ticks >= max_ticks) {
    return std::numeric_limits<u64>::max();
  }

  return static_cast<u64>(ticks);
}

repetition_tester::timing_scope::timing_scope(repetition_tester* tester,
                                              bool active)
    : m_tester{tester}, m_active{active} {}

repetition_tester::timing_scope::timing_scope(timing_scope&& other) noexcept
    : m_tester{other.m_tester}, m_active{other.m_active} {
  other.m_tester = nullptr;
  other.m_active = false;
}

repetition_tester::timing_scope&
repetition_tester::timing_scope::operator=(timing_scope&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (m_tester && m_active) {
    m_tester->end_timing_scope();
  }

  m_tester = other.m_tester;
  m_active = other.m_active;

  other.m_tester = nullptr;
  other.m_active = false;
  return *this;
}

repetition_tester::timing_scope::~timing_scope() {
  if (m_tester && m_active) {
    m_tester->end_timing_scope();
  }
}

status repetition_tester::start_wave(wave_config const& config) {
  if (m_mode == mode::testing) {
    m_last_status = {status_code::already_testing, "Wave is already running"};
    return m_last_status;
  }

  if (config.target_byte_count == 0) {
    m_last_status = {status_code::invalid_config,
                     "target_byte_count must be greater than zero"};
    return m_last_status;
  }

  if (config.cpu_timer_frequency == 0) {
    m_last_status = {status_code::invalid_config,
                     "cpu_timer_frequency must be greater than zero"};
    return m_last_status;
  }

  if (config.trial_duration_ticks == 0) {
    m_last_status = {status_code::invalid_config,
                     "trial_duration_ticks must be greater than zero"};
    return m_last_status;
  }

  m_config = config;
  m_mode = mode::testing;
  m_wave_started_at = ReadCPUTimer();
  m_valid_sample_count = 0;
  m_result = {};
  m_result.cpu_timer_frequency = config.cpu_timer_frequency;
  m_result.min.cpu_timer_ticks = std::numeric_limits<u64>::max();
  reset_sample_state();

  m_last_status = {status_code::ok, "ok"};
  return m_last_status;
}

bool repetition_tester::is_testing() const { return m_mode == mode::testing; }

std::optional<repetition_tester::timing_scope> repetition_tester::time_scope() {
  status scope_status = begin_timing_scope();
  if (!scope_status.is_success()) {
    return std::nullopt;
  }

  return timing_scope{this, true};
}

status repetition_tester::count_bytes(u64 byte_count) {
  if (m_mode != mode::testing) {
    m_last_status = {status_code::not_testing,
                     "count_bytes called while no wave is running"};
    return m_last_status;
  }

  if (!m_discard_current_sample) {
    m_current_sample.byte_count += byte_count;
  }

  m_last_status = {status_code::ok, "ok"};
  return m_last_status;
}

status repetition_tester::finalize_sample() {
  if (m_mode != mode::testing) {
    m_last_status = {status_code::not_testing,
                     "finalize_sample called while no wave is running"};
    return m_last_status;
  }

  if (m_scope_active) {
    return report_invalid_sample(
        "finalize_sample called while a timing scope is still active");
  }

  if (m_discard_current_sample) {
    reset_sample_state();
    m_last_status = {status_code::invalid_sample,
                     "sample discarded due to prior validation failure"};
    return m_last_status;
  }

  if (m_completed_scope_count == 0) {
    return report_invalid_sample("sample contains no timing scopes");
  }

  if (m_current_sample.byte_count != m_config.target_byte_count) {
    return report_invalid_sample("processed byte count does not match target");
  }

  bool is_warmup = m_valid_sample_count < m_config.warmup_sample_count;
  ++m_valid_sample_count;

  if (!is_warmup) {
    if (m_result.sample_count == 0) {
      m_result.min = m_current_sample;
      m_result.max = m_current_sample;
    } else {
      if (m_current_sample.cpu_timer_ticks < m_result.min.cpu_timer_ticks) {
        m_result.min = m_current_sample;
      }
      if (m_current_sample.cpu_timer_ticks > m_result.max.cpu_timer_ticks) {
        m_result.max = m_current_sample;
      }
    }

    m_result.total.cpu_timer_ticks += m_current_sample.cpu_timer_ticks;
    m_result.total.mem_page_faults += m_current_sample.mem_page_faults;
    m_result.total.byte_count += m_current_sample.byte_count;
    ++m_result.sample_count;

    if (m_current_sample.cpu_timer_ticks == m_result.min.cpu_timer_ticks) {
      // Course behavior: new minima reset the trial start window.
      m_wave_started_at = ReadCPUTimer();
    }
  }

  reset_sample_state();

  u64 now = ReadCPUTimer();
  if ((now - m_wave_started_at) > m_config.trial_duration_ticks) {
    m_mode = mode::completed;
    update_averages();
    m_last_status = {status_code::wave_completed, "wave completed"};
    return m_last_status;
  }

  m_last_status = {status_code::ok, "ok"};
  return m_last_status;
}

std::optional<result_snapshot> repetition_tester::result() const {
  if (m_mode == mode::completed) {
    return m_result;
  }

  return std::nullopt;
}

status repetition_tester::last_status() const { return m_last_status; }

status repetition_tester::begin_timing_scope() {
  if (m_mode != mode::testing) {
    m_last_status = {status_code::not_testing,
                     "time_scope called while no wave is running"};
    return m_last_status;
  }

  if (m_scope_active) {
    return report_invalid_sample("nested timing scopes are not allowed");
  }

  m_scope_active = true;
  m_scope_start_cpu_ticks = ReadCPUTimer();
  if (m_config.track_page_faults) {
    m_scope_start_page_faults = ReadOSPageFaultCount();
  }

  m_last_status = {status_code::ok, "ok"};
  return m_last_status;
}

void repetition_tester::end_timing_scope() {
  if (m_mode != mode::testing) {
    return;
  }

  if (!m_scope_active) {
    return;
  }

  u64 end_cpu_ticks = ReadCPUTimer();
  m_current_sample.cpu_timer_ticks += (end_cpu_ticks - m_scope_start_cpu_ticks);

  if (m_config.track_page_faults) {
    u64 end_page_faults = ReadOSPageFaultCount();
    m_current_sample.mem_page_faults +=
        (end_page_faults - m_scope_start_page_faults);
  }

  ++m_completed_scope_count;
  m_scope_active = false;
}

status repetition_tester::report_invalid_sample(char const* message) {
  m_discard_current_sample = true;

  if (m_config.validation == validation_policy::hard_fail) {
    m_mode = mode::error;
    m_last_status = {status_code::tester_error, message};
    return m_last_status;
  }

  m_last_status = {status_code::invalid_sample, message};
  return m_last_status;
}

void repetition_tester::reset_sample_state() {
  m_scope_active = false;
  m_discard_current_sample = false;
  m_scope_start_cpu_ticks = 0;
  m_scope_start_page_faults = 0;
  m_completed_scope_count = 0;
  m_current_sample = {};
}

void repetition_tester::update_averages() {
  if (m_result.sample_count == 0) {
    m_result.min = {};
    m_result.max = {};
    m_result.average = {};
    return;
  }

  f64 divisor = static_cast<f64>(m_result.sample_count);
  m_result.average.cpu_timer_ticks =
      static_cast<f64>(m_result.total.cpu_timer_ticks) / divisor;
  m_result.average.mem_page_faults =
      static_cast<f64>(m_result.total.mem_page_faults) / divisor;
  m_result.average.byte_count =
      static_cast<f64>(m_result.total.byte_count) / divisor;
}
