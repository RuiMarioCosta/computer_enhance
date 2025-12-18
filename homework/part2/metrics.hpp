#include "types.hpp"

#if _WIN32

#include <intrin.h>

#else

#include <sys/time.h>
#include <x86intrin.h>

#endif

u64 GetOSTimerFreq(void);
u64 ReadOSTimer(void);

/* NOTE(casey): This does not need to be "inline", it could just be "static"
   because compilers will inline it anyway. But compilers will warn about
   static functions that aren't used. So "inline" is just the simplest way
   to tell them to stop complaining about that. */
inline u64 ReadCPUTimer(void) {
  // NOTE(casey): If you were on ARM, you would need to replace __rdtsc
  // with one of their performance counter read instructions, depending
  // on which ones are available on your platform.

  return __rdtsc();
}

u64 GetCPUFreq();
