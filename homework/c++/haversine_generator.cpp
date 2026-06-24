#include "haversine_math.hpp"
#include "types.hpp"

#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <random>
#include <string>

std::uniform_real_distribution<> getDistribution(f64 center, f64 radius,
                                                 f64 limit) {
  auto min = center - radius;
  if (min < -limit) {
    min = -limit;
  }

  auto max = center + radius;
  if (max > limit) {
    max = limit;
  }

  return std::uniform_real_distribution<>(min, max);
}

int main(int argc, char *argv[]) {
  f64 EarthRadius = 6372.8;
  u64 MaxPairCount = (1ULL << 34);

  if (argc == 4) {
    char const *MethodName = argv[1];
    u64 SeedValue = atoll(argv[2]);
    size_t PairCount = atoll(argv[3]);

    auto ClusterCountLeft = std::numeric_limits<u64>::max();
    if (strcmp(MethodName, "cluster") == 0) {
      ClusterCountLeft = 0;
    } else if (strcmp(MethodName, "uniform") != 0) {
      MethodName = "uniform";
      std::cerr << "WARNING: Unrecognized method name. Using 'uniform'.\n";
    }

    if (PairCount > MaxPairCount) {
      std::cerr << "To avoid accidentally generating massive files, number of "
                   "pairs must be less than "
                << MaxPairCount << '\n';
      return 1;
    }

    f64 MaxAllowedX = 180;
    f64 MaxAllowedY = 90;

    std::mt19937 Xgen;
    Xgen.seed(SeedValue);
    std::uniform_real_distribution<> Xdis(-MaxAllowedX, MaxAllowedX);
    std::uniform_real_distribution<> XcenterDis(-MaxAllowedX, MaxAllowedX);
    std::uniform_real_distribution<> XradiusDis(0, MaxAllowedX);
    std::mt19937 Ygen;
    Ygen.seed(SeedValue);
    std::uniform_real_distribution<> Ydis(-MaxAllowedY, MaxAllowedY);
    std::uniform_real_distribution<> YcenterDis(-MaxAllowedY, MaxAllowedY);
    std::uniform_real_distribution<> YradiusDis(0, MaxAllowedY);

    std::string jsonFile = "data_" + std::to_string(PairCount) + "_flex.json";
    std::ofstream json{jsonFile};
    std::string haverFile =
        "data_" + std::to_string(PairCount) + "_haveranswer.f64";
    std::ofstream haver{haverFile, std::ios_base::binary};
    constexpr auto max_precision{std::numeric_limits<long double>::digits10 +
                                 1};
    json << "{\"pairs\":[\n" << std::setprecision(max_precision);

    f64 Sum = 0;
    for (int n = 0; n < PairCount; n++) {
      if (n % 64 == 0) {
        f64 Xcenter = XcenterDis(Xgen);
        f64 Ycenter = YcenterDis(Ygen);
        f64 Xradius = XradiusDis(Xgen);
        f64 Yradius = YradiusDis(Ygen);
        Xdis = getDistribution(Xcenter, Xradius, MaxAllowedX);
        Ydis = getDistribution(Ycenter, Yradius, MaxAllowedY);
      }
      f64 X0 = Xdis(Xgen);
      f64 X1 = Xdis(Xgen);
      f64 Y0 = Ydis(Ygen);
      f64 Y1 = Ydis(Ygen);

      f64 HaversineDistance = ReferenceHaversine(X0, Y0, X1, Y1, EarthRadius);
      Sum += HaversineDistance;

      json << "\t{\"x0\":" << X0 << ", " << "\"y0\":" << Y0 << ", "
           << "\"x1\":" << X1 << ", " << "\"y1\":" << Y1 << "},\n";
      haver.write(reinterpret_cast<char *>(&HaversineDistance),
                  sizeof(HaversineDistance));
    }
    json.seekp(-3, std::ios_base::end);
    json << "\n]}";
    Sum /= static_cast<f64>(PairCount);
    haver.write(reinterpret_cast<char *>(&Sum), sizeof(Sum));

    std::cout << "Method: " << MethodName << '\n'
              << "Random seed: " << SeedValue << '\n'
              << "Pair count: " << PairCount << '\n'
              << "Expected sum: " << Sum << '\n';

  } else {
    std::cerr << "Usage: " << argv[0]
              << " [uniform/cluster] [random seed] "
                 "[number of coordinate pairs to generate]\n";
    return 1;
  }

  return 0;
}
