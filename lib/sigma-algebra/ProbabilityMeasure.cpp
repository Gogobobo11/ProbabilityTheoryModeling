#include <stdexcept>
#include "OutcomeSpace.hpp"
#include "ProbabilityMeasure.hpp"

namespace ptm {

ProbabilityMeasure::ProbabilityMeasure(const OutcomeSpace& omega) : omega_(omega), atom_probs_(omega.GetSize(), 0) {
}

void ProbabilityMeasure::SetAtomicProbability(OutcomeSpace::OutcomeId id, double p) {
  if (p < 0) {
    throw std::logic_error("probability must be positive");
  }
  atom_probs_[id] = p;
}

double ProbabilityMeasure::GetAtomicProbability(OutcomeSpace::OutcomeId id) const {
  return atom_probs_.at(id);
}

bool ProbabilityMeasure::IsValid(double eps) const {
  double sum = 0.0;
  double c = 0.0;
  for (std::size_t i = 0; i < atom_probs_.size(); ++i) {
    if (atom_probs_[i] < 0) {
      return false;
    }
    double y = atom_probs_[i] - c;
    double t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }
  return std::abs(sum - 1) < eps;
}
double ProbabilityMeasure::Probability(const Event& events) const {
  if (events.GetSize() != omega_.GetSize()) {
    throw std::logic_error("Event dimension must match outcome space");
  }

  double sum = 0.0;
  double c = 0.0;
  for (std::size_t i = 0; i < atom_probs_.size(); ++i) {
    if (events.Contains(i)) {
      double y = atom_probs_[i] - c;
      double t = sum + y;
      c = (t - sum) - y;
      sum = t;
    }
  }
  return sum;
}

const OutcomeSpace& ProbabilityMeasure::GetOutcomeSpace() const {
  return omega_;
}

} // namespace ptm
