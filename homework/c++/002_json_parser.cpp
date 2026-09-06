#include "haversine_parser.hpp"
#include "types.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 2 || argc == 3) {
    auto const file_content = read_file(argv[1]);
    auto tokenizer = JsonTokenizer(file_content);
    auto parser = JsonParser(std::move(tokenizer));
    auto data = parser.parse();

    auto pairCount = getPairCount(data);
    std::cout << "Input size: " << file_content.size() << '\n';
    std::cout << "Pair count: " << pairCount << '\n';
    constexpr auto max_precision{std::numeric_limits<long double>::digits10};
    auto sum = sumHaversineDistances(pairCount, data);
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

  return 0;
}
