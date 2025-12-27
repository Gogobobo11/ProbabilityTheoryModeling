#include <optional>
#include <stdexcept>
#include "DiscreteRandomVariable.hpp"
#include "ProbabilityMeasure.hpp"

namespace ptm {

DiscreteRandomVariable::DiscreteRandomVariable(const OutcomeSpace& omega,
                                               const ProbabilityMeasure& P,
                                               std::vector<double> values) :
    omega_(omega), P_(P), values_(std::move(values)) {
  if (P_.GetOutcomeSpace() != omega_) {
    throw std::logic_error("Probability measure is defined for different outcome space");
  }
  if (omega_.GetSize() != values_.size()) {
    throw std::logic_error("Set of values must match the outcome space size");
  }
}

std::optional<double> DiscreteRandomVariable::Value(OutcomeSpace::OutcomeId id) const {
  if (id > values_.size()) {
    return std::nullopt;
  }
  return values_[id];
}

double DiscreteRandomVariable::ExpectedValue() const {
  double sum = 0.0;
  double c = 0.0;
  for (std::size_t i = 0; i < values_.size(); ++i) {
    double y = values_[i] * P_.GetAtomicProbability(i) - c;
    double t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }

  return sum;
}

} // namespace ptm
