#include <set>
#include <vector>
#include "OutcomeSpace.hpp"
#include "SigmaAlgebra.hpp"

namespace ptm {
SigmaAlgebra::SigmaAlgebra(const OutcomeSpace& omega, std::vector<Event> events) :
    omega_(omega), events_(std::move(events)) {
}

const OutcomeSpace& SigmaAlgebra::GetOutcomeSpace() const noexcept {
  return omega_;
}

const std::vector<Event>& SigmaAlgebra::GetEvents() const noexcept {
  return events_;
}

bool SigmaAlgebra::IsSigmaAlgebra() const {
  if (events_.empty())
    return false;

  std::set<Event> algebra(events_.begin(), events_.end());

  if (!algebra.contains(Event::Full(omega_.GetSize()))) {
    return false;
  }

  for (const auto& A : events_) {
    if (A.GetSize() != omega_.GetSize()) {
      return false;
    }

    if (!algebra.contains(Event::Complement(A))) {
      return false;
    }

    for (const auto& B : events_) {
      if (!algebra.contains(Event::Unite(A, B))) {
        return false;
      }
    }
  }

  return true;
}

SigmaAlgebra SigmaAlgebra::Generate(const OutcomeSpace& omega, const std::vector<Event>& generators) {
  std::set<Event> algebra;

  auto add = [&](const Event& e) { return algebra.insert(e).second; };

  add(Event::Empty(omega.GetSize()));
  add(Event::Full(omega.GetSize()));

  for (const auto& g : generators) {
    if (g.GetSize() == omega.GetSize()) {
      add(g);
    }
  }

  bool changed = true;
  while (changed) {
    changed = false;

    std::vector<Event> current(algebra.begin(), algebra.end());

    for (size_t i = 0; i < current.size(); ++i) {
      const Event& A = current[i];

      if (add(Event::Complement(A))) {
        changed = true;
      }

      for (size_t j = i + 1; j < current.size(); ++j) {
        const Event& B = current[j];

        if (add(Event::Unite(A, B))) {
          changed = true;
        }

        if (add(Event::Intersect(A, B))) {
          changed = true;
        }
      }
    }
  }

  return {omega, std::vector<Event>(algebra.begin(), algebra.end())};
}
} // namespace ptm
