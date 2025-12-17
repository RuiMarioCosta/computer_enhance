#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t u64;
typedef double f64;

#include "76_simple_profiler.cpp"
#include "77_parse_haversine_profiled.cpp"

int main(int argc, char *argv[]) {
  BeginProfile();

  if (argc == 2 || argc == 3) {
    // Prof_Read = ReadCPUTimer();
    auto const file_content = read_file(argv[1]);
    // Prof_MiscSetup = ReadCPUTimer();
    auto tokenizer = JsonTokenizer(file_content);
    // Prof_Parse = ReadCPUTimer();
    auto parser = JsonParser(std::move(tokenizer));
    auto data = parser.parse();

    // Prof_GetPairs = ReadCPUTimer();
    auto pairCount = getPairCount(data);
    std::cout << "Input size: " << file_content.size() << '\n';
    std::cout << "Pair count: " << pairCount << '\n';
    constexpr auto max_precision{std::numeric_limits<long double>::digits10};
    // Prof_Sum = ReadCPUTimer();
    auto sum = sumHaversineDistances(pairCount, data);
    // Prof_MiscOutput = ReadCPUTimer();
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

  EndAndPrintProfile();
  return 0;
}
