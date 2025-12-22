#include "Event.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace ptm {

Event::Event(std::vector<bool> mask) : mask_(std::move(mask)) {
}

size_t Event::GetSize() const noexcept {
  return mask_.size();
}

bool Event::Contains(OutcomeSpace::OutcomeId id) const {
  return id < mask_.size() ? mask_.at(id) : false;
}

const std::vector<bool>& Event::GetMask() const noexcept {
  return mask_;
}

Event Event::Empty(std::size_t n) {
  return Event(std::vector<bool>(n, false));
}

Event Event::Full(std::size_t n) {
  return Event(std::vector(n, true));
}

Event Event::Complement(const Event& e) {
  Event result;
  result.mask_.resize(e.mask_.size());
  for (std::size_t i = 0; i < e.mask_.size(); ++i) {
    result.mask_[i] = !e.mask_[i];
  }
  return result;
}

Event Event::Unite(const Event& a, const Event& b) {
  Event result;

  if (a.mask_.size() != b.mask_.size()) {
    throw std::logic_error("Events should belong to the same outcome space");
  }

  result.mask_.resize(a.mask_.size());

  for (std::size_t i = 0; i < a.mask_.size(); ++i) {
    result.mask_[i] = a.Contains(i) || b.Contains(i);
  }

  return result;
}

Event Event::Intersect(const Event& a, const Event& b) {
  Event result;
  if (a.mask_.size() != b.mask_.size()) {
    throw std::logic_error("Events should belong to the same outcome space");
  }

  result.mask_.resize(a.mask_.size());

  for (std::size_t i = 0; i < a.mask_.size(); ++i) {
    result.mask_[i] = a.Contains(i) || b.Contains(i);
  }

  return result;
}

} // namespace ptm
