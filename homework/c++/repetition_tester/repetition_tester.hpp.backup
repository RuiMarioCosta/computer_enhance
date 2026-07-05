#pragma once

#include "types.hpp"

#include <optional>

enum class status_code {
  ok,
  wave_completed,
  not_testing,
  already_testing,
  invalid_config,
  invalid_sample,
  invalid_operation,
  no_result,
  tester_error,
};

struct status {
  status_code code{status_code::ok};
  char const* message{"ok"};

  [[nodiscard]] bool is_success() const {
    return code == status_code::ok || code == status_code::wave_completed;
  }
};

enum class validation_policy {
  hard_fail,
  soft_fail,
};

struct wave_config {
  u64 target_byte_count{};
  u64 cpu_timer_frequency{};
  u64 trial_duration_ticks{};
  validation_policy validation{validation_policy::hard_fail};
  bool track_page_faults{true};
  u32 warmup_sample_count{};

  wave_config() = default;
  wave_config(u64 target_byte_count, u64 cpu_timer_frequency,
              u32 seconds_to_try = 10, bool track_page_faults = true,
              u32 warmup_sample_count = 0)
      : target_byte_count{target_byte_count},
        cpu_timer_frequency{cpu_timer_frequency},
        trial_duration_ticks{cpu_timer_frequency *
                             static_cast<u64>(seconds_to_try)},
        track_page_faults{track_page_faults},
        warmup_sample_count{warmup_sample_count} {}
};

[[nodiscard]] u64 seconds_to_ticks(f64 seconds, u64 cpu_timer_frequency);

struct sample_counters {
  u64 cpu_timer_ticks{};
  u64 mem_page_faults{};
  u64 byte_count{};
};

struct averaged_counters {
  f64 cpu_timer_ticks{};
  f64 mem_page_faults{};
  f64 byte_count{};
};

struct result_snapshot {
  u64 cpu_timer_frequency{};
  u64 sample_count{};
  sample_counters total{};
  sample_counters min{};
  sample_counters max{};
  averaged_counters average{};
};

class repetition_tester {
public:
  class timing_scope {
  public:
    timing_scope() = default;
    timing_scope(repetition_tester* tester, bool active);
    timing_scope(timing_scope const&) = delete;
    timing_scope& operator=(timing_scope const&) = delete;
    timing_scope(timing_scope&& other) noexcept;
    timing_scope& operator=(timing_scope&& other) noexcept;
    ~timing_scope();

  private:
    repetition_tester* m_tester{};
    bool m_active{};
  };

  repetition_tester() = default;

  [[nodiscard]] status start_wave(wave_config const& config);
  [[nodiscard]] bool is_testing() const;
  [[nodiscard]] std::optional<timing_scope> time_scope();
  [[nodiscard]] status count_bytes(u64 byte_count);
  [[nodiscard]] status finalize_sample();
  [[nodiscard]] std::optional<result_snapshot> result() const;
  [[nodiscard]] status last_status() const;

private:
  friend class timing_scope;

  enum class mode {
    idle,
    testing,
    completed,
    error,
  };

  [[nodiscard]] status begin_timing_scope();
  void end_timing_scope();
  [[nodiscard]] status report_invalid_sample(char const* message);
  void reset_sample_state();
  void update_averages();

  mode m_mode{mode::idle};
  wave_config m_config{};
  status m_last_status{};

  u64 m_wave_started_at{};
  bool m_scope_active{};
  bool m_discard_current_sample{};
  u64 m_scope_start_cpu_ticks{};
  u64 m_scope_start_page_faults{};
  u32 m_completed_scope_count{};

  u64 m_valid_sample_count{};
  sample_counters m_current_sample{};
  result_snapshot m_result{};
};
