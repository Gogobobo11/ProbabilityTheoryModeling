#include "LogisticDistribution.hpp"

#include <cassert>
#include <cmath>
#include <numbers>
#include <random>

using namespace ptm;

LogisticDistribution::LogisticDistribution(double mu, double s) : mu_(mu), s_(s) {
  assert(s > 0);
}

double LogisticDistribution::Pdf(double x) const {
  const double kExpCoef = std::exp(-(x - mu_) / s_);

  return (kExpCoef / (s_ * std::pow(1 + kExpCoef, 2)));
}

double LogisticDistribution::Cdf(double x) const {
  const double kExpCoef = std::exp(-(x - mu_) / s_);

  return 1 / (1 + kExpCoef);
}

double LogisticDistribution::Sample(std::mt19937& rnd) const {
  std::uniform_real_distribution<double> dist;

  return -s_ * std::log(1 / dist(rnd) - 1) + mu_;
}

double LogisticDistribution::TheoreticalMean() const {
  return mu_;
}

double LogisticDistribution::TheoreticalVariance() const {
  return s_ * s_ * std::numbers::pi * std::numbers::pi / 3;
}

