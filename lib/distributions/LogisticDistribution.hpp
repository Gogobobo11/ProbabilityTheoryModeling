#ifndef PTM_LOGISTICDISTRIBUTION_HPP_
#define PTM_LOGISTICDISTRIBUTION_HPP_

#include <random>

#include "Distribution.hpp"

namespace ptm {

// 
class LogisticDistribution : public Distribution {
public:
  explicit LogisticDistribution(double mu, double s);

  [[nodiscard]] double Pdf(double x) const override;
  [[nodiscard]] double Cdf(double x) const override;
  double Sample(std::mt19937& rng) const override;

  [[nodiscard]] double TheoreticalMean() const override;
  [[nodiscard]] double TheoreticalVariance() const override;

private:
  double mu_;
  double s_;
};

} // namespace ptm

#endif // PTM_LOGISTICDISTRIBUTION_HPP_
