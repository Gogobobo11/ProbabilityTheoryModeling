#include "LawOfLargeNumbersSimulator.hpp"

#include <cmath>
#include <stdexcept>

namespace ptm {

LawOfLargeNumbersSimulator::LawOfLargeNumbersSimulator(std::shared_ptr<Distribution> dist) : dist_(dist) {
  if (dist_ == nullptr) {
    throw std::logic_error("Distribution cannot be null");
  }
}

std::shared_ptr<Distribution> LawOfLargeNumbersSimulator::GetDistribution() const noexcept {
  return dist_;
}

LLNPathResult LawOfLargeNumbersSimulator::Simulate(std::mt19937& rng, size_t max_n, size_t step) const {
  if (max_n == 0) {
    throw std::logic_error("max_n must be positive");
  }
  if (step == 0) {
    throw std::logic_error("step must be positive");
  }
  if (step > max_n) {
    throw std::logic_error("step cannot be greater than max_n");
  }

  LLNPathResult result;

  const double theoretical_mean = dist_->TheoreticalMean();

  double sum = 0.0;
  double c = 0.0;

  for (size_t i = 1; i <= max_n; ++i) {
    double x = dist_->Sample(rng);

    double y = x - c;
    double t = sum + y;
    c = (t - sum) - y;
    sum = t;

    if (i % step == 0) {
      double sample_mean = sum / static_cast<double>(i);
      double abs_error = std::abs(sample_mean - theoretical_mean);

      LLNPathEntry entry{.n = i, .sample_mean = sample_mean, .abs_error = abs_error};
      result.entries.push_back(entry);
    }
  }

  return result;
}

} // namespace ptm
