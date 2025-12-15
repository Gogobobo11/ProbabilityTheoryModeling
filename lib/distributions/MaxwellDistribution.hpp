#ifndef PTM_MAXWELLDISTRIBUTION_HPP_
#define PTM_MAXWELLISTRIBUTION_HPP_

#include <random>

#include "Distribution.hpp"

namespace ptm {

// Максвелла-Больцмана
class MaxwellDistribution : public Distribution {
public:
  MaxwellDistribution(double a);

  [[nodiscard]] double Pdf(double x) const override;
  [[nodiscard]] double Cdf(double x) const override;
  double Sample(std::mt19937& rng) const override;

  [[nodiscard]] double TheoreticalMean() const override;
  [[nodiscard]] double TheoreticalVariance() const override;

private:
  double a_;
};

} // namespace ptm

#endif // PTM_MAXWELLDISTRIBUTION_HPP_
