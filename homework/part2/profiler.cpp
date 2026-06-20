#include "profiler.hpp"

#include "metrics.hpp"
#include "types.hpp"

#include <format>
#include <iomanip>
#include <iostream>
#include <print>
#include <string_view>

static profiler GlobalProfiler;
static u32 GlobalProfilerParent;

#if PROFILER
static std::array<profile_anchor, GlobalProfilerAnchorCount>
    GlobalProfilerAnchors;

profile_scope::profile_scope(std::string_view label, size_t anchor_index,
                             u64 byte_count)
    : m_label{label}, m_index{anchor_index},
      m_parent_index{GlobalProfilerParent},
      m_old_tsc_elapsed_inclusive{
          GlobalProfilerAnchors[anchor_index].tsc_elapsed_inclusive},
      m_start{ReadCPUTimer()} {
  GlobalProfilerParent = m_index;
  GlobalProfilerAnchors[anchor_index].processed_byte_count += byte_count;
}

profile_scope::~profile_scope() {
  u64 elapsed = ReadCPUTimer() - m_start;

  profile_anchor* anchor = &GlobalProfilerAnchors[m_index];
  anchor->tsc_elapsed_inclusive = m_old_tsc_elapsed_inclusive + elapsed;
  anchor->tsc_elapsed_exclusive += elapsed;
  ++anchor->hit_count;
  anchor->label = m_label;

  profile_anchor* parent_anchor = &GlobalProfilerAnchors[m_parent_index];
  parent_anchor->tsc_elapsed_exclusive -= elapsed;

  GlobalProfilerParent = m_parent_index;
}

static void print_profile(u64 total_elapsed, profile_anchor const& anchor) {
  u64 elapsed = anchor.tsc_elapsed_exclusive;
  f64 percent = 100.0 * ((f64)elapsed / (f64)total_elapsed);

  std::string line =
      std::format("  {}[{}]: {} ({:.2f}%)", anchor.label, anchor.hit_count,
                  anchor.tsc_elapsed_exclusive, percent);

  if (elapsed != anchor.tsc_elapsed_inclusive) {
    f64 percent_with_children =
        100.0 * ((f64)(anchor.tsc_elapsed_inclusive) / (f64)total_elapsed);
    line += std::format(" {:.2f}% w/children", percent_with_children);
  }

  line += ")";

  if (anchor.processed_byte_count) {
    constexpr f64 mebibyte = 1024.0 * 1024.0;
    constexpr f64 gibibyte = mebibyte * 1024.0;
    f64 seconds = (f64)anchor.tsc_elapsed_inclusive / (f64)GetCPUFreq();
    f64 bytes_per_second = (f64)anchor.processed_byte_count / seconds;
    f64 mebibytes = (f64)anchor.processed_byte_count / mebibyte;
    f64 gibibytes_per_second = bytes_per_second / gibibyte;
    line +=
        std::format(" {:.3f}mb at {:.2f}gb/s", mebibytes, gibibytes_per_second);
  }

  std::println("{}", line);
}

void PrintAnchorData(u64 total_elapsed) {
  for (auto const& anchor : GlobalProfilerAnchors) {
    if (anchor.tsc_elapsed_inclusive) {
      print_profile(total_elapsed, anchor);
    }
  }
}

#else

#define PrintAnchorData(...)

#endif

void BeginProfile() { GlobalProfiler.start = ReadCPUTimer(); }

void EndAndPrintProfile() {
  GlobalProfiler.end = ReadCPUTimer();

  u64 CPUFreq = GetCPUFreq();
  u64 TotalCPUElapsed = GlobalProfiler.end - GlobalProfiler.start;
  if (CPUFreq) {
    std::println("\nTotal time: {:.4f}ms (CPU freq {})",
                 1000.0 * TotalCPUElapsed / CPUFreq, CPUFreq);
  }
  PrintAnchorData(TotalCPUElapsed);
}
