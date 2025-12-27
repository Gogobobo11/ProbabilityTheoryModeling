#include <cassert>
#include <cmath>
#include <random>
#include "PoissonDistribution.hpp"

using namespace ptm;

double Factorial(double n) {
  long long x = 1;

  for (int i = 1; i <= n; ++i) {
    x *= i;
  }

  return (double) x;
}

PoissonDistribution::PoissonDistribution(double lambda) : lambda_(lambda) {
  assert(lambda > 0);
}

double PoissonDistribution::Pdf(double x) const {
  x = std::round(x);

  return std::pow(lambda_, x) * std::exp(-lambda_) / Factorial(x);
}

double PoissonDistribution::Cdf(double x) const {
  double sum = 0;

  for (int i = 0; i <= x; ++i) {
    sum += std::pow(lambda_, i) / Factorial(i);
  }

  return std::exp(-lambda_) * sum;
}

double PoissonDistribution::Sample(std::mt19937& rnd) const {
  std::poisson_distribution<> dist(lambda_);

  return dist(rnd);
}

double PoissonDistribution::TheoreticalMean() const {
  return lambda_;
}

double PoissonDistribution::TheoreticalVariance() const {
  return lambda_;
}

