#include <numbers>
#include <random>
#include "NormalDistribution.hpp"

using namespace ptm;

NormalDistribution::NormalDistribution(double mean, double stddev) : mean_(mean), stddev_(stddev) {
}

double NormalDistribution::Pdf(double x) const {
  return (std::exp(-std::pow(x - mean_, 2) / (2 * stddev_ * stddev_)) /
          std::sqrt(std::numbers::pi * 2 * stddev_ * stddev_));
}

double NormalDistribution::Cdf(double x) const {
  return [](double z) { return (1 + std::erf(z / std::numbers::sqrt2)) / 2; }((x - mean_) / stddev_);
}

double NormalDistribution::Sample(std::mt19937& rnd) const {
  std::normal_distribution<double> dist(mean_, stddev_);

  return dist(rnd);
}

double NormalDistribution::TheoreticalMean() const {
  return mean_;
}

double NormalDistribution::TheoreticalVariance() const {
  return stddev_ * stddev_;
}

double NormalDistribution::GetMean() const {
  return mean_;
}

double NormalDistribution::GetStddev() const {
  return stddev_;
}
