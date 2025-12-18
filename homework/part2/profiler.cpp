#include "profiler.hpp"

#include "metrics.hpp"
#include "types.hpp"

#include <iomanip>
#include <iostream>
#include <string_view>

static profiler GlobalProfiler;

profile_point::profile_point(std::string_view label) : m_start{ReadCPUTimer()} {
  GlobalProfiler.points[GlobalProfiler.index].label = label;
  GlobalProfiler.index++;
}

profile_point::~profile_point() {
  u64 elapsed = ReadCPUTimer() - m_start;
  GlobalProfiler.points[GlobalProfiler.index].time = elapsed;
}

void BeginProfile() { GlobalProfiler.start = ReadCPUTimer(); }

#define TimeBlock(Name)                                                        \
  profile_point { Name }
#define TimeFunction TimeBlock(__func__)

void print(std::string_view desc, u64 elapsed, u64 begin, u64 end) {
  std::cout << std::setprecision(2) << desc << ": " << (end - begin) << " ("
            << 100.f * (end - begin) / elapsed << "%)\n";
}

void EndAndPrintProfile() {
  GlobalProfiler.end = ReadCPUTimer();

  u64 CPUFreq = GetCPUFreq();
  u64 TotalCPUElapsed = GlobalProfiler.end - GlobalProfiler.start;
  if (CPUFreq) {
    printf("\nTotal time: %0.4fms (CPU freq %llu)\n",
           1000.0 * (f64)TotalCPUElapsed / (f64)CPUFreq, CPUFreq);
  }

  u64 prev_time = GlobalProfiler.start;
  for (auto const &point : GlobalProfiler.points) {
    print(point.label, TotalCPUElapsed, point.time, prev_time);
    prev_time = point.time;
  }
}
