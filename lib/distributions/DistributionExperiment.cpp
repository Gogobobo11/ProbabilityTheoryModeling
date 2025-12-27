#include "DistributionExperiment.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>

using namespace ptm;

DistributionExperiment::DistributionExperiment(std::shared_ptr<Distribution> dist, size_t sample_size) :
    dist_(std::move(dist)), sample_size_(sample_size) {
}

ExperimentStats DistributionExperiment::Run(std::mt19937& rng) {
  ExperimentStats stats;

  std::vector<double> run_results(sample_size_);

  for (auto& run : run_results) {
    run = dist_->Sample(rng);
  }

  stats.empirical_mean = std::accumulate(run_results.begin(), run_results.end(), 0.) / (double) sample_size_;

  auto tmp = run_results | std::views::transform([&](double x) { return std::pow(x - stats.empirical_mean, 2); });

  stats.empirical_variance = std::accumulate(tmp.begin(), tmp.end(), 0.) / double(sample_size_ - 1);

  stats.mean_error = std::abs(stats.empirical_mean - dist_->TheoreticalMean());
  stats.variance_error = std::abs(stats.empirical_variance - dist_->TheoreticalVariance());

  return stats;
}

std::vector<double> DistributionExperiment::EmpiricalCdf(const std::vector<double>& grid,
                                                         std::mt19937& rng,
                                                         std::size_t sample_size) {
  std::vector<double> sample(sample_size);
  for (auto& x : sample) {
    x = dist_->Sample(rng);
  }

  std::ranges::sort(sample);

  std::vector<double> cdf;
  cdf.reserve(grid.size());

  for (double x : grid) {
    auto it = std::ranges::upper_bound(sample, x);
    size_t count = std::distance(sample.begin(), it);
    cdf.push_back(double(count) / double(sample_size));
  }

  return cdf;
}

double DistributionExperiment::KolmogorovDistance(const std::vector<double>& grid,
                                                  const std::vector<double>& empirical_cdf) const {
  std::vector<double> theoretical_cdf;
  theoretical_cdf.reserve(grid.size());

  for (auto& value : grid) {
    theoretical_cdf.push_back(dist_->Cdf(value));
  }

  auto distances = std::views::iota(0u, grid.size()) |
                   std::views::transform([&](int x) { return std::abs(theoretical_cdf[x] - empirical_cdf[x]); });

  return std::ranges::max(distances);
}

