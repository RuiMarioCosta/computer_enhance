#include <array>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string_view>

typedef uint64_t u64;
typedef double f64;

#include "74_metrics.cpp"

struct measure {
  u64 time;
  char const *label;
};

struct profiler {
  std::array<measure, 4096> points;
  u64 start;
  u64 end;
  size_t index = 0;
};

static profiler GlobalProfiler;

class profile_point {
  u64 m_start;
  size_t m_index;

public:
  profile_point(std::string_view label) : m_start{ReadCPUTimer()} {
    GlobalProfiler.points[GlobalProfiler.index].label = label;
    GlobalProfiler.index++;
  }
  ~profile_point() {
    u64 elapsed = ReadCPUTimer() - m_start;
    GlobalProfiler.points[GlobalProfiler.index].time = elapsed;
  };
};

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
