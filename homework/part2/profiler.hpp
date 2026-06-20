#include "types.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <string_view>

#ifndef PROFILER
#define PROFILER 1
#endif

#if PROFILER

constexpr size_t GlobalProfilerAnchorCount = 4096;

struct profile_anchor {
  u64 tsc_elapsed_exclusive; // does not include children
  u64 tsc_elapsed_inclusive; // includes children
  u64 hit_count;
  std::string_view label;
};

class profile_scope {
  std::string_view m_label;
  size_t m_index;
  size_t m_parent_index;
  u64 m_old_tsc_elapsed_inclusive;
  u64 m_start;

public:
  profile_scope(std::string_view label, size_t anchor_index);
  ~profile_scope();
};

#define NameConcat2(A, B) A##B
#define NameConcat(A, B) NameConcat2(A, B)
#define TimeBlock(Name)                                                        \
  profile_scope NameConcat(Block, __LINE__)(Name, __COUNTER__ + 1);
#define ProfilerEndOfCompilationUnit                                           \
  static_assert(                                                               \
      __COUNTER__ < GlobalProfilerAnchorCount,                                 \
      "Number of profile points exceeds size of profiler::Anchors array")

#else

#define TimeBlock(...)
#define PrintAnchorData(...)
#define ProfilerEndOfCompilationUnit

#endif

#define TimeFunction TimeBlock(__func__)

struct profiler {
  u64 start;
  u64 end;
};

void BeginProfile();

void EndAndPrintProfile();
