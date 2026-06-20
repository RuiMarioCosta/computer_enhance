#include "profiler.hpp"

#include "metrics.hpp"
#include "types.hpp"

#include <iomanip>
#include <iostream>
#include <print>
#include <string_view>

static profiler GlobalProfiler;
static u32 GlobalProfilerParent;

profile_scope::profile_scope(std::string_view label, size_t anchor_index)
    : m_label{label}, m_index{anchor_index},
      m_parent_index{GlobalProfilerParent},
      m_old_tsc_elapsed_inclusive{
          GlobalProfiler.anchors[anchor_index].tsc_elapsed_inclusive},
      m_start{ReadCPUTimer()} {
  GlobalProfilerParent = m_index;
}

profile_scope::~profile_scope() {
  u64 elapsed = ReadCPUTimer() - m_start;

  profile_anchor* anchor = &GlobalProfiler.anchors[m_index];
  anchor->tsc_elapsed_inclusive = m_old_tsc_elapsed_inclusive + elapsed;
  anchor->tsc_elapsed_exclusive += elapsed;
  ++anchor->hit_count;
  anchor->label = m_label;

  profile_anchor* parent_anchor = &GlobalProfiler.anchors[m_parent_index];
  parent_anchor->tsc_elapsed_exclusive -= elapsed;

  GlobalProfilerParent = m_parent_index;
}

void BeginProfile() { GlobalProfiler.start = ReadCPUTimer(); }

static void print_profile(u64 total_elapsed, profile_anchor const& anchor) {
  u64 elapsed = anchor.tsc_elapsed_exclusive;
  f64 percent = 100.0 * ((f64)elapsed / (f64)total_elapsed);
  if (elapsed != anchor.tsc_elapsed_inclusive) {
    f64 percent_with_children =
        100.0 * ((f64)(anchor.tsc_elapsed_inclusive) / (f64)total_elapsed);
    std::println("  {}[{}]: {} ({:.2f}%, {:.2f}% w/children)", anchor.label,
                 anchor.hit_count, anchor.tsc_elapsed_exclusive, percent,
                 percent_with_children);
  } else {
    std::println("  {}[{}]: {} ({:.2f}%)", anchor.label, anchor.hit_count,
                 anchor.tsc_elapsed_exclusive, percent);
  }
}

void EndAndPrintProfile() {
  GlobalProfiler.end = ReadCPUTimer();

  u64 CPUFreq = GetCPUFreq();
  u64 TotalCPUElapsed = GlobalProfiler.end - GlobalProfiler.start;
  if (CPUFreq) {
    std::println("\nTotal time: {:.4f}ms (CPU freq {})",
                 1000.0 * TotalCPUElapsed / CPUFreq, CPUFreq);
  }

  for (auto const& anchor : GlobalProfiler.anchors) {
    if (anchor.tsc_elapsed_inclusive) {
      print_profile(TotalCPUElapsed, anchor);
    }
  }
}
