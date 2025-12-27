#include "ExponentialDistribution.hpp"

using namespace ptm;

ExponentialDistribution::ExponentialDistribution(double lambda) : lambda_(lambda) {
}

double ExponentialDistribution::Pdf(double x) const {
  return lambda_ * std::exp(-lambda_ * x);
}

double ExponentialDistribution::Cdf(double x) const {
  return 1.0 - std::exp(-lambda_ * x);
}

double ExponentialDistribution::Sample(std::mt19937& rnd) const {
  std::exponential_distribution<double> dist(lambda_);

  return dist(rnd);
}

double ExponentialDistribution::TheoreticalMean() const {
  return 1.0 / lambda_;
}

double ExponentialDistribution::TheoreticalVariance() const {
  return 1.0 / (lambda_ * lambda_);
}
