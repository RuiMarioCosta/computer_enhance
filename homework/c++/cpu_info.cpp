#include <intrin.h>
#include <iostream>

static void PrintCpuSimdSupport() {
  int cpuInfo[4]{};
  __cpuidex(cpuInfo, 1, 0);

  bool osxsave = (cpuInfo[2] & (1 << 27)) != 0;
  bool avxCpu = (cpuInfo[2] & (1 << 28)) != 0;
  bool avx512Cpu = false;

  bool avxOs = false;
  bool avx512Os = false;
  if (osxsave) {
   unsigned long long xcr0 = _xgetbv(0);
   avxOs = (xcr0 & 0x6) == 0x6; // XMM + YMM state enabled
   avx512Os = (xcr0 & 0xE6) == 0xE6; // XMM + YMM + Opmask + ZMM state
  }

  __cpuidex(cpuInfo, 7, 0);
  avx512Cpu = (cpuInfo[1] & (1 << 16)) != 0;

  bool avx2Cpu = (cpuInfo[1] & (1 << 5)) != 0;
  bool avx2Os = avxOs && avx2Cpu;
  bool avx512 = avx512Os && avx512Cpu;

  std::cout << "CPU AVX: " << avxCpu << "\n";
  std::cout << "OS AVX: " << avxOs << "\n";
  std::cout << "CPU AVX2: " << avx2Cpu << "\n";
  std::cout << "OS AVX2: " << avx2Os << "\n";
  std::cout << "CPU AVX-512: " << avx512Cpu << "\n";
  std::cout << "OS AVX-512: " << avx512Os << "\n";
  std::cout << "AVX-512 usable: " << avx512 << "\n";
}

int main() {
  PrintCpuSimdSupport();
  return 0;
}
