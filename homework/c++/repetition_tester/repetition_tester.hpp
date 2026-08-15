#pragma once

#include "types.hpp"

#include <filesystem>
#include <fstream>
#include <limits>
#include <string_view>

/**
 * @brief Marks a buffer as externally observable after benchmark work.
 *
 * This function is used as an optimization barrier in tight benchmark loops:
 * by passing the buffer pointer and size to a non-inlined call site, we make
 * it harder for the compiler to prove that writes are dead and remove them.
 *
 * @param p Pointer to the start of the buffer that was read/written.
 * @param n Size of the buffer in bytes.
 */
void escape_buffer(void* p, size_t n);

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

  template <typename T> void save_to_file(std::filesystem::path path, T value) {
    std::ofstream ofs{path, std::ios::app};
    auto result = m_results.min;
    f64 seconds = static_cast<f64>(result.cpu_timer) / m_cpu_timer_frequency;
    f64 gib = 1 << 30;
    f64 bandwidth = result.byte_count / (gib * seconds);
    ofs << value << ',' << bandwidth << '\n';
  }

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
