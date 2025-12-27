#ifndef PTM_OUTCOMESPACE_HPP_
#define PTM_OUTCOMESPACE_HPP_

#include <string>
#include <vector>

namespace ptm {

class OutcomeSpace {
public:
  using OutcomeId = size_t;

  OutcomeSpace() = default;

  OutcomeId AddOutcome(const std::string& name);

  [[nodiscard]] size_t GetSize() const noexcept;
  [[nodiscard]] const std::string& GetName(OutcomeId id) const;

  auto operator<=>(const OutcomeSpace& other) const = default;

private:
  std::vector<std::string> names_;
};

} // namespace ptm

#endif // PTM_OUTCOMESPACE_HPP_
