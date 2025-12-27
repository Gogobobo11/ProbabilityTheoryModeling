#include <cassert>
#include <random>
#include "UniformDistribution.hpp"

using namespace ptm;

UniformDistribution::UniformDistribution(double a, double b) : a_(a), b_(b) {
}

double UniformDistribution::Pdf(double x) const {
  if (x < a_ or x > b_) {
    return 0;
  }

  return 1 / (b_ - a_);
}

double UniformDistribution::Cdf(double x) const {
  if (x > b_)
    return 1;

  if (x < a_)
    return 0;

  return (x - a_) / (b_ - a_);
}

double UniformDistribution::Sample(std::mt19937& rnd) const {
  std::uniform_real_distribution<double> dist(a_, b_);

  return dist(rnd);
}

double UniformDistribution::TheoreticalMean() const {
  return (b_ + a_) / 2;
}

double UniformDistribution::TheoreticalVariance() const {
  static const double kVarianceKoef = 12;
  return (b_ - a_) * (b_ - a_) / kVarianceKoef;
}
