#include "MaxwellDistribution.hpp"

#include <cassert>
#include <cmath>
#include <numbers>
#include <random>

using namespace ptm;
using namespace std::numbers;

MaxwellDistribution::MaxwellDistribution(double a) : a_(a) {
  assert(a > 0);
}

double MaxwellDistribution::Pdf(double x) const {
  assert(x > 0);

  return (inv_sqrtpi * sqrt2 * x * x / std::pow(a_, 3) * std::exp(-(x * x) / (2 * a_ * a_)));
}

double MaxwellDistribution::Cdf(double x) const {
  assert(x > 0);

  return (std::erf(x / (a_ * sqrt2)) - inv_sqrtpi * sqrt2 * x / a_ * std::exp(-(x * x) / (2 * a_ * a_)));
}

double MaxwellDistribution::Sample(std::mt19937& rnd) const {
  std::normal_distribution<double> dist(0, 1);

  return a_ * std::hypot(dist(rnd), dist(rnd), dist(rnd));
}

double MaxwellDistribution::TheoreticalMean() const {
  return 2 * a_ * sqrt2 * inv_sqrtpi;
}

double MaxwellDistribution::TheoreticalVariance() const {
  return a_ * a_ * (3 * pi - 8) / pi;
}

