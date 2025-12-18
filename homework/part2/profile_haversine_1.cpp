#include "haversine_parser.hpp"
#include "metrics.hpp"
#include "types.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string_view>

void print(std::string_view desc, u64 elapsed, u64 begin, u64 end) {
  std::cout << std::setprecision(2) << desc << ": " << (end - begin) << " ("
            << 100.f * (end - begin) / elapsed << "%)\n";
}

int main(int argc, char *argv[]) {
  u64 Prof_Begin = 0;
  u64 Prof_Read = 0;
  u64 Prof_MiscSetup = 0;
  u64 Prof_Parse = 0;
  u64 Prof_GetPairs = 0;
  u64 Prof_Sum = 0;
  u64 Prof_MiscOutput = 0;
  u64 Prof_End = 0;

  Prof_Begin = ReadCPUTimer();

  if (argc == 2 || argc == 3) {
    Prof_Read = ReadCPUTimer();
    auto const file_content = read_file(argv[1]);
    Prof_MiscSetup = ReadCPUTimer();
    auto tokenizer = JsonTokenizer(file_content);
    Prof_Parse = ReadCPUTimer();
    auto parser = JsonParser(std::move(tokenizer));
    auto data = parser.parse();

    Prof_GetPairs = ReadCPUTimer();
    auto pairCount = getPairCount(data);
    std::cout << "Input size: " << file_content.size() << '\n';
    std::cout << "Pair count: " << pairCount << '\n';
    constexpr auto max_precision{std::numeric_limits<long double>::digits10};
    Prof_Sum = ReadCPUTimer();
    auto sum = sumHaversineDistances(pairCount, data);
    Prof_MiscOutput = ReadCPUTimer();
    std::cout << std::setprecision(max_precision) << "Haversine sum: " << sum
              << '\n';

    if (argc == 3) {
      std::ifstream haver{argv[2], std::ios_base::binary};
      haver.seekg(0, std::ios_base::end);
      auto size = haver.tellg();

      u64 refCount = size / sizeof(f64) - 1;
      if (pairCount != refCount) {
        throw std::runtime_error{"pairCount != refCount"};
      }

      haver.seekg(refCount * sizeof(f64));
      f64 refSum;
      haver.read(reinterpret_cast<char *>(&refSum), sizeof(refSum));

      std::cout << "Reference sum: " << refSum << '\n';
      std::cout << "Difference: " << sum - refSum << '\n';
    }

  } else {
    std::cerr << "Usage: " << argv[0] << " [haversine_input.json]\n"
              << "       " << argv[0]
              << " [haversine_input.json] [answers.f64]\n";
    return 1;
  }

  std::cout << '\n';

  Prof_End = ReadCPUTimer();

  u64 TotalCPUElapsed = Prof_End - Prof_Begin;

  u64 CPUFreq = GetCPUFreq();
  if (CPUFreq) {
    printf("\nTotal time: %0.4fms (CPU freq %llu)\n",
           1000.0 * (f64)TotalCPUElapsed / (f64)CPUFreq, CPUFreq);
  }

  print("startup", TotalCPUElapsed, Prof_Begin, Prof_Read);
  print("read", TotalCPUElapsed, Prof_Read, Prof_MiscSetup);
  print("misc setup", TotalCPUElapsed, Prof_MiscSetup, Prof_Parse);
  print("parse", TotalCPUElapsed, Prof_Parse, Prof_GetPairs);
  print("get pairs", TotalCPUElapsed, Prof_GetPairs, Prof_Sum);
  print("sum", TotalCPUElapsed, Prof_Sum, Prof_MiscOutput);
  print("misc output", TotalCPUElapsed, Prof_MiscOutput, Prof_End);

  return 0;
}
