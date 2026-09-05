#include <intrin.h>
#include <iostream>
#include <windows.h>

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
    avxOs = (xcr0 & 0x6) == 0x6;      // XMM + YMM state enabled
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

static void PrintCoreInfo() {
  // Check hybrid CPU support via CPUID leaf 7 (EDX bit 15)
  int cpuInfo[4]{};
  __cpuidex(cpuInfo, 7, 0);
  bool isHybrid = (cpuInfo[3] & (1 << 15)) != 0;
  std::cout << "Hybrid CPU: " << isHybrid << "\n";

  if (!isHybrid) {
    std::cout << "All cores are the same type.\n";
    return;
  }

  DWORD logicalCount = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
  HANDLE hThread = GetCurrentThread();

  for (DWORD i = 0; i < logicalCount; i++) {
    DWORD_PTR mask = (DWORD_PTR)1 << i;
    DWORD_PTR prev = SetThreadAffinityMask(hThread, mask);
    if (!prev)
      continue;

    Sleep(0); // yield so thread migrates to target core

    __cpuidex(cpuInfo, 0x1A, 0);
    int coreType = (cpuInfo[0] >> 24) & 0xFF;

    const char* typeName = "Unknown";
    if (coreType == 0x40)
      typeName = "P-core";
    else if (coreType == 0x20)
      typeName = "E-core";

    std::cout << "Logical core " << i << ": " << typeName << " (type=0x"
              << std::hex << coreType << std::dec << ")\n";

    SetThreadAffinityMask(hThread, prev);
  }
}

int main() {
  PrintCpuSimdSupport();
  std::cout << "\n";
  PrintCoreInfo();
  return 0;
}
