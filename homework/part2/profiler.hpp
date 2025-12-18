#include "types.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <string_view>

struct measure {
  u64 time;
  std::string_view label;
};

struct profiler {
  std::array<measure, 4096> points;
  u64 start;
  u64 end;
  size_t index = 0;
};

class profile_point {
  u64 m_start;
  size_t m_index;

public:
  profile_point(std::string_view label);
  ~profile_point();
};

void BeginProfile();

#define TimeBlock(Name)                                                        \
  profile_point { Name }
#define TimeFunction TimeBlock(__func__)

void print(std::string_view desc, u64 elapsed, u64 begin, u64 end);
void EndAndPrintProfile();
