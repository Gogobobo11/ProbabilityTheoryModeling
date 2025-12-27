#include "LaplaceDistribution.hpp"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <random>

using namespace ptm;

LaplaceDistribution::LaplaceDistribution(double mu, double b) : mu_(mu), b_(b) {
  assert(b > 0);
}

double LaplaceDistribution::Pdf(double x) const {
  return 1 / (2 * b_) * std::exp(-std::abs(x - mu_) / b_);
}

double LaplaceDistribution::Cdf(double x) const {
  if (x <= mu_)
    return std::exp((x - mu_) / b_) / 2;

  return 1 - std::exp(-(x - mu_) / b_) / 2;
}

double LaplaceDistribution::Sample(std::mt19937& rnd) const {
  std::uniform_real_distribution<double> dist;

  double uniform_sample = dist(rnd);

  static const double kCdfWhenXEqMu = 0.5;

  if (uniform_sample < kCdfWhenXEqMu) {
    return b_ * std::log(2 * uniform_sample) + mu_;
  }

  return mu_ - b_ * std::log(2 - 2 * uniform_sample);
}

double LaplaceDistribution::TheoreticalMean() const {
  return mu_;
}

double LaplaceDistribution::TheoreticalVariance() const {
  return 2 * b_ * b_;
}

