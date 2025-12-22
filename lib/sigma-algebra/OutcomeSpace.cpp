#include "OutcomeSpace.hpp"

namespace ptm {

OutcomeSpace::OutcomeId OutcomeSpace::AddOutcome(const std::string& name) {
  names_.push_back(name);
  return names_.size() - 1;
}

size_t OutcomeSpace::GetSize() const noexcept {
  return names_.size();
}
const std::string& OutcomeSpace::GetName(OutcomeId id) const {
  return names_.at(id);
}

} // namespace ptm
